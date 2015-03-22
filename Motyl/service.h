#pragma once

#include "applicationBase.h"
#include "modelclass.h"
#include "shader_base.h"
#include "camera.h"
#include "inputclass.h"

using namespace std;

class Service
{
public:

	Service();
	virtual ~Service();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	std::shared_ptr<ID3D11DeviceContext> Context;
	ShaderBase** Shader;
	gk2::DeviceHelper Device;
	gk2::Camera Camera;

	InputClass* InputClass;

private:

};
