#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "bezierC2_curve.h"

class InterpolatedC2Curve : public BezierC2Curve
{
public:
	InterpolatedC2Curve();
	InterpolatedC2Curve(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera camera,
		InputClass* input);
	InterpolatedC2Curve(const BezierCurve&);
	InterpolatedC2Curve(Service& servie);
	~InterpolatedC2Curve();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Draw();
	virtual void SetNodes(vector<ModelClass*> nodes);
	virtual void UpdateNode(SimplePoint*);

private:
	void createBaseFunctionsMatrix();
	vector<ModelClass*> m_interpolants;
	void dividePoint(int index, float dividor, float** matrix);
	void substractPoint(int indexFirst, int indexSecond, float factor);
	void reductSingleElementUp(int i, int j, float** matrix);
	void reductSingleElementDown(int i, int j, float** matrix, int rows, int cols, int n, float leadingValue);
};