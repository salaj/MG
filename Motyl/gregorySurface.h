#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "gregoryPatch.h"
#include "bezierSurface.h"
#include <map>


enum TranslatedBezier{
	left,
	right,
	bottom
};

class GregorySurface : public ModelClass{
public:
	GregorySurface();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	//BezierSegment(XMVECTOR A, XMVECTOR B, XMVECTOR C, XMVECTOR D);
	GregorySurface(const GregorySurface&);
	~GregorySurface();

	virtual void Initialize();
	virtual void Draw();
	virtual void Reset();
	virtual void UpdateNode(SimplePoint*);
	virtual void setTriangleTopology();
	virtual void setLineTopology();
	void AddPatch(GregoryPatch* patch);
	void TranslatePoints(TranslatedBezier direction);
	virtual void Translate(SimplePoint* p);
	void SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth);
	void SetBezierSurfaces(vector<BezierSurface*> bezierSurfaces);
	vector<SimplePoint*> GetNodes();
	bool isCyllindrical;
	double m_sizeX = 0.2, m_sizeY = 0.4;
	int m_rows = 4, m_cols = 4;
private:
	vector<BezierSurface*> m_bezierSurfaces;
	vector<GregoryPatch*> m_gregoryPatches;
	map<int, SimplePoint*> m_nodes;


	TranslatedBezier checkTranslatedPoint(SimplePoint* p);
};