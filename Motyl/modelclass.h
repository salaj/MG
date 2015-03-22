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

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////

class ModelClass
{

public:
	ModelClass();
	ModelClass(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*, 
		gk2::DeviceHelper device,
		gk2::Camera camera,
		InputClass* input);
	ModelClass(const ModelClass&);
	ModelClass(Service& servie);
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


protected:
	std::shared_ptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount;
	XMMATRIX m_modelMatrix;
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