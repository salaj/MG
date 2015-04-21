#include "bezierC2_curve.h"

using namespace gk2;

CurveBase BezierC2Curve::m_base = CurveBase::Bezier;

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

void BezierC2Curve::Draw()
{
	if (m_base == CurveBase::Bezier)
	{
		//first draw all virtual bezier points
		for (int i = 0; i < m_Bezier.size(); i++)
		{
			m_Bezier[i]->Initialize();
			m_Bezier[i]->Draw();
		}
	}
	//then rest
	BezierCurve::Draw();
}

void BezierC2Curve::SetNodes(vector<ModelClass*> nodes)
{
	m_deBoor = nodes;
	BezierC2Curve::Reset();
	for (int i = 0; i < m_Bezier.size(); i++)
		delete m_Bezier[i];
	m_Bezier.clear();
	UpdateNode(dynamic_cast<SimplePoint*>(nodes[0]));
}


void BezierC2Curve::UpdateNode(SimplePoint* point)
{
	if (m_base == CurveBase::BSpline)
	{
		drawInBSplineBase();
		return;
	}
	else if (m_base == CurveBase::Bezier)
	{
		//if moved point is virtual bezier point <=> it is copy
		if (!point->m_isGenuine)
		{
			for (int i = 0; i < m_segments.size(); i++)
			{
				if (m_segments[i]->Contain(point))
				{
					convertFromBezierToBSplineBase(*m_segments[i]);
				}
			}
		}
		drawInBernsteinBase();
		return;
	}
}

void BezierC2Curve::drawInBSplineBase()
{
	m_vertexCount = bezier_length();
	if (m_vertexCount == 0)
		m_vertexCount = 1;
	int n = 3;
	int m = m_deBoor.size();
	list<VertexPos*>vertices;
	VertexPos* verticesContour;
	for (int i = 0; i < m_vertexCount; i++)
	{
		float t = (float)(i * (m + 1)) / (float)(m_vertexCount - 1);
		if (t >= 2.0f && t <= m - 1  )
		{
			XMFLOAT3 pos = calculateInBSplineBase(t);
			vertices.push_back(new VertexPos{
				pos
			});
		}
	}

	verticesContour = drawContour();

	if (vertices.size() == 0)
	{
		//there must be only one point, create fake point to draw, but not segment
		vertices.push_back(new VertexPos{ m_deBoor[0]->GetPosition3() });
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
	delete[] arr;
	setPointTopology();
}

VertexPos* BezierC2Curve::drawContour()
{
	int segmentCount = m_segments.size();
	int internalNodesCounter = 0;
	m_vertexCountContour = m_deBoor.size() * 2 - 2;
	VertexPos* verticesContour;
	if (m_vertexCountContour > 0)
		verticesContour = new VertexPos[m_vertexCountContour];
	else
		verticesContour = new VertexPos[1];

	for (int i = 0; i < m_deBoor.size(); i++)
	{
		verticesContour[internalNodesCounter++] = VertexPos{ m_deBoor[i]->GetPosition3() };
		if (i > 0 && i <  m_deBoor.size() - 1)
		{
			verticesContour[internalNodesCounter] = verticesContour[internalNodesCounter - 1];
			internalNodesCounter++;
		}
	}
	return verticesContour;
}

void BezierC2Curve::drawInBernsteinBase()
{
	convertFromBSplineToBezbierBase();
	//now we have bezier points, so create segments and draw
	m_segments.clear();
	list<VertexPos*> vertices;
	VertexPos* verticesContour;
	int indexDeBoor = 0;
	int m = m_deBoor.size();
	for (int i = 0; i < m_Bezier.size(); i += 4)
	{
		BezierC2Segment* segment = new BezierC2Segment();
		vector<SimplePoint*>* internalSegmentNodes = new vector<SimplePoint*>();
		internalSegmentNodes->push_back(m_Bezier[i]);
		internalSegmentNodes->push_back(m_Bezier[i + 1]);
		internalSegmentNodes->push_back(m_Bezier[i + 2]);
		internalSegmentNodes->push_back(m_Bezier[i + 3]);
		segment->m_nodes = *internalSegmentNodes;
		vector<ModelClass*>* externalSegmentNodes = new vector<ModelClass*>();
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 1]);
		externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 2]);
		if (m > 3)
			externalSegmentNodes->push_back(m_deBoor[indexDeBoor + 3]);
		indexDeBoor += 1;
		segment->m_deBoor = *externalSegmentNodes;
		segment->calculate(&BezierSegment::calculateCubic);
		m_segments.push_back(segment);
	}

	int segmentCount = m_segments.size();

	for (int i = 0; i < segmentCount; i++)
	{
		list<VertexPos*> singleSegmentPoints = m_segments[i]->GetSegmentPoints();
		//concatenate segment points with target vertices
		vertices.splice(vertices.end(), singleSegmentPoints);
	}

	verticesContour = drawContour();

	if (vertices.size() == 0)
	{
		//there must be only one point, create fake point to draw, but not segment
		vertices.push_back(new VertexPos{ m_deBoor[0]->GetPosition3()});
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
	delete[] arr;
	delete[] verticesContour;
	setPointTopology();
}


