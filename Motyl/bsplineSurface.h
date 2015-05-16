#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bsplinePatch.h"
#include <map>

class BSplineSurface : public ModelClass{
public:
	BSplineSurface();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	BSplineSurface(const BSplineSurface&);
	~BSplineSurface();

	virtual void Initialize();
	virtual void Draw();
	virtual void UpdateNode(SimplePoint*);
	virtual void setTriangleTopology();
	virtual void setLineTopology();
	void AddPatch(BSplinePatch* patch);
	void TranslateSurfacePoints();
	void TranslateCyllinderPoints();
	void SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth);
private:
	vector<BSplinePatch*> m_bsplinePatches;
	map<int, SimplePoint*> m_nodes;
	double m_sizeX = 0.2, m_sizeY = 0.4;
	int m_rows, m_cols;
};