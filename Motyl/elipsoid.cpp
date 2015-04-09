#include "elipsoid.h"
#include "shader_elipsoid.h"

using namespace gk2;

Elipsoid::Elipsoid(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device, gk2::Camera camera, InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
}


Elipsoid::Elipsoid(const Elipsoid& other) : ModelClass(other)
{
}


Elipsoid::~Elipsoid()
{
}

Elipsoid::Elipsoid()
{
}

void* Elipsoid::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void Elipsoid::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}


void Elipsoid::Initialize()
{
	ModelClass::Initialize();

	Sleep(10);
	square_X = fmaxf(2.0f, square_X - 2);
	square_Y = fmaxf(2.0f, square_Y - 2);
	std::vector<VertexPosNormal> points;
	points.clear();
	for (int i = -width; i + square_X / 2 <= width; i += square_X)
		for (int j = -height; j + square_Y / 2 <= height; j += square_Y)
		{
			// (i,j) = lewy górny róg kwadratu
			int center_X = i + square_X / 2;
			int center_Y = j + square_Y / 2;
			XMFLOAT3* position = ElipsoidPos(center_X, center_Y);
			if (position != NULL)
			{
				VertexPosNormal vertex1;
				vertex1.Pos = XMFLOAT3(center_X, center_Y, position->z);
				XMFLOAT3 norm = *ElipsoidNormal(vertex1.Pos);
				vertex1.Normal = norm;
				points.push_back(vertex1);
			}
		}

	VertexPosNormal* vertices;
	unsigned short* indices;
	//D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = points.size();

	// Set the number of indices in the index array.
	m_indexCount = points.size();//points.size() * 3 / 2;

	// Create the vertex array.
	vertices = new VertexPosNormal[m_vertexCount];

	// Create the index array.
	indices = new unsigned short[m_indexCount];
	int index = 0;
	for (int i = 0; i < points.size(); i++)
	{
		vertices[i] = points[i];
	}

	if (m_vertexCount > 0){
		m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
		//m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
		//Draw(false, false);
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
}

float Elipsoid :: calculateSquareEquation(XMMATRIX& D_M, float x, float y)
{
	float a = D_M._33;
	float b = D_M._31 * x +
		D_M._32 * y +
		D_M._13 * x +
		D_M._23 * y +
		D_M._43;
	D_M._34;
	float c = D_M._11 * x * x +
		D_M._21 * x * y +
		D_M._41 * x +
		D_M._12 * x * y +
		D_M._22 * y * y +
		D_M._42 * y +
		D_M._14 * x +
		D_M._24 * y +
		D_M._44;

	float delta = b * b - 4 * a * c;

	if (delta < 0)
	{
		counterNo++;
		return NULL;
	}
	else
	{
		counterYes++;
		//sprawdziæ to które bli¿ej kamery
		return fmaxf((-b + sqrtf(delta)) / (2 * a), (-b - sqrtf(delta)) / (2 * a));
	}
}


XMFLOAT3* Elipsoid::ElipsoidPos(float x, float y)
{
	XMMATRIX M = m_modelMatrix, M_INV, M_INV_T, D_M, D;

	float _a = 1 / (a * a);
	float _b = 1 / (b * b);
	float _c = 1 / (c * c);

	D = XMMATRIX{
		_a, 0, 0, 0,
		0, _b, 0, 0,
		0, 0, _c, 0,
		0, 0, 0, -1
	};

	XMVECTOR res;
	M_INV = XMMatrixInverse(&res, M);
	M_INV_T = XMMatrixTranspose(M_INV);

	D_M = M_INV_T * D * M_INV;

	float z = calculateSquareEquation(D_M, x, y);

	if (z == NULL)
		return NULL;
	else
		return new XMFLOAT3(x, y, z);
}



XMFLOAT3* Elipsoid::ElipsoidNormal(XMFLOAT3& position)
{

	float a = 20, b = 10, c = 10;

	a = 1 / (a * a);
	b = 1 / (b * b);
	c = 1 / (c * c);
	return new XMFLOAT3{
		2 * position.x * a,
		2 * position.y * b,
		2 * position.z * c
	};
}

void Elipsoid::resetSize()
{
	square_X = 40;
	square_Y = 40;
}

void Elipsoid::Draw()
{
	Initialize();
	m_context->UpdateSubresource(m_shader_base->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);
	ElipsoidShader* shader = dynamic_cast<ElipsoidShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, new XMFLOAT4(intensity, 0.0f, 0.0f, 0.0f), 0, 0);
	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE_WITH_NORMAL, &VB_OFFSET);
	m_context->Draw(m_vertexCount, 0);
}

void Elipsoid::setTriangleTopology(){}
void Elipsoid::setLineTopology() {}