#include "torus.h"
#include "shader_torus.h"

using namespace gk2;

const float Torus::R_SMALL_TORUS = 0.1f;
const float Torus::R_BIG_TORUS = 0.5;
int Torus::N_SEGMENTS_NUMBER = 64;
int Torus::N_CIRCLE_SEGMENTS_NUMBER = 10;

const double delta = 0.05f;
double sum = 0;

Torus::Torus(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	ModelClass::Initialize();
}

Torus::Torus(Service& service) : ModelClass(service)
{
	ModelClass::Initialize();
}

Torus::Torus(const Torus& other) : ModelClass(other)
{
}


Torus::~Torus()
{
}

Torus::Torus()
{
}

void* Torus::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void Torus::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}


void Torus::decrementN_SEGMENTS_NUMBER()
{
	sum += delta;
	if (sum < 1.0f)
		return;
	sum = 0.0f;
	if (N_SEGMENTS_NUMBER < 5)
		return;
	N_SEGMENTS_NUMBER--;
}

void Torus::incrementN_SEGMENTS_NUMBER()
{
	sum += delta;
	if (sum < 1.0f)
		return;
	sum = 0.0f;
	if (N_SEGMENTS_NUMBER > 200)
		return;
	N_SEGMENTS_NUMBER++;
}

void Torus::decrementN_CIRCLE_SEGMENTS_NUMBER()
{
	sum += delta;
	if (sum < 1.0f)
		return;
	sum = 0.0f;
	if (N_CIRCLE_SEGMENTS_NUMBER < 3)
		return;
	N_CIRCLE_SEGMENTS_NUMBER--;
}

void Torus::incrementN_CIRCLE_SEGMENTS_NUMBER()
{
	sum += delta;
	if (sum < 1.0f)
		return;
	sum = 0.0f;
	if (N_CIRCLE_SEGMENTS_NUMBER > 20)
		return;
	N_CIRCLE_SEGMENTS_NUMBER++;
}

XMFLOAT3 Torus::TorusPos(float a, float t)
{
	float xt = (R_BIG_TORUS + R_SMALL_TORUS * cosf(a)) * cosf(t);
	float yt = (R_BIG_TORUS + R_SMALL_TORUS * cosf(a)) * sinf(t);
	float zt = R_SMALL_TORUS * sinf(a);
	return XMFLOAT3(xt, yt, zt);
}

XMFLOAT3 Torus::TorusDt(float a, float t)
{
	float xt = -sinf(t);
	float yt = cosf(t);
	float zt = 0;
	return XMFLOAT3(xt, yt, zt);
}

XMFLOAT3 Torus::TorusDa(float a, float t)
{
	float xt = -sinf(a) * cosf(t);
	float yt = -sinf(a) * sinf(t);
	float zt = cosf(a);
	return XMFLOAT3(xt, yt, zt);
}

void Torus::Initialize()
{

	VertexPosNormal* vertices;
	// Set the number of vertices in the vertex array.
	m_vertexCount = N_SEGMENTS_NUMBER * N_CIRCLE_SEGMENTS_NUMBER;

	// Create the vertex array.
	vertices = new VertexPosNormal[m_vertexCount];

	for (int i = 0; i < N_SEGMENTS_NUMBER; i++)
	{
		FLOAT t = (2 * D3DX_PI * i) / (N_SEGMENTS_NUMBER);
		for (int j = 0; j < N_CIRCLE_SEGMENTS_NUMBER; j++)
		{
			FLOAT a = (2 * D3DX_PI * j) / (N_CIRCLE_SEGMENTS_NUMBER);
			vertices[N_CIRCLE_SEGMENTS_NUMBER * i + j].Pos = TorusPos(a, t);
			XMVECTOR output = XMVector3Cross(XMLoadFloat3(&TorusDt(a, t)), (XMLoadFloat3(&TorusDa(a, t))) );
			XMVECTOR outputNormalize = XMVector3Normalize(output);
			XMStoreFloat3( &(vertices[N_CIRCLE_SEGMENTS_NUMBER * i + j].Normal), outputNormalize);
			//XMStoreFloat3( &(vertices[N_CIRCLE_SEGMENTS_NUMBER * i + j].Color), outputNormalize);
			//vertices[N_CIRCLE_SEGMENTS_NUMBER * i + j].Normal = XMFLOAT3(1.0f, 1.0f, 1.0f);
			/*vertices[N_CIRCLE_SEGMENTS_NUMBER * i + j].Color = XMFLOAT3(1.0f, 0.0f, 0.0f);*/
		}
	}

	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	//setTriangleTopology();
	setLineTopology();
}

