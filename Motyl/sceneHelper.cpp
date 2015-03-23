#include "sceneHelper.h"

using namespace gk2;

void* SceneHelper::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void SceneHelper::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void SceneHelper::Initialize(Service& service)
{
	m_InputClass = service.InputClass;
	m_modelsManager.Initialize(service);
}

SceneHelper::SceneHelper()
{
}

SceneHelper::~SceneHelper()
{
	delete m_InputClass;
}

vector<ModelClass*>& SceneHelper::GetModels()
{
	return m_modelsManager.GetModels();
}
void SceneHelper::CreateModels()
{
	m_modelsManager.CreateModels();
}
void SceneHelper::InitializeModels()
{
	m_modelsManager.InitializeModels();
}
void SceneHelper::DrawModels()
{
	m_modelsManager.DrawModels();
}


void SceneHelper::translateModels(vector<ModelClass*>& models, XMFLOAT3 offset)
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i] -> Translate(offset);
	}
}

void SceneHelper::scaleModels(vector<ModelClass*>& models, float scale)
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i] -> Scale(scale);
	}
}

void SceneHelper::rotateModels(vector<ModelClass*>& models, float rotation, ActiveAxis axis)
{
	for (int i = 0; i < models.size(); i++)
	{
		switch (axis)
		{
		case AxisX:
			models[i]->RotateX(rotation);
			break;
		case AxisY:
			models[i]->RotateY(rotation);
			break;
		case AxisZ:
			models[i]->RotateZ(rotation);
			break;
		default:
			break;
		}
	}
}

void SceneHelper::AddModel()
{
	m_modelsManager.AddModel(SimplePointType);
}

void SceneHelper::CheckInput()
{
	vector<ModelClass*>& models = m_modelsManager.GetModels();
	vector<ModelClass*> activeModels;
	activeModels.push_back(models[0]);

	ActiveFeature feature = m_InputClass->getActiveFeature();
	ActiveAxis axis = m_InputClass->getActiveAxis();
	ActiveRadius radius = m_InputClass->getActiveRadius();
	float rotation = (float)XM_PI * 0.0002f;
	float factor = 0.0005f;
	float factorScale = factor;


	//if (m_InputClass->IsKeyDown(VK_OEM_PLUS))
	//{
	//	intensity += 3.0f;
	//	m_context->UpdateSubresource(m_lightIntensity.get(), 0, 0, new XMFLOAT4(intensity, 0.0f, 0.0f, 0.0f), 0, 0);
	//}
	//if (m_InputClass->IsKeyDown(VK_OEM_MINUS))
	//{
	//	intensity -= 3.0f;
	//	m_context->UpdateSubresource(m_lightIntensity.get(), 0, 0, new XMFLOAT4(intensity, 0.0f, 0.0f, 0.0f), 0, 0);
	//}

	if (m_InputClass->IsKeyDown(0x57)) //W
	{
		XMFLOAT3 offset = XMFLOAT3(0, factor, 0);
		translateModels(activeModels, offset);
	}

	if (m_InputClass->IsKeyDown(0x53)) //S
	{
		XMFLOAT3 offset = XMFLOAT3(0, -factor, 0);
		translateModels(activeModels, offset);
	}

	if (m_InputClass->IsKeyDown(0x41)) //A
	{
		XMFLOAT3 offset = XMFLOAT3(-factor, 0, 0);
		translateModels(activeModels, offset);
	}


	if (m_InputClass->IsKeyDown(0x44)) //D
	{
		XMFLOAT3 offset = XMFLOAT3(factor, 0, 0);
		translateModels(activeModels, offset);
	}

	if (m_InputClass->IsKeyDown(0x5A)) //Z
	{
		XMFLOAT3 offset = XMFLOAT3(0, 0, -factor);
		translateModels(activeModels, offset);
	}

	if (m_InputClass->IsKeyDown(0x58)) //X
	{
		XMFLOAT3 offset = XMFLOAT3(0, 0, factor);
		translateModels(activeModels, offset);
	}



	if (m_InputClass->IsKeyDown(VK_RETURN))
	{
		if (m_CanAdd)
		{
			m_CanAdd = false;
			AddModel();
		}
	}
	else
	{
		m_CanAdd = true;
	}

	if (radius == BigRadius)
	{
		if (m_InputClass->IsKeyDown(VK_LEFT))
		{
			Torus::decrementN_SEGMENTS_NUMBER();
			m_modelsManager.InitializeModels();
		}
		if (m_InputClass->IsKeyDown(VK_RIGHT))
		{
			Torus::incrementN_SEGMENTS_NUMBER();
			m_modelsManager.InitializeModels();
		}
	}

	if (radius == SmallRadius)
	{
		if (m_InputClass->IsKeyDown(VK_LEFT))
		{
			Torus::decrementN_CIRCLE_SEGMENTS_NUMBER();
			m_modelsManager.InitializeModels();
		}
		if (m_InputClass->IsKeyDown(VK_RIGHT))
		{
			Torus::incrementN_CIRCLE_SEGMENTS_NUMBER();
			m_modelsManager.InitializeModels();
		}	
	}

	//if (counter++ < 100){
	//	XMMATRIX* rotationMatrix = CreateYAxisRotationMatrix(rotation);
	//	rotate(m_elipsoidModel, *rotationMatrix);
	//}


	if (feature == Translation)
	{
		if (axis == AxisZ)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT3 offset = XMFLOAT3(0, 0, factor);
				translateModels(models, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT3 offset = XMFLOAT3(0, 0, -factor);
				//m_Torus->Translate(offset);
				translateModels(models, offset);
			}
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT3 offset = XMFLOAT3(0, factor, 0);
				translateModels(models, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT3 offset = XMFLOAT3(0, -factor, 0);
				translateModels(models, offset);
			}
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT3 offset = XMFLOAT3(factor, 0, 0);
				translateModels(models, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT3 offset = XMFLOAT3(-factor, 0, 0);
				translateModels(models, offset);
			}
		}
	}

	if (feature == Rotation)
	{
		if (axis == AxisZ)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(models, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(models, -rotation, axis);
			}
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(models, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(models, -rotation, axis);
			}
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(models, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(models, -rotation, axis);
			}
		}
	}

	if (feature == Scale)
	{
		if (m_InputClass->IsKeyDown(VK_UP)){
			scaleModels(models, (1 - factorScale));
		}
		if (m_InputClass->IsKeyDown(VK_DOWN)){
			scaleModels(models, (1 + factorScale));
		}
	}

}