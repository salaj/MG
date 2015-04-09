#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezier_segment.h"

class BezierC2Segment : public BezierSegment{
public:
	typedef XMFLOAT3(BezierSegment::*segment_length_calculator)(float);
	BezierC2Segment();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	//BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D);
	BezierC2Segment(const BezierC2Segment&);
	~BezierC2Segment();
	virtual bool Contain(SimplePoint* point);
	vector<ModelClass*> m_deBoor;
private:
};