void BezierC2Curve::Reset()
{
	if (BezierC2Curve::m_base == CurveBase::BSpline)
		drawInBSplineBase();
	else if (BezierC2Curve::m_base == CurveBase::Bezier)
		drawInBernsteinBase();
}

void BezierC2Curve::convertFromBezierToBSplineBase(BezierC2Segment& segment)
{
	XMVECTOR D_2, D_1, D0, D1,
		A, B, C, D;

	int m = segment.m_deBoor.size();

		A = XMLoadFloat4(&segment.m_nodes[0]->GetPosition());
		B = XMLoadFloat4(&segment.m_nodes[1]->GetPosition());
		C = XMLoadFloat4(&segment.m_nodes[2]->GetPosition());
		D = XMLoadFloat4(&segment.m_nodes[3]->GetPosition());


		D_2 = 6 * A + 2 * C - 7 * B;
		D_1 = 2 * B - C;
		D0 = 2 * C - B;
		D1 = 6 * D + 2 * B - 7 * C;

		segment.m_deBoor[0]->SetPosition(D_2);
		segment.m_deBoor[1]->SetPosition(D_1);
		segment.m_deBoor[2]->SetPosition(D0);
		if (m > 3)
			segment.m_deBoor[3]->SetPosition(D1);
}

void BezierC2Curve::convertFromBSplineToBezbierBase()
{
	XMVECTOR B0, B1, B2, B3,
		A, B, C, D;
	SimplePoint* origin = dynamic_cast<SimplePoint*>(m_deBoor[0]);
	SimplePoint* copy;
	int m = m_deBoor.size();
	int n = m > 4 ? 4 : m;

	///////////POSSIBLE MODIFICATION///////////
	if (m_deBoor.size() < 3)
		return;
	////////////////////////

	for (int i = 0; i <= m_deBoor.size() - n; i++)
	{
		A = XMLoadFloat4(&m_deBoor[i]->GetPosition());
		B = XMLoadFloat4(&m_deBoor[i + 1]->GetPosition());
		C = XMLoadFloat4(&m_deBoor[i + 2]->GetPosition());
		if (m <= i + 3)
			D = XMVectorZero();
		else
			D = XMLoadFloat4(&m_deBoor[i + 3]->GetPosition());
		
		if (i == 0)
			B0 = (A + 4 * B + C) / 6;
		else
			B0 = B3;
		if (m_Bezier.size() <= 4 * i)
		{
			copy = new SimplePoint(*origin);
			m_Bezier.push_back(copy);
		}
		else
			copy = m_Bezier[4 * i];
		copy->SetPosition(B0);

		B1 = (4 * B + 2 * C) / 6;
		if (m_Bezier.size() <= 4 * i + 1)
		{
			copy = new SimplePoint(*origin);
			m_Bezier.push_back(copy);
		}
		else
			copy = m_Bezier[4 * i + 1];
		copy->SetPosition(B1);
		B2 = (2 * B + 4 * C) / 6;
		if (m_Bezier.size() <= 4 * i + 2)
		{
			copy = new SimplePoint(*origin);
			m_Bezier.push_back(copy);
		}
		else
			copy = m_Bezier[4 * i + 2];
		copy->SetPosition(B2);
		B3 = (B + 4 * C + D) / 6;
		if (m_Bezier.size() <= 4 * i + 3)
		{
			copy = new SimplePoint(*origin);
			m_Bezier.push_back(copy);
		}
		else
			copy = m_Bezier[4 * i + 3];
		copy->SetPosition(B3);
	}

}

