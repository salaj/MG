#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////
class Cursor : public ModelClass
{

private:

public:
	Cursor();
	Cursor(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera& camera,
		InputClass* input);
	Cursor(const Cursor&);
	Cursor(Service& servie);
	~Cursor();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw();

	void setStereoscopy(bool);

	Cursor& operator=(const Cursor& element);

private:
	virtual void setTriangleTopology();
	virtual void setLineTopology();

	bool m_isStereoscopic = true;
};