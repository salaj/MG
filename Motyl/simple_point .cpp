#include "simple_point .h"
#include "shader_simple_point.h"

using namespace gk2;

const float SimplePoint::R_SMALL_TORUS = 0.1f;
const float SimplePoint::R_BIG_TORUS = 0.5;
int SimplePoint::N_SEGMENTS_NUMBER = 64;
int SimplePoint::N_CIRCLE_SEGMENTS_NUMBER = 10;

SimplePoint::SimplePoint(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	ModelClass::Initialize();
}

SimplePoint::SimplePoint(Service& service) : ModelClass(service)
{
	m_Type = ModelType::SimplePointType;
	ModelClass::Initialize();
}

SimplePoint::SimplePoint(const SimplePoint& other) : ModelClass(other)
{
}


SimplePoint::~SimplePoint()
{
}

SimplePoint::SimplePoint()
{
}

void* SimplePoint::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void SimplePoint::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}


XMFLOAT3 SimplePoint::TorusPos(float a, float t)
{
	float xt = (R_BIG_TORUS + R_SMALL_TORUS * cosf(a)) * cosf(t);
	float yt = (R_BIG_TORUS + R_SMALL_TORUS * cosf(a)) * sinf(t);
	float zt = R_SMALL_TORUS * sinf(a);
	return XMFLOAT3(xt, yt, zt);
}

XMFLOAT3 SimplePoint::TorusDt(float a, float t)
{
	float xt = -sinf(t);
	float yt = cosf(t);
	float zt = 0;
	return XMFLOAT3(xt, yt, zt);
}

XMFLOAT3 SimplePoint::TorusDa(float a, float t)
{
	float xt = -sinf(a) * cosf(t);
	float yt = -sinf(a) * sinf(t);
	float zt = cosf(a);
	return XMFLOAT3(xt, yt, zt);
}

void SimplePoint::Initialize()
{
	// Set the number of vertices in the vertex array.
	m_vertexCount = 24;

	VertexPosNormal vertices[] =
	{
		//Front face
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },

		//Left face
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		//Bottom face
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		//Back face
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		//Right face
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		//Top face
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
	};
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	//m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);


	setTriangleTopology();
	//setLineTopology();
}

void SimplePoint::setLineTopology()
{

	// Set the number of indices in the index array.
	m_indexCount = 60;

	unsigned short indices[] =
	{
		0, 1, 1, 2, 2, 0, 0, 2, 2, 3,//Front face
		4, 5, 5, 6, 6, 4, 4, 6, 6, 7,//Left face
		8, 9, 9, 10, 10, 8, 8, 10, 10, 11,	//Botton face
		12, 13, 13, 14, 14, 12, 12, 14, 14, 15,	//Back face
		16, 17, 17, 18, 18, 16, 16, 18, 18, 19,	//Right face
		20, 21, 21, 22, 22, 20, 20, 22, 22, 23	//Top face
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, 60);
}

void SimplePoint::setTriangleTopology()
{
	// Set the number of indices in the index array.
	m_indexCount = 36;

	unsigned short indices[] =
	{
		0, 1, 2, 0, 2, 3,		//Front face
		4, 5, 6, 4, 6, 7,		//Left face
		8, 9, 10, 8, 10, 11,	//Botton face
		12, 13, 14, 12, 14, 15,	//Back face
		16, 17, 18, 16, 18, 19,	//Right face
		20, 21, 22, 20, 22, 23	//Top face
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

void SimplePoint::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}


void SimplePoint::Draw()
{
	SimplePointShader* shader = dynamic_cast<SimplePointShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);

	if (m_input->isStereoscopyActive)
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
	else
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);
	}

}