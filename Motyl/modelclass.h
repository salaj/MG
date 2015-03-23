#pragma once


//////////////
// INCLUDES //
//////////////
#include "service.h"
#include "utils.h"
#include "vertices.h"
#include "deviceHelper.h"
#include "shader_base.h"
#include "camera.h"
#include "inputclass.h"
#include "service.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////

using namespace gk2;
using namespace std;



class ModelClass
{

public:
	ModelClass();
	ModelClass(std::shared_ptr<ID3D11DeviceContext>deviceContext,
		ShaderBase* shaderBase,
		DeviceHelper deviceHelper,
		Camera camera,
		InputClass* input);
	ModelClass(const ModelClass&);
	ModelClass(Service& service);
	~ModelClass();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	//called when object is initialized
	virtual void Initialize() = 0;
	virtual void Draw() = 0;

	void Shutdown();
	void Render(std::shared_ptr<ID3D11DeviceContext>*);
	void SetModelMatrix(XMMATRIX&);
	void GetModelMatrix(XMMATRIX&);
	int GetIndexCount();

	///TRANSFORMATIONS

	void RotateX(double);
	void RotateY(double);
	void RotateZ(double);
	void Translate(XMFLOAT3&);
	void Scale(float);

	XMMATRIX m_modelMatrix;


protected:
	std::shared_ptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount;
	gk2::DeviceHelper m_device;
	std::shared_ptr<ID3D11DeviceContext> m_context;
	ShaderBase* m_shader_base;
	InputClass* m_input;

	XMMATRIX createStereoscopicProjMatrixLeft();
	XMMATRIX createStereoscopicProjMatrixRight();

	static const unsigned int VB_STRIDE;
	static const unsigned int VB_OFFSET;

	virtual void setTriangleTopology() = 0;
	virtual void setLineTopology() = 0;

private:
	//const float rotation = (float)XM_PI * 0.05f;

	gk2::Camera m_camera;

	const float diametryE = 1.f;

	XMMATRIX* CreateXAxisRotationMatrix(float angle);
	XMMATRIX* CreateYAxisRotationMatrix(float angle);
	XMMATRIX* CreateZAxisRotationMatrix(float angle);
	XMMATRIX* CreateTranslationMatrix(XMFLOAT3 offset);
	XMMATRIX* CreateScaleMatrix(float s);
};