////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

using namespace gk2;

const unsigned int ModelClass::VB_STRIDE = sizeof(VertexPosNormal);
const unsigned int ModelClass::VB_OFFSET = 0;
int ModelClass::m_counter = 0;


ModelClass::ModelClass(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device, gk2::Camera camera, InputClass* input)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_device = device;
	m_context = deviceContext;
	m_shader_base = shader_base;
	m_camera = camera;
	m_input = input;
}


ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::ModelClass(Service& service)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_device = service.Device;
	m_context = service.Context;
	m_shader_base = service.Shader[service.shaderIndex];
	m_camera = service.Camera;
	m_input = service.InputClass;
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

XMFLOAT4 ModelClass::GetPosition()
{
	/*return m_position;*/
	return XMFLOAT4(m_modelMatrix._41, m_modelMatrix._42, m_modelMatrix._43, 1);
}

XMFLOAT4 ModelClass::GetNormalizedPosition()
{
	return XMFLOAT4(m_position.x * m_modelMatrix._11, m_position.y * m_modelMatrix._22, m_position.z * m_modelMatrix._33, m_position.w * m_modelMatrix._44);
}

void ModelClass::Initialize()
{
	m_modelMatrix = XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);

	m_id = m_counter++;
	m_selected = false;
	m_position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ModelClass::SetPosition(XMFLOAT4 position)
{
	m_position = position;
}

float ModelClass::GetSquareDistanceBetweenModels(ModelClass* first, ModelClass*second)
{
	XMFLOAT4 firstPos = first->GetPosition();
	XMFLOAT4 secondPos = second->GetPosition();
	float squareX = (firstPos.x - secondPos.x) * (firstPos.x - secondPos.x);
	float squareY = (firstPos.y - secondPos.y) * (firstPos.y - secondPos.y);
	float squareZ = (firstPos.z - secondPos.z) * (firstPos.z - secondPos.z);
	return squareX + squareY + squareZ;
}

void ModelClass::updatePosition(XMMATRIX* transform)
{
	//XMMATRIX old_transform = XMMATRIX(*transform);
	////potrzebna jest translacja i mno¿ymy pozycjê razy wektor
	//XMMATRIX old_position = {
	//	m_position.x, m_position.y, m_position.z, m_position.w,
	//	0, 0, 0, 0,
	//	0, 0, 0, 0,
	//	0, 0, 0, 0,
	//};
	//XMMATRIX new_position = old_position * old_transform;
	//m_position.x = new_position._11;
	//m_position.y = new_position._12;
	//m_position.z = new_position._13;
	//m_position.w = new_position._14;

	//XMFLOAT3 float3 = XMFLOAT3(m_position.x,
	//	m_position.y,
	//	m_position.z);
	//XMVECTOR pos = XMVector3Transform(XMLoadFloat3(&float3), *transform);
	//m_position.x = XMVectorGetX(pos);
	//m_position.x = XMVectorGetY(pos);
	//m_position.x = XMVectorGetZ(pos);
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

XMMATRIX* ModelClass::CreateTranslationMatrix(XMFLOAT4 offset)
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
	double r = 10;
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
	double r = 10;
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
	updatePosition(scaleMatrix);
	m_modelMatrix = XMMatrixMultiply(*scaleMatrix, m_modelMatrix);
}

void ModelClass::Translate(XMFLOAT4& delta)
{
	XMMATRIX* translate = CreateTranslationMatrix(delta);
	updatePosition(translate);
	m_modelMatrix = XMMatrixMultiply(*translate, m_modelMatrix);
}

void ModelClass :: RotateX(double rotation)
{
	XMMATRIX* rotationMatrix = CreateXAxisRotationMatrix(rotation);
	updatePosition(rotationMatrix);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}
void ModelClass::RotateY(double rotation)
{
	XMMATRIX* rotationMatrix = CreateYAxisRotationMatrix(rotation);
	updatePosition(rotationMatrix);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}
void ModelClass::RotateZ(double rotation)
{
	XMMATRIX* rotationMatrix = CreateZAxisRotationMatrix(rotation);
	updatePosition(rotationMatrix);
	m_modelMatrix = XMMatrixMultiply(*rotationMatrix, m_modelMatrix);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

XMFLOAT4 ModelClass::GetTranslatedPosition(ModelClass* cursor)
{
	XMMATRIX modelMatrix = cursor->m_modelMatrix;
	XMFLOAT4 normalized = cursor->GetPosition();//XMFLOAT4(m_position.x * modelMatrix._11, m_position.y * modelMatrix._22, m_position.z * modelMatrix._33, m_position.w * modelMatrix._44);
	XMFLOAT4 windowRect = XMFLOAT4(300, 300, 300, 300); //FIXME
	return XMFLOAT4(normalized.x * windowRect.x, normalized.y * windowRect.y, normalized.z * windowRect.z,
		normalized.w * windowRect.w);
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