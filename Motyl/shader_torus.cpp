#include "shader_torus.h"

using namespace gk2;

#define RESOURCES_PATH L"resources/"
const std::wstring TorusShader::ShaderFile = RESOURCES_PATH L"shaders/Torus.hlsl";


void* TorusShader::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void TorusShader::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

TorusShader::TorusShader(std::shared_ptr<ID3D11DeviceContext> context,
	gk2::DeviceHelper device,
	D3D11_PRIMITIVE_TOPOLOGY topology) : ShaderBase(context, device, topology)
{
}

TorusShader::~TorusShader()
{
}

TorusShader::TorusShader()
{
}

TorusShader::TorusShader(const TorusShader&)
{
}

std::shared_ptr<ID3D11Buffer>& TorusShader::GetCBColor()
{
	return m_color;
}


void TorusShader::InitializeConstantBuffers()
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


void TorusShader::InitializeShaders()
{
	std::shared_ptr<ID3DBlob> vsByteCode = m_device.CompileD3DShader(ShaderFile, "VS_Main", "vs_4_0");
	std::shared_ptr<ID3DBlob> psByteCode = m_device.CompileD3DShader(ShaderFile, "PS_Main", "ps_4_0");
	m_vertexShader = m_device.CreateVertexShader(vsByteCode);
	m_pixelShader = m_device.CreatePixelShader(psByteCode);
	m_inputLayout = m_device.CreateInputLayout<VertexPosNormal>(vsByteCode);
}

//void TorusShader::SetShaders()
//{
//	m_context->IASetInputLayout(m_inputLayout.get());
//	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	m_context->VSSetShader(m_vertexShader.get(), 0, 0);
//	m_context->PSSetShader(m_pixelShader.get(), 0, 0);
//}

void TorusShader::SetConstantBuffers()
{
	ID3D11Buffer* vsb[] = { m_cbWorld.get(), m_cbView.get(), m_cbProj.get()};
	m_context->VSSetConstantBuffers(0, 3, vsb);
	ID3D11Buffer* psb[] = { m_color.get() };
	m_context->PSSetConstantBuffers(0, 1, psb);
}