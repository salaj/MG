#include "bezier_curve.h"
#include "shader_simple_point.h"

using namespace gk2;

int BezierCurve::CurveCounter = 0;

BezierCurve::BezierCurve(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera& camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	ModelClass::Initialize();
}

BezierCurve::BezierCurve(Service& service) : ModelClass(service)
{
	m_Type = ModelType::BezierType;
	ModelClass::Initialize();
}

BezierCurve::BezierCurve(const BezierCurve& other) : ModelClass(other)
{
}


BezierCurve::~BezierCurve()
{
}

BezierCurve::BezierCurve()
{
}

void* BezierCurve::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierCurve::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BezierCurve::SetNodes(vector<ModelClass*>& nodes)
{
	int numberOfNodes = nodes.size();
	vector<SimplePoint*> simplePoints = vector<SimplePoint*>();
	for (int i = 0; i < numberOfNodes; i++)
	{
		simplePoints.push_back(dynamic_cast<SimplePoint*>(nodes[i]));
	}
	m_nodes = simplePoints;
	Reset();
}


void BezierCurve::InsertNodeAfter(SimplePoint* node, SimplePoint* after)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->m_id == after->m_id)
			m_nodes.insert(m_nodes.begin() + i + 1, node);
	}
}

void BezierCurve::RemoveNode(SimplePoint* node)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->m_id == node->m_id)
			m_nodes.erase(m_nodes.begin() + i);
	}
}

void BezierCurve::InsertNodeAt(SimplePoint* node, int position)
{
	m_nodes.insert(m_nodes.begin() + position, node);
}

void BezierCurve::RemoveNodeAt(int position)
{
	m_nodes.erase(m_nodes.begin() + position);
}

