#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include <memory>
#include "shader_base.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TorusShader
////////////////////////////////////////////////////////////////////////////////
class BezierCurveShader : public ShaderBase
{

public:

	BezierCurveShader();
	BezierCurveShader(std::shared_ptr<ID3D11DeviceContext>,
		gk2::DeviceHelper m_device,
		D3D11_PRIMITIVE_TOPOLOGY topology);
	BezierCurveShader(const BezierCurveShader&);
	~BezierCurveShader();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	//Initializes constant buffers
	virtual void InitializeConstantBuffers();

	//Initializes shaders
	virtual void InitializeShaders();

	std::shared_ptr<ID3D11Buffer>& GetCBColor();


	//POLYGON based contours
	//Shader's
	std::shared_ptr<ID3D11VertexShader> m_vertexShaderContour;
	std::shared_ptr<ID3D11PixelShader> m_pixelShaderContour;
	//VertexPosNormal input layout
	std::shared_ptr<ID3D11InputLayout> m_inputLayoutContour;
	D3D11_PRIMITIVE_TOPOLOGY m_NodesTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

private:
	//Path to the shaders' file
	static const std::wstring ShaderFile;
	std::shared_ptr<ID3D11Buffer> m_color;

	////Binds shaders to the device context
	//virtual void SetShaders();
	//Binds constant buffers to shaders
	virtual void SetConstantBuffers();
};