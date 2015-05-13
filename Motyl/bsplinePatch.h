#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezier_segment.h"
#include "simple_point .h"
#include "shader_bspline_patch.h"
#include "bezierC2_curve.h"

class BSplinePatch : public ModelClass{
public:
	BSplinePatch();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	BSplinePatch(const BSplinePatch&);
	BSplinePatch(Service& servie);
	~BSplinePatch();

	virtual void SetNodes(vector<ModelClass*>& nodes);
	virtual void Initialize();
	virtual void Draw();
	virtual void Reset();
	//POLYGON Contour
	std::shared_ptr<ID3D11Buffer> m_vertexBufferContour, m_indexBufferContour;
	int m_vertexCountContour, m_indexCountContour;
	int verticalSpaces, horizontalSpaces;
	vector<SimplePoint*> GetNodes();
private:

	vector<SimplePoint*> m_nodes;
	void setPointTopology();
	virtual void setTriangleTopology();
	virtual void setLineTopology();

	void createDeBoorPointMatrix();
	XMMATRIX createBernteinBaseVector(float val, bool transpose);
	void generateLines(list<VertexPos*>&);
	void generateBezierNet(VertexPos*);
	//double bezier_length(SimplePoint* bezierNodes);
	float calculateSingleDeBoor(int n, int i, double t, double* knots, int maxI);

	XMMATRIX matrixOfX;
	XMMATRIX matrixOfY;
	XMMATRIX matrixOfZ;
};