#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <list>
#include "bezier_curve.h"
#include "bezierC2_segment.h"
#include <cfloat>


////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////

enum CurveBase{
	BSpline,
	Bezier
};
class BezierC2Curve : public BezierCurve
{
public:
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
	virtual void Draw();
	vector<BezierC2Segment*> m_segments;
	XMFLOAT3 calculateInBSplineBase(float t);
	vector<SimplePoint*> m_Bezier;
	static CurveBase m_base;
private:
	vector<ModelClass*> m_deBoor;
	vector<SimplePoint*> CalculateBezierPoints();
	void RecalculateBezierPoints();
	double bezier_length();
	void convertFromBSplineToBezbierBase();
	void convertFromBezierToBSplineBase();
	double* T;
	int index;

	void drawInBernsteinBase(bool updateBezierPoints = true);
	void drawInBSplineBase();
	//works in both bases
	VertexPos* drawContour();
};