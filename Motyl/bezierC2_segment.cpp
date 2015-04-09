#include "bezierC2_segment.h"


BezierC2Segment::BezierC2Segment()
{
}


BezierC2Segment::~BezierC2Segment()
{
}

BezierC2Segment::BezierC2Segment(const BezierC2Segment&)
{
}

void* BezierC2Segment::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierC2Segment::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

bool BezierC2Segment::Contain(SimplePoint* point)
{
	for (int i = 0; i < m_deBoor.size(); i++)
		if (point->m_id == m_deBoor[i]->m_id)
			return true;
	return false;
}