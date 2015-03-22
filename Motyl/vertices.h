#pragma once

#include <d3d11.h>
#include <xnamath.h>

namespace gk2
{
	struct VertexPos 
	{
		XMFLOAT3 Pos;
		static const unsigned int LayoutElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC Layout[LayoutElements];
	};

	struct VertexPosNormal
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		//XMFLOAT3 Color;
		static const unsigned int LayoutElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Layout[LayoutElements];
	};
}

