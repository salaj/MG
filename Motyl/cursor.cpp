#include "cursor.h"
#include "shader_cursor.h"

using namespace gk2;


Cursor::Cursor(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	m_Type = ModelType::CursorType;
	ModelClass::Initialize();
}

Cursor::Cursor(Service& service) : ModelClass(service)
{
	m_Type = ModelType::CursorType;
	ModelClass::Initialize();
}

Cursor::Cursor(const Cursor& other) : ModelClass(other)
{
}


Cursor::~Cursor()
{
}

Cursor::Cursor()
{
}

void* Cursor::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void Cursor::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}


void Cursor::Initialize()
{
	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	VertexPosNormal vertices[] =
	{
		//Front face
		{ XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, -0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	//m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);


	//setTriangleTopology();
	setLineTopology();
}

void Cursor::setLineTopology()
{

	// Set the number of indices in the index array.
	m_indexCount = 4;

	unsigned short indices[] =
	{
		0, 3, 1, 2,
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

void Cursor::setTriangleTopology()
{
	//// Set the number of indices in the index array.
	//m_indexCount = 36;

	//unsigned short indices[] =
	//{
	//	0, 1, 2, 0, 2, 3,		//Front face
	//	4, 5, 6, 4, 6, 7,		//Left face
	//	8, 9, 10, 8, 10, 11,	//Botton face
	//	12, 13, 14, 12, 14, 15,	//Back face
	//	16, 17, 18, 16, 18, 19,	//Right face
	//	20, 21, 22, 20, 22, 23	//Top face
	//};
	//m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

void Cursor::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}


void Cursor::Draw()
{
	CursorShader* shader = dynamic_cast<CursorShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE_WITH_NORMAL, &VB_OFFSET);
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