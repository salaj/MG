#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////
class SimplePoint : public ModelClass
{

private:

	static const float R_SMALL_TORUS;
	static const float R_BIG_TORUS;
	static int N_SEGMENTS_NUMBER;
	static int N_CIRCLE_SEGMENTS_NUMBER;

public:
	SimplePoint();
	SimplePoint(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera camera,
		InputClass* input);
	SimplePoint(const SimplePoint&);
	SimplePoint(Service& servie);
	~SimplePoint();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw();

	void setStereoscopy(bool);

	SimplePoint& operator=(const SimplePoint& element);
	static SimplePoint* GetModelDilateToWeight(ModelClass* a, ModelClass* b, float weight);
private:


	virtual void setTriangleTopology();
	virtual void setLineTopology();

	bool m_isStereoscopic = true;
};