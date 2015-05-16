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
class Torus : public ModelClass
{

private:

	static const float R_SMALL_TORUS;
	static const float R_BIG_TORUS;
	static int N_SEGMENTS_NUMBER;
	static int N_CIRCLE_SEGMENTS_NUMBER;

public:
	Torus();
	Torus(std::shared_ptr<ID3D11DeviceContext> deviceContext,
		ShaderBase* shaderBase,
		gk2::DeviceHelper device,
		gk2::Camera& camera,
		InputClass* input);
	Torus(const Torus& torus);
	Torus(Service& service);
	~Torus();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw();

	static void decrementN_SEGMENTS_NUMBER();
	static void incrementN_SEGMENTS_NUMBER();
	static void decrementN_CIRCLE_SEGMENTS_NUMBER();
	static void incrementN_CIRCLE_SEGMENTS_NUMBER();

	void setStereoscopy(bool);

	Torus& operator=(const Torus& element);

private:
	XMFLOAT3 TorusPos(float a, float t);
	XMFLOAT3 TorusDt(float a, float t);
	XMFLOAT3 TorusDa(float a, float t);

	virtual void setTriangleTopology();
	virtual void setLineTopology();

	bool m_isStereoscopic = true;
};