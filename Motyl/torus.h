//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Torus
////////////////////////////////////////////////////////////////////////////////
class Torus : public ModelClass
{

private:

	const float R_SMALL_TORUS = 0.1;
	const float R_BIG_TORUS = 0.5;
	int N_SEGMENTS_NUMBER = 64;
	int N_CIRCLE_SEGMENTS_NUMBER = 10;

public:
	Torus();
	Torus(std::shared_ptr<ID3D11DeviceContext>, ShaderBase*, gk2::DeviceHelper device, gk2::Camera);
	Torus(const Torus&);
	~Torus();

	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	virtual void Initialize();
	virtual void Draw(bool, bool);

	void decrementN_SEGMENTS_NUMBER();
	void incrementN_SEGMENTS_NUMBER();
	void decrementN_CIRCLE_SEGMENTS_NUMBER();
	void incrementN_CIRCLE_SEGMENTS_NUMBER();

	Torus& operator=(const Torus& element);

private:
	XMFLOAT3 TorusPos(float a, float t);
	XMFLOAT3 TorusDt(float a, float t);
	XMFLOAT3 TorusDa(float a, float t);

	virtual void setTriangleTopology();
	virtual void setLineTopology();
};