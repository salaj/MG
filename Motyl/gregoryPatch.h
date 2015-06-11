#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezier_segment.h"
#include "simple_point .h"
#include "shader_gregory_patch.h"

class GregoryPatch : public ModelClass{
public:
	GregoryPatch();
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
	GregoryPatch(const GregoryPatch&);
	GregoryPatch(Service& servie);
	~GregoryPatch();

	virtual void SetNodes(vector<ModelClass*>& nodes);
	virtual void Initialize();
	virtual void Draw();
	virtual void Reset();
	//POLYGON Contour
	std::shared_ptr<ID3D11Buffer> m_vertexBufferContour, m_indexBufferContour;
	int m_vertexCountContour, m_indexCountContour;
	int verticalSpaces, horizontalSpaces;
	vector<SimplePoint*> GetNodes();

	//first row
	SimplePoint* p0;
	SimplePoint* e0plus;
	SimplePoint* e1minus;
	SimplePoint* p1;

	//second row
	SimplePoint* e0minus;
	SimplePoint* f0minus;
	SimplePoint* f0plus;
	SimplePoint* f1minus;
	SimplePoint* f1plus;
	SimplePoint* e1plus;

	//third row
	SimplePoint* e3plus;
	SimplePoint* f3plus;
	SimplePoint* f3minus;
	SimplePoint* f2plus;
	SimplePoint* f2minus;
	SimplePoint* e2minus;

	//fourth row
	SimplePoint* p3;
	SimplePoint* e3minus;
	SimplePoint* e2plus;
	SimplePoint* p2;

private:

	vector<SimplePoint*> m_nodes;
	void setPointTopology();
	virtual void setTriangleTopology();
	virtual void setLineTopology();

	void createDeBoorPointMatrix();
	XMMATRIX createBernteinBaseVector(float val, bool transpose);
	void generateLines(list<VertexPos*>&);
	void generateBezierNet(VertexPos*);

	XMMATRIX matrixOfX;
	XMMATRIX matrixOfY;
	XMMATRIX matrixOfZ;

	float b_u[4];
	float b_v[4];




};