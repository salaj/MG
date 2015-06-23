#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezierSurface.h"
#include "shader_intersection_surface.h"
#include "shader_intersection_square.h"
#include "line.h"
#include <float.h>
#include <map>

class IntersectionSurface : public ModelClass{
public:
	IntersectionSurface();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	//BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D);
	IntersectionSurface(const IntersectionSurface&);
	IntersectionSurface(Service& servie);
	~IntersectionSurface();

	virtual void Initialize();
	virtual void Draw();
	virtual void Reset();
	virtual void UpdateNode(SimplePoint*);
	virtual void setTriangleTopology();
	virtual void setLineTopology();
	void setPointTopology();
	void AddSurface(BezierSurface* surface);
	void NewtonLineSurface();
	XMVECTOR NewtonSurfaceSurface(float u, float v, float s, float t, bool& isDivergent);
	void FindIntersection(float u_origin = 0.5f, float v_origin = 0.5f, float s_origin = 0.5f, float t_origin = 0.5f);
	void SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth);
	vector<SimplePoint*> GetNodes();
	bool isCyllindrical;
	double m_sizeX = 0.2, m_sizeY = 0.4;
	int m_rows = 4, m_cols = 4;
	int firstCollapse = -1, secondCollapse= -1;
	Line* P;
	void SetOrigin(float u, float v, float s, float t);
	void FindFirstPoint(float& u, float&v, float&s, float&t, XMFLOAT4 startPos);
	IntersectionSquareShader* shaderSquare;
	vector<BezierSurface*> GetSurfaces();
	static float precision;
private:
	vector<BezierSurface*> m_bezierSurface;
	map<int, SimplePoint*> m_nodes;
	float u_origin, v_origin, s_origin, t_origin;

	void FindNewNewtonStartPoint(float&u, float& v, XMVECTOR target, BezierSurface* surface, float factor);

	void calculateDerivative(BezierSurface* G, BezierSurface* H, list<VertexPos*>& vertices, vector<float>(&vertices_parametrization)[4], float& u, float& v, float& s, float& t, bool isNegativeDerivative);

};