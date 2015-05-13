#include "bezierPatch.h"


BezierPatch::BezierPatch()
{
}


BezierPatch::~BezierPatch()
{
}

BezierPatch::BezierPatch(const BezierPatch&)
{
}

BezierPatch::BezierPatch(Service& service) : ModelClass(service)
{
	m_Type = ModelType::BezierPatchType;
	ModelClass::Initialize();
}


void* BezierPatch::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierPatch::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BezierPatch::Initialize()
{
	verticalSpaces = horizontalSpaces = 4;
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
}
vector<SimplePoint*> BezierPatch:: GetNodes()
{
	return m_nodes;
}

void BezierPatch::SetNodes(vector<ModelClass*>& nodes)
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

XMMATRIX BezierPatch::createBernteinBaseVector(float val, bool transpose)
{
	XMVECTOR B;
	float t = val;
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	XMMATRIX matrix;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix.m[i][j] = 0.0f;
	if (transpose)
	{
		matrix.m[0][0] = uuu;
		matrix.m[1][0] = 3 * t * uu;
		matrix.m[2][0] = 3 * tt * u;
		matrix.m[3][0] = ttt;
	}
	else
	{
		matrix.m[0][0] = uuu;
		matrix.m[0][1] = 3 * t * uu;
		matrix.m[0][2] = 3 * tt * u;
		matrix.m[0][3] = ttt;
	}
	return matrix;
}

void BezierPatch::createBezierPointMatrix()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			matrixOfX.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().x;
			matrixOfY.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().y;
			matrixOfZ.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().z;
		}
}

void BezierPatch::generateLines(list<VertexPos*>& vertices)
{
	double numberOfDots = 300;

	vector<int> bezier_Indices;

	//creates vertical lines
	for (int m = 0; m < horizontalSpaces; m++)
	{
		bezier_Indices.push_back(m + 0 * horizontalSpaces);
		bezier_Indices.push_back(m + 1 * horizontalSpaces);
		bezier_Indices.push_back(m + 2 * horizontalSpaces);
		bezier_Indices.push_back(m + 3 * horizontalSpaces);
		//numberOfDots = bezier_length(bezier_Indices);
		bezier_Indices.clear();
		XMMATRIX B_u = createBernteinBaseVector((float)m / (float)(horizontalSpaces - 1), false);
		for (int i = 0; i < numberOfDots; i++)
		{
			XMMATRIX B_v = createBernteinBaseVector((float)i / (float)numberOfDots, true);
			resultX = B_u * matrixOfX * B_v;
			resultY = B_u * matrixOfY * B_v;
			resultZ = B_u * matrixOfZ * B_v;
			vertices.push_back(new VertexPos{
				XMFLOAT3(
				resultX.m[0][0],
				resultY.m[0][0],
				resultZ.m[0][0]
				) });
		}
	}
	//creates horizontal lines
	for (int m = 0; m < verticalSpaces; m++)
	{
		bezier_Indices.push_back(m * horizontalSpaces + 0);
		bezier_Indices.push_back(m * horizontalSpaces + 1);
		bezier_Indices.push_back(m * horizontalSpaces + 2);
		bezier_Indices.push_back(m * horizontalSpaces + 3);
		//numberOfDots = bezier_length(bezier_Indices);
		bezier_Indices.clear();
		XMMATRIX B_v = createBernteinBaseVector((float)m / (float)(verticalSpaces - 1), true);
		for (int i = 0; i < numberOfDots; i++)
		{
			XMMATRIX B_u = createBernteinBaseVector((float)i / (float)numberOfDots, false);
			resultX = B_u * matrixOfX * B_v;
			resultY = B_u * matrixOfY * B_v;
			resultZ = B_u * matrixOfZ * B_v;
			vertices.push_back(new VertexPos{
				XMFLOAT3(
				resultX.m[0][0],
				resultY.m[0][0],
				resultZ.m[0][0]
				) });
		}
	}
}

void BezierPatch::generateBezierNet(VertexPos* verticesContour)
{
	int internalNodesCounter = 0;
	for (int i = 0; i < m_nodes.size(); i += 4)
	{
		verticesContour[internalNodesCounter++] = { m_nodes[i]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 3]->GetPosition3() };
	}
	for (int i = 0; i < 4; i++)
	{
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 0]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 3]->GetPosition3() };
	}
}


void BezierPatch::Reset()
{
	createBezierPointMatrix();
	list<VertexPos*> vertices = list<VertexPos*>();
	//we start from Second Point cause we want segments continuity C0
	int index = 0;
	int numberOfControlPoints = m_nodes.size() - 1;
	int segmentLength = 0;
	generateLines(vertices);

	m_vertexCountContour = (4 + 4 - 2) * 4 * 2; //48
	VertexPos* verticesContour;
	verticesContour = new VertexPos[m_vertexCountContour];
	generateBezierNet(verticesContour);

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

//double BezierPatch::bezier_length(XMFLOAT3* bezierNodes)
//{
//	double t;
//	int i;
//
//	float sumAllSegments = 0;
//	double screenRes = 300;
//	for (int i = 1; i < 4; i++)
//	{
//	XMFLOAT3 pos1 = bezierNodes[0]->GetPosition3();
//	XMFLOAT3 pos2 = m_nodes[bezierIndices[i]]->GetPosition3();
//	float singleSegment = (pos1.x - pos2.x) *  (pos1.x - pos2.x) +
//	(pos1.y - pos2.y) *  (pos1.y - pos2.y) +
//	(pos1.z - pos2.z) *  (pos1.z - pos2.z);
//	sumAllSegments += sqrtf(singleSegment);
//	}
//	XMFLOAT3 pos1 = m_nodes[bezierIndices[3]]->GetPosition3();
//	XMFLOAT3 pos2 = m_nodes[bezierIndices[0]]->GetPosition3();
//	float singleSegment = (pos1.x - pos2.x) *  (pos1.x - pos2.x) +
//	(pos1.y - pos2.y) *  (pos1.y - pos2.y) +
//	(pos1.z - pos2.z) *  (pos1.z - pos2.z);
//	sumAllSegments += sqrtf(singleSegment);
//
//	return sumAllSegments * screenRes;
//}


void BezierPatch::setPointTopology()
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

void BezierPatch::setTriangleTopology()
{

}
void BezierPatch::setLineTopology()
{

}

void BezierPatch::Draw()
{
	BezierPatchShader* shader = dynamic_cast<BezierPatchShader*>(m_shader_base);
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