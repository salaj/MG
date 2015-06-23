#include "shader_intersection_square.h"

using namespace gk2;

#define RESOURCES_PATH L"resources/"
const std::wstring IntersectionSquareShader::ShaderFile = RESOURCES_PATH L"shaders/IntersectionSquare.hlsl";


void* IntersectionSquareShader::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void IntersectionSquareShader::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

IntersectionSquareShader::IntersectionSquareShader(std::shared_ptr<ID3D11DeviceContext> context,
	gk2::DeviceHelper device,
	D3D11_PRIMITIVE_TOPOLOGY topology) : ShaderBase(context, device, topology)
{
}

IntersectionSquareShader::~IntersectionSquareShader()
{
}

IntersectionSquareShader::IntersectionSquareShader()
{
}

IntersectionSquareShader::IntersectionSquareShader(const IntersectionSquareShader&)
{
}

std::shared_ptr<ID3D11Buffer>& IntersectionSquareShader::GetCBColor()
{
	return m_color;
}


void IntersectionSquareShader::InitializeConstantBuffers()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMMATRIX);
	desc.Usage = D3D11_USAGE_DEFAULT;
	m_cbWorld = m_device.CreateBuffer(desc);
	m_cbView = m_device.CreateBuffer(desc);
	m_cbProj = m_device.CreateBuffer(desc);
	//desc.ByteWidth = sizeof(XMFLOAT4) * 3;
	//m_cbLightPos = m_device.CreateBuffer(desc);
	//desc.ByteWidth = sizeof(XMFLOAT4) * 5;
	//m_cbLightColors = m_device.CreateBuffer(desc);
	//desc.ByteWidth = sizeof(XMFLOAT4);
	//m_cbSurfaceColor = m_device.CreateBuffer(desc);
	desc.ByteWidth = sizeof(XMFLOAT4);
	m_color = m_device.CreateBuffer(desc);
}


void IntersectionSquareShader::InitializeShaders()
{
	std::shared_ptr<ID3DBlob> vsByteCode = m_device.CompileD3DShader(ShaderFile, "VS_Main", "vs_4_0");
	std::shared_ptr<ID3DBlob> psByteCode = m_device.CompileD3DShader(ShaderFile, "PS_Main", "ps_4_0");
	m_vertexShader = m_device.CreateVertexShader(vsByteCode);
	m_pixelShader = m_device.CreatePixelShader(psByteCode);
	m_inputLayout = m_device.CreateInputLayout<VertexPos>(vsByteCode);

	m_vertexShaderContour = m_device.CreateVertexShader(vsByteCode);
	m_pixelShaderContour = m_device.CreatePixelShader(psByteCode);
	m_inputLayoutContour = m_device.CreateInputLayout<VertexPos>(vsByteCode);
}

void IntersectionSquareShader::SetConstantBuffers()
{
	ID3D11Buffer* vsb[] = { m_cbWorld.get(), m_cbView.get(), m_cbProj.get()};
	m_context->VSSetConstantBuffers(0, 3, vsb);
	ID3D11Buffer* psb[] = { m_color.get() };
	m_context->PSSetConstantBuffers(0, 1, psb);
}