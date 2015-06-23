#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Line
////////////////////////////////////////////////////////////////////////////////
class Line : public ModelClass
{

private:

public:
	Line();
	Line(std::shared_ptr<ID3D11DeviceContext>,
		ShaderBase*,
		gk2::DeviceHelper device,
		gk2::Camera& camera,
		InputClass* input);
	Line(const Line&);
	Line(Service& servie);
	~Line();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw();
	virtual void Restart(float t);
	void setStereoscopy(bool);

	Line& operator=(const Line& element);

	float X, Y;
	XMFLOAT4 Q(float t);

private:
	virtual void setTriangleTopology();
	virtual void setLineTopology();

	bool m_isStereoscopic = true;
};