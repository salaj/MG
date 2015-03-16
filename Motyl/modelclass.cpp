////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

using namespace gk2;

const unsigned int ModelClass::VB_STRIDE = sizeof(VertexPosNormal);
const unsigned int ModelClass::VB_OFFSET = 0;

ModelClass::ModelClass(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_device = device;
	m_context = deviceContext;
	m_shader_base = shader_base;
	m_camera = camera;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

ModelClass::ModelClass()
{
}

void* ModelClass::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void ModelClass::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void ModelClass::Initialize()
{
	m_modelMatrix = XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}

XMMATRIX* ModelClass :: CreateXAxisRotationMatrix(float angle)
{
	return new XMMATRIX(
		1, 0, 0, 0,
		0, cosf(angle), -sinf(angle), 0,
		0, sinf(angle), cosf(angle), 0,
		0, 0, 0, 1
		);
}

XMMATRIX* ModelClass :: CreateYAxisRotationMatrix(float angle)
{
	return new XMMATRIX(
		cosf(angle), 0, sinf(angle), 0,
		0, 1, 0, 0,
		-sinf(angle), 0, cosf(angle), 0,
		0, 0, 0, 1
		);
}

XMMATRIX* ModelClass :: CreateZAxisRotationMatrix(float angle)
{
	return new XMMATRIX(
		cosf(angle), -sinf(angle), 0, 0,
		sinf(angle), cosf(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}

XMMATRIX* ModelClass::CreateTranslationMatrix(XMFLOAT3 offset)
{
	return new XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		offset.x, offset.y, offset.z, 1

		);
}

XMMATRIX* ModelClass::CreateScaleMatrix(float s)
{
	return new XMMATRIX(
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
		);
}

XMMATRIX ModelClass::createStereoscopicProjMatrixLeft()
{
	double r = 10;// m_camera.GetMinDistance() - m_camera.GetPosition().z;
	double e = diametryE;
	double val = -e / (2 * r);
	XMMATRIX ret = XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		val, 0, 0, 1 / r,
		0, 0, 0, 1
		);
	return ret;
}

XMMATRIX ModelClass::createStereoscopicProjMatrixRight()
{
	double r = 10;// m_camera.GetMinDistance() - m_camera.GetPosition().z;
	double e = diametryE;
	return XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		e / (2 * r), 0, 0, 1 / r,
		0, 0, 0, 1
		);
}

void ModelClass::Scale(float scale)
{
	XMMATRIX* scaleMatrix = CreateScaleMatrix(scale);
	m_modelMatrix = XMMatrixMultiply(*scaleMatrix, m_modelMatrix);
}

void ModelClass::Translate(XMFLOAT3& delta)
{
	XMMATRIX* translate = CreateTranslationMatrix(delta);
	m_modelMatrix = XMMatrixMultiply(*translate, m_modelMatrix);
}

void ModelClass :: RotateX(double rotation)
{
	XMMATRIX* rotationMatrix = CreateXAxisRotationMatrix(rotation);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}
void ModelClass::RotateY(double rotation)
{
	XMMATRIX* rotationMatrix = CreateYAxisRotationMatrix(rotation);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}
void ModelClass::RotateZ(double rotation)
{
	XMMATRIX* rotationMatrix = CreateZAxisRotationMatrix(rotation);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

void ModelClass::GetModelMatrix(XMMATRIX& modelMatrix)
{
	modelMatrix = m_modelMatrix;
	return;
}

void ModelClass::SetModelMatrix(XMMATRIX& modelMatrix)
{
	m_modelMatrix = modelMatrix;
}