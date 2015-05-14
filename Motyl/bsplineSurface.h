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
	void SetDimensions(int rows, int cols, int surfaceWidth, int surfaceHeigth);
private:
	vector<BSplinePatch*> m_bsplinePatches;
	map<int, SimplePoint*> m_nodes;
	int m_sizeX = 20, m_sizeY = 40;
	int m_rows, m_cols;
};