class Point
{
public:
	Point(){ x = 0.; y = 0.; z = 0.; };
	// copy operator
	Point operator=(const Point pt);
	Point operator+(const Point pt) const;
	//Point operator-(const Point pt) const;
	Point operator*(double m) const;
	Point operator/(double m) const;
	double x, y, z;
};

Point Point::operator=(const Point pt)
{
	x = pt.x;
	y = pt.y;
	z = pt.z;
	return *this;
}
Point Point::operator+(const Point pt) const
{
	Point temp;
	temp.x = x + pt.x;
	temp.y = y + pt.y;
	temp.z = z + pt.z;
	return temp;
}
Point Point::operator*(double m) const
{
	Point temp;
	temp.x = x*m;
	temp.y = y*m;
	temp.z = z*m;
	return temp;
}
Point Point::operator/(double m) const
{
	Point temp;
	temp.x = x / m;
	temp.y = y / m;
	temp.z = z / m;
	return temp;
}

Point deBoor(int k, int degree, int i, double x, double* knots, Point *ctrlPoints, int maxI)
{ 
	if (k == 0)
	{
		if (i < 0 || i >= maxI)
			return Point();
		return ctrlPoints[i];
	}
	else
	{
		double alpha = (x - knots[i]) / (knots[i + degree + 1 - k] - knots[i]);
		return (deBoor(k - 1, degree, i - 1, x, knots, ctrlPoints, maxI)*(1.0f - alpha) + deBoor(k - 1, degree, i, x, knots, ctrlPoints,maxI)*alpha);
	}
}

XMFLOAT3 BezierC2Curve::calculateInBSplineBase(float t)
{
	
	int n = 3;
	int m = m_deBoor.size();
	double* T = new double[m + n + 1];
	T[0] = 0.0f;
	for (int i = 0; i <= m + 1; i++)
		T[i + 1] = (double)i;
	T[m + n] = (double)(m + 1);
	//double T[8] = { 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 5.0f};
	Point* controlPoints = new Point[m];
	for (int i = 0; i < m; i++)
	{
		controlPoints[i].x = m_deBoor[i]->GetPosition().x;
		controlPoints[i].y = m_deBoor[i]->GetPosition().y;
		controlPoints[i].z = m_deBoor[i]->GetPosition().z;
	}

	Point resN3i;
	for (int i = 0; i < m + n; i++)
	{
		if (t >= T[i] && t < T[i + 1])
		{
			resN3i = deBoor(n, n, i, t, T, controlPoints, m);
			break;
		}
	}
	return XMFLOAT3(
		resN3i.x,
		resN3i.y,
		resN3i.z
		);
}

double BezierC2Curve::bezier_length()
{
	double t;
	int i;
	int m = m_deBoor.size();
	int steps = 10;
	int estimation = 2;
	int screenWidth = 300;
	XMFLOAT3 dot;
	XMFLOAT3 previous_dot;
	double length = 0.0;
	for (i = 0; i <= steps; i++) {
		t = 2.0f + (double)(i * m - 3) / (double)steps;
		dot = calculateInBSplineBase(t);
		if (i > 0) {
			double x_diff = dot.x - previous_dot.x;
			double y_diff = dot.y - previous_dot.y;
			length += sqrt(x_diff * x_diff + y_diff * y_diff);
		}
		previous_dot = dot;
	}
	return length * screenWidth * estimation;
}
