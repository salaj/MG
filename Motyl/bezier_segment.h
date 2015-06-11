#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"
#include <list>
#include "simple_point .h"

class BezierSegment{
public:
	typedef XMFLOAT3(BezierSegment::*segment_length_calculator)(float);
	BezierSegment();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	//BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D);
	BezierSegment(const BezierSegment&);
	~BezierSegment();
	//bernstein polygon vertices
	XMVECTOR A;
	XMVECTOR B;
	XMVECTOR C;
	XMVECTOR D;
	XMVECTOR Q(float t);
	void calculate(segment_length_calculator);
	virtual bool Contain(SimplePoint* point);
	double bezier_length(segment_length_calculator);

	XMFLOAT3 calculateCubic(float t);
	XMFLOAT3 calculateSquare(float t);
	XMFLOAT3 calculateLine(float t);

	list<VertexPos*> GetSegmentPoints();
	vector<SimplePoint*> m_nodes;
private:
	list<VertexPos*> vertices;
	segment_length_calculator m_previousCalculator;
	int m_BSplineFunctionIndex = 3;
	void fillNodes();
};