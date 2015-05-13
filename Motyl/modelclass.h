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
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////

using namespace gk2;
using namespace std;



class ModelClass
{

public:

	static int m_counter;
	static int m_fakeCounter;

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
	void Translate(XMFLOAT4&);
	void Scale(float);

	XMFLOAT4 GetPosition();
	XMFLOAT3 GetPosition3();
	XMFLOAT4 GetNormalizedPosition();
	XMFLOAT4 GetTranslatedPosition(ModelClass* cursor);
	void SetPosition(XMFLOAT4 position);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMVECTOR pos);
	static float GetSquareDistanceBetweenModels(ModelClass*, ModelClass*);
	static XMFLOAT4 GetRelativeScaleVector(ModelClass*, ModelClass*);

	XMMATRIX m_modelMatrix;
	bool m_selected;
	int m_id;
	ModelType m_Type = ModelType::Undecided;
	bool m_isGenuine;
protected:
	std::shared_ptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount;
	gk2::DeviceHelper m_device;
	std::shared_ptr<ID3D11DeviceContext> m_context;
	ShaderBase* m_shader_base;
	InputClass* m_input;

	XMMATRIX createStereoscopicProjMatrixLeft();
	XMMATRIX createStereoscopicProjMatrixRight();

	static const unsigned int VB_STRIDE_WITH_NORMAL;
	static const unsigned int VB_STRIDE;
	static const unsigned int VB_OFFSET;

	virtual void setTriangleTopology() = 0;
	virtual void setLineTopology() = 0;

private:
	static int counter;
	//const float rotation = (float)XM_PI * 0.05f;

	gk2::Camera m_camera;

	const float diametryE = 1.f;

	XMMATRIX CreateXAxisRotationMatrix(float angle);
	XMMATRIX CreateYAxisRotationMatrix(float angle);
	XMMATRIX CreateZAxisRotationMatrix(float angle);
	XMMATRIX* CreateTranslationMatrix(XMFLOAT4 offset);
	XMMATRIX* CreateScaleMatrix(float s);

	XMFLOAT4 m_position;
};