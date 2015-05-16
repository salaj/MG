#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"
#include "simple_point .h"
#include "shader_bezier_curve.h"
#include <list>
#include "bezier_segment.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////
class BezierCurve : public ModelClass
{

private:

	static const float R_SMALL_TORUS;
	static const float R_BIG_TORUS;
	static int N_SEGMENTS_NUMBER;
	static int N_CIRCLE_SEGMENTS_NUMBER;

public:
	//typedef XMFLOAT3(BezierCurve::*segment_length_calculator)(float);
	BezierCurve();
	BezierCurve(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera& camera,
		InputClass* input);
	BezierCurve(const BezierCurve&);
	BezierCurve(Service& servie);
	~BezierCurve();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	static int CurveCounter;

	virtual void Initialize();
	virtual void Draw();

	void setStereoscopy(bool);

	BezierCurve& operator=(const BezierCurve& element);
	void SetNodes(vector<ModelClass*>& nodes);
	//deprecated
	void InsertNodeAt(SimplePoint*, int position);
	void InsertNodeAfter(SimplePoint*, SimplePoint*);
	void RemoveNodeAt(int position);
	void RemoveNode(SimplePoint*);
	/////////////
	virtual void UpdateNode(SimplePoint*);

	virtual void Reset();

	//POLYGON Contour
	std::shared_ptr<ID3D11Buffer> m_vertexBufferContour, m_indexBufferContour;
	int m_vertexCountContour, m_indexCountContour;
protected:
	
	////bernstein polygon vertices
	XMVECTOR A;
	XMVECTOR B;
	XMVECTOR C;
	XMVECTOR D;

	//double bezier_length(segment_length_calculator);
	vector<SimplePoint*> m_nodes;
	vector<BezierSegment*> m_segments;

	virtual void setTriangleTopology();
	virtual void setLineTopology();
	void setPointTopology();

	bool m_isStereoscopic = true;
};