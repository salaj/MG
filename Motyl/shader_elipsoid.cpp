#include "shader_elipsoid.h"

using namespace gk2;

#define RESOURCES_PATH L"resources/"
const std::wstring ElipsoidShader::ShaderFile = RESOURCES_PATH L"shaders/Elipsoid.hlsl";


void* ElipsoidShader::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void ElipsoidShader::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

ElipsoidShader::ElipsoidShader(std::shared_ptr<ID3D11DeviceContext> context,
	gk2::DeviceHelper device,
	D3D11_PRIMITIVE_TOPOLOGY topology) : ShaderBase(context, device, topology)
{
}

ElipsoidShader::~ElipsoidShader()
{
}

ElipsoidShader::ElipsoidShader()
{
}

ElipsoidShader::ElipsoidShader(const ElipsoidShader&)
{
}

std::shared_ptr<ID3D11Buffer>& ElipsoidShader::GetCBColor()
{
	return m_lightIntensity;
}


void ElipsoidShader::InitializeConstantBuffers()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMMATRIX);
	desc.Usage = D3D11_USAGE_DEFAULT;
	m_cbWorld = m_device.CreateBuffer(desc);
	m_cbView = m_device.CreateBuffer(desc);
	m_cbProj = m_device.CreateBuffer(desc);


	desc.ByteWidth = sizeof(XMFLOAT4);
	m_lightIntensity = m_device.CreateBuffer(desc);
}


void ElipsoidShader::InitializeShaders()
{
	std::shared_ptr<ID3DBlob> vsByteCode = m_device.CompileD3DShader(ShaderFile, "VS_Main", "vs_4_0");
	std::shared_ptr<ID3DBlob> psByteCode = m_device.CompileD3DShader(ShaderFile, "PS_Main", "ps_4_0");
	m_vertexShader = m_device.CreateVertexShader(vsByteCode);
	m_pixelShader = m_device.CreatePixelShader(psByteCode);
	m_inputLayout = m_device.CreateInputLayout<VertexPosNormal>(vsByteCode);
}

void ElipsoidShader::SetShaders()
{
	m_context->IASetInputLayout(m_inputLayout.get());
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_context->VSSetShader(m_vertexShader.get(), 0, 0);
	m_context->PSSetShader(m_pixelShader.get(), 0, 0);
}

void ElipsoidShader::SetConstantBuffers()
{
	ID3D11Buffer* vsb[] = { m_cbWorld.get(), m_cbView.get(), m_cbProj.get() };
	m_context->VSSetConstantBuffers(0, 3, vsb);
	ID3D11Buffer* psb[] = { m_lightIntensity.get() };
	m_context->PSSetConstantBuffers(0, 1, psb);
}