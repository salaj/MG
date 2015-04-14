#include "bezier_segment.h"


BezierSegment::BezierSegment()
{
}

//BezierSegment::BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D)
//{
//	this->A = A;
//	this->B = B;
//	this->C = C;
//	this->D = D;
//}


BezierSegment::~BezierSegment()
{
}

BezierSegment::BezierSegment(const BezierSegment&)
{
}

void* BezierSegment::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierSegment::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

list<VertexPos*> BezierSegment::GetSegmentPoints()
{
	return vertices;
}

void BezierSegment::calculate(segment_length_calculator calculator)
{
	vertices.clear();
	if (calculator != nullptr)
		m_previousCalculator = calculator;
	else
		calculator = m_previousCalculator;
	fillNodes();
	int segmentLength = bezier_length(calculator);
	for (int i = 0; i < segmentLength; i++)
	{
		float t = (float)i / (float)(segmentLength - 1);
		vertices.push_back(new VertexPos{
			(this->*calculator)(t)
		});
	}
}

void BezierSegment::fillNodes()
{
	int numberOfInternalNodes = m_nodes.size();
	if (numberOfInternalNodes > 0)
		A = XMLoadFloat4(&m_nodes[0]->GetPosition());
	if (numberOfInternalNodes > 1)
		B = XMLoadFloat4(&m_nodes[1]->GetPosition());
	if (numberOfInternalNodes > 2)
		C = XMLoadFloat4(&m_nodes[2]->GetPosition());
	if (numberOfInternalNodes > 3)
		D = XMLoadFloat4(&m_nodes[3]->GetPosition());
}

bool BezierSegment::Contain(SimplePoint* point)
{
	for (int i = 0; i < m_nodes.size(); i++)
		if (point->m_id == m_nodes[i]->m_id)
			return true;
	return false;
}

double BezierSegment::bezier_length(segment_length_calculator calculator)
{
	double t;
	int i;
	int steps = 10;
	int estimation = 2;
	int screenWidth = 300;
	XMFLOAT3 dot;
	XMFLOAT3 previous_dot;
	double length = 0.0;
	for (i = 0; i <= steps; i++) {
		t = (double)i / (double)steps;
		dot = (this->*calculator)(t);
		if (i > 0) {
			double x_diff = dot.x - previous_dot.x;
			double y_diff = dot.y - previous_dot.y;
			length += sqrt(x_diff * x_diff + y_diff * y_diff);
		}
		previous_dot = dot;
	}
	return length * screenWidth * estimation;
}

XMFLOAT3 BezierSegment::calculateCubic(float t)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	XMVECTOR result = uuu * A; //first term in equation
	result += 3 * uu * t * B;  //second term in equation
	result += 3 * u * tt * C; //third term in equation
	result += ttt * D;

	return XMFLOAT3(
		XMVectorGetX(result),
		XMVectorGetY(result),
		XMVectorGetZ(result)
		);
}

XMFLOAT3 BezierSegment::calculateSquare(float t)
{
	float u = 1 - t;
	float uu = u * u;
	float tt = t * t;

	XMVECTOR result = uu * A;  //first term in equation
	result += 2 * u * t * B; //second term in equation
	result += tt * C; //third term in equation

	return XMFLOAT3(
		XMVectorGetX(result),
		XMVectorGetY(result),
		XMVectorGetZ(result)
		);
}


XMFLOAT3 BezierSegment::calculateLine(float t)
{
	float u = 1 - t;

	XMVECTOR result = u * A;  //first term in equation
	result += t * B; //second term in equation

	return XMFLOAT3(
		XMVectorGetX(result),
		XMVectorGetY(result),
		XMVectorGetZ(result)
		);
}
