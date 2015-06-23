#include "line.h"
#include "shader_cursor.h"

using namespace gk2;


Line::Line(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera& camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	m_Type = ModelType::CursorType;
	ModelClass::Initialize();
}

Line::Line(Service& service) : ModelClass(service)
{
	m_Type = ModelType::LineType;
	ModelClass::Initialize();
}

Line::Line(const Line& other) : ModelClass(other)
{
}


Line::~Line()
{
}

Line::Line()
{
}

void* Line::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void Line::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void Line::Restart(float t)
{
	m_vertexCount = 2;
	VertexPosNormal vertices[] =
	{
		{ XMFLOAT3(X, Y, Q(t).z), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(X, Y, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }
	};
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	setLineTopology();
}


void Line::Initialize()
{
	// Set the number of vertices in the vertex array.
	m_vertexCount = 2;

	VertexPosNormal vertices[] =
	{
		{ XMFLOAT3(X, Y, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }
	};
	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	setLineTopology();
}

void Line::setLineTopology()
{

	// Set the number of indices in the index array.
	m_indexCount = 2;
	unsigned short indices[] =
	{
		0, 1
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

XMFLOAT4 Line::Q(float t)
{
	if (t > 1.0f)
		t = 1.0f;
	else if (t < 0.0f)
		t = 0.0f;
	return XMFLOAT4(X, Y, t, 1);
}

void Line::setTriangleTopology()
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

void Line::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}


void Line::Draw()
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