void Torus::setLineTopology()
{
	int numberOfVerticesInSquare = 6;

	unsigned short* indices;

	// Set the number of indices in the index array.
	m_indexCount = N_SEGMENTS_NUMBER * N_CIRCLE_SEGMENTS_NUMBER * numberOfVerticesInSquare;

	// Create the index array.
	indices = new unsigned short[m_indexCount];

	for (int j = 0; j < N_CIRCLE_SEGMENTS_NUMBER; j++)
	{
		for (int i = 0; i < N_SEGMENTS_NUMBER; i++)
		{
			int firstRight = (i * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			int secondRight = ((i + 1) * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			int thirdRight;
			if (j == N_CIRCLE_SEGMENTS_NUMBER - 1) 
				thirdRight = ((i)* N_CIRCLE_SEGMENTS_NUMBER + j + 1) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			else
				thirdRight = ((i + 1) * N_CIRCLE_SEGMENTS_NUMBER + j + 1) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);


			//FIRST EDGE
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare] = firstRight;
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare + 1] = secondRight;

			//SECOND EDGE
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare + 2] = secondRight;
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare + 3] = thirdRight;

				//THIRD EDGE
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare + 4] = thirdRight;
			indices[(j * N_SEGMENTS_NUMBER + i) * numberOfVerticesInSquare + 5] = firstRight;
		}
	}

	unsigned int wartosc = (unsigned int)m_indexCount;
	m_indexBuffer = m_device.CreateIndexBuffer(indices, (unsigned int)m_indexCount);

	delete[] indices;
	indices = 0;
}

void Torus::setTriangleTopology()
{
	unsigned short* indices;

	// Set the number of indices in the index array.
	m_indexCount = N_SEGMENTS_NUMBER * N_CIRCLE_SEGMENTS_NUMBER * 3 * 2;

	// Create the index array.
	indices = new unsigned short[m_indexCount];

	for (int j = 0; j < N_CIRCLE_SEGMENTS_NUMBER; j++)
	{
		for (int i = 0; i < N_SEGMENTS_NUMBER; i++)
		{
			indices[(j * N_SEGMENTS_NUMBER + i) * 3] = (i * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			indices[(j * N_SEGMENTS_NUMBER + i) * 3 + 1] = ((i + 1) * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			if (j == N_CIRCLE_SEGMENTS_NUMBER - 1) //last iteration
				indices[(j * N_SEGMENTS_NUMBER + i) * 3 + 2] = ((i)* N_CIRCLE_SEGMENTS_NUMBER + j + 1) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			else
				indices[(j * N_SEGMENTS_NUMBER + i) * 3 + 2] = ((i + 1) * N_CIRCLE_SEGMENTS_NUMBER + j + 1) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
		}
	}

	int offset = N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER * 3;

	for (int j = 0; j < N_CIRCLE_SEGMENTS_NUMBER; j++)
	{
		for (int i = 0; i < N_SEGMENTS_NUMBER; i++)
		{
			int firstLeft = offset + (j * N_SEGMENTS_NUMBER + i) * 3;
			int firstRight = (i * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			indices[firstLeft] = firstRight;
			int secondLeft = offset + (j * N_SEGMENTS_NUMBER + i) * 3 + 1;
			int secondRight = ((i + 1) * N_CIRCLE_SEGMENTS_NUMBER + j) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			indices[secondLeft] = secondRight;
			int thirdLeft = offset + (j * N_SEGMENTS_NUMBER + i) * 3 + 2;
			int thirdRight = (i* N_CIRCLE_SEGMENTS_NUMBER + j - 1) % (N_CIRCLE_SEGMENTS_NUMBER * N_SEGMENTS_NUMBER);
			if (j == 0)//first interation
			{
				if (i == N_SEGMENTS_NUMBER - 1)
					int b = 1;
				thirdRight = N_CIRCLE_SEGMENTS_NUMBER * (i + 1) - 1;
			}
			if (j == N_CIRCLE_SEGMENTS_NUMBER - 1)
				int a = 1;
			indices[thirdLeft] = thirdRight;

			indices[firstLeft] = secondRight;
			indices[secondLeft] = firstRight;

		}
	}

	unsigned int wartosc = (unsigned int)m_indexCount;
	m_indexBuffer = m_device.CreateIndexBuffer(indices, (unsigned int)m_indexCount);

	delete[] indices;
	indices = 0;
}

void Torus::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}


void Torus::Draw()
{
	TorusShader* shader = dynamic_cast<TorusShader*>(m_shader_base);
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