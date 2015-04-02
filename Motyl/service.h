#pragma once

#include "applicationBase.h"
#include "shader_base.h"
#include "camera.h"
#include "inputclass.h"
//#include "guiUpdater.h"

using namespace std;

namespace gk2{

	class Service
	{
	public:

		Service();
		virtual ~Service();

		//static void* operator new(size_t size);
		//static void operator delete(void* ptr);

		shared_ptr<ID3D11DeviceContext> Context;
		ShaderBase** Shader;
		int shaderIndex;
		gk2::DeviceHelper Device;
		gk2::Camera Camera;

		InputClass* InputClass;
		//GUIUpdater* GUIUpdater;
		shared_ptr<Mouse> Mouse;

	};
}
