#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezier_segment.h"
#include "simple_point .h"
#include "shader_bezier_patch.h"

class BezierPatch : public ModelClass{
public:
	BezierPatch();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	BezierPatch(const BezierPatch&);
	BezierPatch(Service& servie);
	~BezierPatch();

	virtual void SetNodes(vector<ModelClass*>& nodes);
	virtual void Initialize();
	virtual void Draw();
	virtual void Reset();
	//virtual void UpdateNode(SimplePoint*);
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

	void createBezierPointMatrix();
	XMMATRIX createBernteinBaseVector(float val, bool transpose);
	void generateLines(list<VertexPos*>&);
	void generateBezierNet(VertexPos*);
	//double bezier_length(SimplePoint* bezierNodes);

	XMMATRIX matrixOfX, resultX;
	XMMATRIX matrixOfY, resultY;
	XMMATRIX matrixOfZ, resultZ;
};