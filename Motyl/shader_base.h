#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include <memory>
#include "applicationBase.h"
#include "utils.h"
#include "vertices.h"
#include "deviceHelper.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderBase
////////////////////////////////////////////////////////////////////////////////
class ShaderBase
{

public:

	ShaderBase();
	ShaderBase(std::shared_ptr<ID3D11DeviceContext>, gk2::DeviceHelper m_device, D3D11_PRIMITIVE_TOPOLOGY topology);
	ShaderBase(const ShaderBase&);
	~ShaderBase();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	std::shared_ptr<ID3D11DeviceContext> m_context;

	std::shared_ptr<ID3D11Buffer>& GetCBWorldMatrix();
	std::shared_ptr<ID3D11Buffer>& GetCBViewMatrix();
	std::shared_ptr<ID3D11Buffer>& GetCBProjMatrix();
	//std::shared_ptr<ID3D11Buffer>& GetCBColor();

	//Initializes constant buffers
	virtual void InitializeConstantBuffers() = 0;

	//Initializes shaders
	virtual void InitializeShaders() = 0;

	void LoadContent();
	void SetContent();
	void UnloadContent();
	
	D3D11_PRIMITIVE_TOPOLOGY getTopology();
protected:
	//Shader's constant buffer containing Local -> World matrix
	std::shared_ptr<ID3D11Buffer> m_cbWorld;
	//Shader's constant buffer containing World -> Camera matrix
	std::shared_ptr<ID3D11Buffer> m_cbView;
	//Shader's constant buffer containing projection matrix
	std::shared_ptr<ID3D11Buffer> m_cbProj;

	//std::shared_ptr<ID3D11Buffer> m_color;

	gk2::DeviceHelper m_device;

	//Path to the shaders' file
	static const std::wstring ShaderFile;

	//Shader's
	std::shared_ptr<ID3D11VertexShader> m_vertexShader;
	std::shared_ptr<ID3D11PixelShader> m_pixelShader;
	//VertexPosNormal input layout
	std::shared_ptr<ID3D11InputLayout> m_inputLayout;

	//Binds shaders to the device context
	void SetShaders();
	//Binds constant buffers to shaders
	virtual void SetConstantBuffers() = 0;

private:
	D3D11_PRIMITIVE_TOPOLOGY m_topology;
};
