#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Elipsoid
////////////////////////////////////////////////////////////////////////////////
class Elipsoid : public ModelClass
{

private:

	const float R_SMALL_TORUS = 0.1;
	const float R_BIG_TORUS = 0.5;
	int N_SEGMENTS_NUMBER = 64;
	int N_CIRCLE_SEGMENTS_NUMBER = 10;

public:
	Elipsoid();
	Elipsoid(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera camera,
		InputClass* input);
	Elipsoid(const Elipsoid&);
	~Elipsoid();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw();


	Elipsoid& operator=(const Elipsoid& element);

private:
	int counterNo = 0, counterYes = 0;
	float a = 20;
	float b = 10;
	float c = 10;
	int height = 400;
	int width = 400;
	int square_X = 40;
	int square_Y = 40;
	// Set the number of vertices in the vertex array.
	int m_vertexCount;
	// Set the number of indices in the index array.
	int m_indexCount;

	float intensity = 10;
	//std::shared_ptr<ID3D11Buffer> m_lightIntensity;
	float calculateSquareEquation(XMMATRIX& D_M, float x, float y);
	void Elipsoid::resetSize();
	XMFLOAT3* ElipsoidPos(float x, float y);
	XMFLOAT3* ElipsoidNormal(XMFLOAT3& position);
	virtual void setTriangleTopology();
	virtual void setLineTopology();
};