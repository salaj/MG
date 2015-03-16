#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <d3d11.h>
#include <xnamath.h>

namespace gk2
{
	class Camera
	{
	public:
		Camera();

		Camera(float minDistance, float maxDistance, float distance =  0.0f);

		void SetRange(float minDistance, float maxDistance);
		void Zoom(float d);
		void Rotate(float dx, float dy);
		XMMATRIX GetViewMatrix();
		void GetViewMatrix(XMMATRIX& viewMatrix);
		XMFLOAT4 GetPosition();
		float GetMinDistance();
		float GetMaxDistance();

	private:

		float m_angleX;
		float m_angleY;
		float m_distance;
		float m_minDistance;
		float m_maxDistance;

		void ClampDistance();
	};
}

#endif __GK2_CAMERA_H_