void BezierCurve::UpdateNode(SimplePoint* point)
{
	if (m_segments.size() == 0)
		return;
	list<VertexPos*> vertices;
	m_vertexCountContour = m_nodes.size() * 2 - 2;
	int internalNodesCounter = 0;
	VertexPos* verticesContour = new VertexPos[m_vertexCountContour];
	for (int i = 0; i < m_segments.size(); i++)
	{
		if (m_segments[i]->Contain(point))
			m_segments[i]->calculate(nullptr);
		list<VertexPos*> singleSegmentPoints = m_segments[i]->GetSegmentPoints();
		//concatenate segment points with target vertices
		vertices.splice(vertices.end(), singleSegmentPoints);

		int numberOfInternalNodesPerSegment = m_segments[i]->m_nodes.size();
		for (int j = 0; j < numberOfInternalNodesPerSegment; j++)
		{
			verticesContour[internalNodesCounter++] = {
				XMFLOAT3(m_segments[i]->m_nodes[j]->GetPosition().x,
				m_segments[i]->m_nodes[j]->GetPosition().y,
				m_segments[i]->m_nodes[j]->GetPosition().z)
			};
			if (j > 0 && j < numberOfInternalNodesPerSegment - 1)
			{
				verticesContour[internalNodesCounter] = verticesContour[internalNodesCounter - 1];
				internalNodesCounter++;
			}
		}
	}
	if (vertices.size() == 0)
	{
		//there must be only one point, or many in the same place, create fake point to draw, but not segment
		vertices.push_back(new VertexPos{
			XMFLOAT3(m_nodes[0]->GetPosition().x,
			m_nodes[0]->GetPosition().y,
			m_nodes[0]->GetPosition().z), //position of single point = A
		});
	}

	m_vertexCount = vertices.size();
	VertexPos *arr = new VertexPos[m_vertexCount];
	int ind = 0;
	for (list<VertexPos*>::iterator it = vertices.begin(); it != vertices.end(); it++)
	{
		arr[ind++] = *(*it);
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(arr, m_vertexCount);
	m_vertexBufferContour = m_device.CreateVertexBuffer(verticesContour, m_vertexCountContour);
	delete arr;
	delete verticesContour;
	setPointTopology();
}

void BezierCurve::Reset()
{
	list<VertexPos*> vertices = list<VertexPos*>();
	//we start from Second Point cause we want segments continuity C0
	int index = 0;
	int numberOfControlPoints = m_nodes.size() - 1;
	int segmentLength = 0;
	m_segments.clear();
	while (numberOfControlPoints > 0)
	{
		BezierSegment* segment = new BezierSegment();
		vector<SimplePoint*>* internalSegmentNodes = new vector<SimplePoint*>();
		internalSegmentNodes->push_back(m_nodes[index]);
		if (numberOfControlPoints / 3 >= 1)
		{
			//there are at least 3 points more so create Cubic Segment
			internalSegmentNodes->push_back(m_nodes[index+1]);
			internalSegmentNodes->push_back(m_nodes[index+2]);
			internalSegmentNodes->push_back(m_nodes[index+3]);
			numberOfControlPoints -= 3;
			index += 3;
			segment->m_nodes = *internalSegmentNodes;
			segment->calculate(&BezierSegment::calculateCubic);
		}
		else if (numberOfControlPoints == 2)
		{
			//there are only 2 point left so create Square Segment
			internalSegmentNodes->push_back(m_nodes[index + 1]);
			internalSegmentNodes->push_back(m_nodes[index + 2]);
			numberOfControlPoints -= 2;
			index += 2;
			segment->m_nodes = *internalSegmentNodes;
			segment->calculate(&BezierSegment::calculateSquare);
		}
		else if (numberOfControlPoints == 1)
		{
			//there is only 1 point left so create Line Segment
			internalSegmentNodes->push_back(m_nodes[index + 1]);
			numberOfControlPoints -= 1;
			index += 1;
			segment->m_nodes = *internalSegmentNodes;
			segment->calculate(&BezierSegment::calculateLine);
		}
		m_segments.push_back(segment);
	}

	int segmentCount = m_segments.size();
	m_vertexCountContour = m_nodes.size() * 2 - 2;
	VertexPos* verticesContour;
	if (m_vertexCountContour > 0)
		verticesContour = new VertexPos[m_vertexCountContour];
	int internalNodesCounter = 0;
	for (int i = 0; i < segmentCount; i++)
	{
		list<VertexPos*> singleSegmentPoints = m_segments[i]->GetSegmentPoints();
		//concatenate segment points with target vertices
		vertices.splice(vertices.end(), singleSegmentPoints);
		//delete m_segments[i];

		int numberOfInternalNodesPerSegment = m_segments[i]->m_nodes.size();
		for (int j = 0; j < numberOfInternalNodesPerSegment; j++)
		{
			verticesContour[internalNodesCounter++] = {
				XMFLOAT3(m_segments[i]->m_nodes[j]->GetPosition().x,
				m_segments[i]->m_nodes[j]->GetPosition().y,
				m_segments[i]->m_nodes[j]->GetPosition().z)
			};
			if (j > 0 && j < numberOfInternalNodesPerSegment - 1)
			{
				verticesContour[internalNodesCounter] = verticesContour[internalNodesCounter - 1];
				internalNodesCounter++;
			}
		}
	} 

	if (vertices.size() == 0)
	{
		//there must be only one point, create fake point to draw, but not segment
		vertices.push_back(new VertexPos{
			XMFLOAT3(m_nodes[index]->GetPosition().x,
			m_nodes[index]->GetPosition().y,
			m_nodes[index]->GetPosition().z), //position of single point = A
		});

		m_vertexCountContour = 1;
		verticesContour = new VertexPos[m_vertexCountContour];
		verticesContour[0] = *vertices.front();
	}


	m_vertexCount = vertices.size();
	VertexPos *arr = new VertexPos[m_vertexCount];
	int ind = 0;
	for (list<VertexPos*>::iterator it=vertices.begin(); it != vertices.end(); it++)
	{
		arr[ind++] = *(*it);
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(arr, m_vertexCount);
	m_vertexBufferContour = m_device.CreateVertexBuffer(verticesContour, m_vertexCountContour);
	delete arr;
	delete verticesContour;
	//setTriangleTopology();
	//setLineTopology();
	setPointTopology();
}



void BezierCurve::Initialize()
{
	////////////////FIXME////////
	if (m_nodes.size() < 4)
	{
		m_vertexCount = m_vertexCountContour = 1;
		VertexPos* vertices = new VertexPos[m_vertexCount];
		m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
		m_vertexBufferContour = m_device.CreateVertexBuffer(vertices, m_vertexCountContour);
		setPointTopology();
		delete[] vertices;
		return;
	}
	///////////////////////////////
	//XMVECTOR A = XMLoadFloat4(&m_nodes[0]->GetPosition());
	//XMVECTOR B = XMLoadFloat4(&m_nodes[1]->GetPosition());
	//XMVECTOR C = XMLoadFloat4(&m_nodes[2]->GetPosition());
	//XMVECTOR D = XMLoadFloat4(&m_nodes[3]->GetPosition());



	//// Set the number of vertices in the vertex array.
	////m_vertexCount = bezier_length();// 20;//m_nodes.size();

	//VertexPos* vertices = new VertexPos[m_vertexCount];

	//for (int i = 0; i < m_vertexCount; i++)
	//{
	//	vertices[i] = {
	//		calculateCubic((float)i / (float)(m_vertexCount - 1)),
	//		XMFLOAT3(0.0f, 0.0f, 1.0f) 
	//	};

	//}

	//m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	//delete vertices;

	////setTriangleTopology();
	////setLineTopology();
	//setPointTopology();
}

void BezierCurve::setPointTopology()
{
	m_indexCount = m_vertexCount;
	unsigned short* indices = new unsigned short[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
	delete[] indices;
	m_indexCountContour = m_vertexCountContour;
	unsigned short* indicesContour = new unsigned short[m_indexCountContour];
	for (int i = 0; i < m_indexCountContour; i++)
	{
		indicesContour[i] = i;
	}
	m_indexBufferContour = m_device.CreateIndexBuffer(indicesContour, m_indexCountContour);
	delete[] indicesContour;
}

void BezierCurve::setLineTopology()
{

	// Set the number of indices in the index array.
	m_indexCount = 38;

	unsigned short* indices = new unsigned short[m_indexCount];
	indices[0] = 0;
	for (int i = 1; i <= 18; i++)
	{
		indices[2 * i] = i;
		indices[2 * i + 1] = i;
	}
	indices[37] = 19;
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
	delete indices;
}

void BezierCurve::setTriangleTopology()
{
	// Set the number of indices in the index array.
	m_indexCount = 36;

	unsigned short indices[] =
	{
		0, 1, 2, 0, 2, 3,		//Front face
		4, 5, 6, 4, 6, 7,		//Left face
		8, 9, 10, 8, 10, 11,	//Botton face
		12, 13, 14, 12, 14, 15,	//Back face
		16, 17, 18, 16, 18, 19,	//Right face
		20, 21, 22, 20, 22, 23	//Top face
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

void BezierCurve::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}

vector<SimplePoint*>BezierCurve:: GetNodes()
{
	return m_nodes;
}


void BezierCurve::Draw()
{
	BezierCurveShader* shader = dynamic_cast<BezierCurveShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(shader->getTopology());
	if (m_input->isStereoscopyActive)
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		if (m_input->isBezierPolygonActive)
		{
				m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
				b = m_vertexBufferContour.get();
				m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
				m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
				m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
				m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
				m_context->DrawIndexed(m_indexCountContour, 0, 0);
				m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
				m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
				m_context->DrawIndexed(m_indexCountContour, 0, 0);
		}
	}
	else
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		if (m_input->isBezierPolygonActive)
		{
			m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
			b = m_vertexBufferContour.get();
			m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
			m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
			m_context->DrawIndexed(m_indexCountContour, 0, 0);
		}
	}
}