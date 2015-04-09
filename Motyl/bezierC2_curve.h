#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <list>
#include "bezier_curve.h"
#include "bezierC2_segment.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////
class BezierC2Curve : public BezierCurve
{

private:

	static const float R_SMALL_TORUS;
	static const float R_BIG_TORUS;
	static int N_SEGMENTS_NUMBER;
	static int N_CIRCLE_SEGMENTS_NUMBER;

public:
	//typedef XMFLOAT3(BezierCurve::*segment_length_calculator)(float);
	BezierC2Curve();
	BezierC2Curve(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera camera,
		InputClass* input);
	BezierC2Curve(const BezierCurve&);
	BezierC2Curve(Service& servie);
	~BezierC2Curve();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void SetNodes(vector<ModelClass*> nodes);
	virtual void Reset();
	virtual void UpdateNode(SimplePoint*);
	vector<BezierC2Segment*> m_segments;
private:
	vector<ModelClass*> m_deBoor;
	vector<SimplePoint*> CalculateBezierPoints();
	void RecalculateBezierPoints();
};