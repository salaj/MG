#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezierPatch.h"
#include <map>

class BezierSurface : public ModelClass{
public:
	BezierSurface();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	//BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D);
	BezierSurface(const BezierSurface&);
	~BezierSurface();

	virtual void Initialize();
	virtual void Draw();
	virtual void UpdateNode(SimplePoint*);
	virtual void setTriangleTopology();
	virtual void setLineTopology();
	void AddPatch(BezierPatch* patch);
	void TranslateSurfacePoints();
	void TranslateCyllinderPoints();
	void SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth);
private:
	vector<BezierPatch*> m_bezierPatches;
	map<int, SimplePoint*> m_nodes;
	double m_sizeX = 0.2, m_sizeY = 0.4;
	int m_rows, m_cols;
};