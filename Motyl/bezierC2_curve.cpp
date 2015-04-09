#include "bezierC2_curve.h"

using namespace gk2;


BezierC2Curve::BezierC2Curve(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : BezierCurve(deviceContext, shader_base, device, camera, input)
{
}

BezierC2Curve::BezierC2Curve(Service& service) : BezierCurve(service)
{
	m_Type = ModelType::BezierC2Type;
}

BezierC2Curve::BezierC2Curve(const BezierCurve& other) : BezierCurve(other)
{
}


BezierC2Curve::~BezierC2Curve()
{
}

BezierC2Curve::BezierC2Curve()
{
}

void* BezierC2Curve::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierC2Curve::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BezierC2Curve::RecalculateBezierPoints()
{
	vector<SimplePoint*> virtualNodes = CalculateBezierPoints();
	for (int i = 0; i < virtualNodes.size(); i++)
	{
		m_nodes[i]->m_modelMatrix = virtualNodes[i]->m_modelMatrix;
		if (virtualNodes[i]->m_id == -1)
			delete virtualNodes[i];
	}
}

vector<SimplePoint*> BezierC2Curve::CalculateBezierPoints()
{
	SimplePoint* previous = NULL;
	vector<SimplePoint*> virtualNodes = vector<SimplePoint*>();
	for (int i = 0; i < m_deBoor.size() - 1; i++)
	{
		if (i == 0 || i == (m_deBoor.size() - 2))
		{
			//first segment
			if (i == 0)
			{
				virtualNodes.push_back(dynamic_cast<SimplePoint*>(m_deBoor[i]));
			}
			//last segment no division
			else
			{
				virtualNodes.push_back(dynamic_cast<SimplePoint*>(m_deBoor[i]));
				virtualNodes.push_back(dynamic_cast<SimplePoint*>(m_deBoor[i + 1]));
			}
		}
		else if (i == 1 || i == (m_deBoor.size() - 3))
		{
			SimplePoint* copy = SimplePoint::GetModelDilateToWeight(m_deBoor[i], m_deBoor[i + 1], 0.5f);
			if (i == 1)
			{
				virtualNodes.push_back(dynamic_cast<SimplePoint*>(m_deBoor[i]));
				virtualNodes.push_back(copy);
			}
			else
			{
				previous = virtualNodes.back();
				SimplePoint* copyFirst = SimplePoint::GetModelDilateToWeight(previous, copy, 0.5f);
				virtualNodes.push_back(copyFirst);
				virtualNodes.push_back(copy);
			}
		}
		else
		{
			//middle segments
			SimplePoint* copyFirst = SimplePoint::GetModelDilateToWeight(m_deBoor[i], m_deBoor[i + 1], 1.0f/3.0f);
			previous = virtualNodes.back();
			SimplePoint* copy = SimplePoint::GetModelDilateToWeight(previous, copyFirst, 0.5f);
			virtualNodes.push_back(copy);
			virtualNodes.push_back(copyFirst);
			SimplePoint* copySecond = SimplePoint::GetModelDilateToWeight(m_deBoor[i], m_deBoor[i + 1], 2.0f/3.0f);
			virtualNodes.push_back(copySecond);
		}

	}
	return virtualNodes;
}


void BezierC2Curve::SetNodes(vector<ModelClass*> nodes)
{
	m_deBoor = nodes;
	if (m_deBoor.size() < 5)
	{
		BezierCurve::SetNodes(m_deBoor);
		return;
	}
	vector<SimplePoint*> virtualNodes = CalculateBezierPoints();
	m_nodes = virtualNodes;
	BezierC2Curve::Reset();
}


void BezierC2Curve::UpdateNode(SimplePoint* point)
{
	if (m_segments.size() == 0)
		return;
	list<VertexPos*> vertices;
	m_vertexCountContour = m_nodes.size() * 2 - 2;
	int internalNodesCounter = 0;
	VertexPos* verticesContour = new VertexPos[m_vertexCountContour];
	RecalculateBezierPoints();
	for (int i = 0; i < m_segments.size(); i++)
	{
		if (m_segments[i]->Contain(point))
		{
			m_segments[i]->calculate(nullptr);
		}
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



void BezierC2Curve::Reset()
{
	list<VertexPos*> vertices = list<VertexPos*>();
	//we start from Second Point cause we want segments continuity C0
	int index = 0;
	int indexDeBoor = 0;
	int numberOfControlPoints = m_nodes.size() - 1;
	int segmentLength = 0;
	m_segments.clear();
	while (numberOfControlPoints > 0)
	{
		BezierC2Segment* segment = new BezierC2Segment();
		vector<SimplePoint*>* internalSegmentNodes = new vector<SimplePoint*>();
		vector<ModelClass*>* externalSegmentNodes = new vector<ModelClass*>();
		internalSegmentNodes->push_back(m_nodes[index]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 1]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 2]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 3]);
		indexDeBoor += 1;
		if (numberOfControlPoints / 3 >= 1)
		{
			//there are at least 3 points more so create Cubic Segment
			internalSegmentNodes->push_back(m_nodes[index + 1]);
			internalSegmentNodes->push_back(m_nodes[index + 2]);
			internalSegmentNodes->push_back(m_nodes[index + 3]);
			numberOfControlPoints -= 3;
			index += 3;
			segment->m_nodes = *internalSegmentNodes;
			segment->m_deBoor = *externalSegmentNodes;
			segment->calculate(&BezierSegment::calculateCubic);
		}
		else if (numberOfControlPoints == 2)
		{
			//there are only 2 point left so create Square Segment
			internalSegmentNodes->push_back(m_nodes[index + 1]);
			internalSegmentNodes->push_back(m_nodes[index + 2]);
			externalSegmentNodes->push_back(m_deBoor[index + 1]);
			externalSegmentNodes->push_back(m_deBoor[index + 2]);
			numberOfControlPoints -= 2;
			index += 2;
			segment->m_nodes = *internalSegmentNodes;
			segment->m_deBoor = *externalSegmentNodes;
			segment->calculate(&BezierSegment::calculateSquare);
		}
		else if (numberOfControlPoints == 1)
		{
			//there is only 1 point left so create Line Segment
			internalSegmentNodes->push_back(m_nodes[index + 1]);
			externalSegmentNodes->push_back(m_deBoor[index + 1]);
			numberOfControlPoints -= 1;
			index += 1;
			segment->m_nodes = *internalSegmentNodes;
			segment->m_deBoor = *externalSegmentNodes;
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
	for (list<VertexPos*>::iterator it = vertices.begin(); it != vertices.end(); it++)
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