//#ifndef __CAMERA_H_
//#define __CAMERA_H_
#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include "utils.h"
#include "vertices.h"
#include "deviceHelper.h"
#include <memory>



namespace gk2
{
	class Camera
	{
	public:
		Camera();

		Camera(float minDistance, float maxDistance, float distance = 0.0f);

		static void* operator new(size_t size);
		static void operator delete(void* ptr);

		void SetRange(float minDistance, float maxDistance);
		void Zoom(float d);
		void Rotate(float dx, float dy);
		XMMATRIX GetViewMatrix();

		void GetViewMatrix(XMMATRIX& viewMtx);
		XMFLOAT4 GetPosition();
		float GetMinDistance();
		float GetMaxDistance();

	private:

		float m_angleX;
		float m_angleY;
		float m_distance;
		float m_minDistance;
		float m_maxDistance;

		XMMATRIX camView;
		XMMATRIX matrixRot;


		void ClampDistance();
	};
}

//#endif
