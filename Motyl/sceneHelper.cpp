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
	selected = NULL;
	m_InputClass = service.InputClass;
	m_GUIUpdater = service.GUIUpdater;
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

void SceneHelper::selectNewModel(ModelClass* model)
{
	float scaleFactor = 2.0f;
	if (selected != NULL)
	{
		selected->Scale(1 / scaleFactor);
		selected->m_selected = false;
		m_modelsManager.RemoveActiveModel(selected->m_id);
	}
	model->Scale(scaleFactor);
	model->m_selected = true;
	selected = model;
	m_modelsManager.AddActiveModel(model->m_id);
}

XMFLOAT2 translatePoint(POINT mousePosition)
{
	XMFLOAT2 ret;
	mousePosition.x = mousePosition.x - 300.0f;
	mousePosition.y = 300.0f - mousePosition.y;
	ret.x = (float)mousePosition.x / 300.0f;
	ret.y = (float)mousePosition.y / 300.0f;
	return ret;
}

void SceneHelper::findClosestModelWithMouse(POINT mousePosition)
{
	XMFLOAT2 translatedMousePosition = translatePoint(mousePosition);
	Cursor cursor = Cursor(); //tmp cursor
	ModelsManager::setModelToPosition(dynamic_cast<ModelClass*>(&cursor),
		XMFLOAT3(translatedMousePosition.x, translatedMousePosition.y, 0));

	vector<ModelClass*>& models = m_modelsManager.GetModels();
	float minSquareDistance = 0.02f;
	float minVal = FLT_MAX;
	int index = -1;
	for (int i = 1; i < models.size(); i++)//we omit cursor
	{
		ModelClass* model = models[i];
		if (model->m_Type != ModelType::SimplePointType)
			continue;
		float val = ModelClass::GetSquareDistanceBetweenModels(&cursor, model);
		if (val < minVal)
		{
			index = i;
			minVal = val;
		}
	}
	if (minVal < minSquareDistance)
	{
		selectNewModel(models[index]);
	}
}

void SceneHelper::findClosestModelWithCursor()
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	vector<ModelClass*>& models = m_modelsManager.GetModels();
	float minSquareDistance = 0.02f;
	float minVal = FLT_MAX;
	int index = -1;
	for (int i = 1; i < models.size(); i++)//we omit cursor
	{
		ModelClass* model = models[i];
		if (model->m_Type != ModelType::SimplePointType)
			continue;
		float val = ModelClass::GetSquareDistanceBetweenModels(cursor, model);
		if (val < minVal)
		{
			index = i; 
			minVal = val;
		}
	}
	if (minVal < minSquareDistance)
	{
		selectNewModel(models[index]);
	}
}


void SceneHelper::translateModels(vector<ModelClass*>& models, XMFLOAT4 offset)
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

void SceneHelper::AddModel(ModelType type)
{
	ModelClass* createdModel = m_modelsManager.AddModel(type);
	if (createdModel != NULL)
	{
		if (createdModel->m_Type != ModelType::CursorType)
			selectNewModel(createdModel);
		//set position on gui side
		XMFLOAT4 position = m_modelsManager.GetModels()[0]->GetTranslatedPosition(m_modelsManager.GetCursor());
		m_GUIUpdater->AddModel(position);
	}
}

void SceneHelper::RemoveModel(ModelClass* model)
{
	m_modelsManager.RemoveModel(model->m_id);
	m_GUIUpdater->RemoveModels(model->m_id);
}

void SceneHelper::CheckInput()
{
	vector<ModelClass*>& models = m_modelsManager.GetModels();

	ModelClass* cursor = m_modelsManager.GetCursor();

	vector<ModelClass*> activeModels;
	activeModels.push_back(cursor);

	ActiveFeature feature = m_InputClass->getActiveFeature();
	ActiveAxis axis = m_InputClass->getActiveAxis();
	ActiveRadius radius = m_InputClass->getActiveRadius();
	float rotation = (float)XM_PI * 0.0002f;
	float factor = 0.005f;
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

	if (m_InputClass->IsKeyDown(VK_SPACE))
	{
		if (m_CanSelect)
		{
			findClosestModelWithCursor();
			m_CanSelect = false;
		}
	}
	else
	{
		m_CanSelect = true;
	}

	if (m_InputClass->IsKeyDown(VK_DELETE))
	{
		if (m_CanDelete)
		{
			if (selected != NULL)
			{
				m_CanDelete = false;
				RemoveModel(selected);
				vector<ModelClass*> newModels = m_modelsManager.GetModels();
				ModelClass* newModel = newModels.size() > 1 ? newModels[1] : NULL;
				if (newModel != NULL)
				{
					selectNewModel(newModel);
				}
			}
		}
	}
	else
	{
		m_CanDelete = true;
	}


	if (m_InputClass->IsKeyDown(0x57)) //W
	{
		XMFLOAT4 offset = XMFLOAT4(0, factor, 0, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}

	if (m_InputClass->IsKeyDown(0x53)) //S
	{
		XMFLOAT4 offset = XMFLOAT4(0, -factor, 0, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}

	if (m_InputClass->IsKeyDown(0x41)) //A
	{
		XMFLOAT4 offset = XMFLOAT4(-factor, 0, 0, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}


	if (m_InputClass->IsKeyDown(0x44)) //D
	{
		XMFLOAT4 offset = XMFLOAT4(factor, 0, 0, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}

	if (m_InputClass->IsKeyDown(0x5A)) //Z
	{
		XMFLOAT4 offset = XMFLOAT4(0, 0, -factor, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}

	if (m_InputClass->IsKeyDown(0x58)) //X
	{
		XMFLOAT4 offset = XMFLOAT4(0, 0, factor, 0);
		translateModels(activeModels, offset);
		m_GUIUpdater->setCursorPositionWorld(m_modelsManager.GetCursor()->GetPosition());
		m_GUIUpdater->setCursorPositionScreen(m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor()));
	}

	activeModels = m_modelsManager.GetActiveModels();


	if (m_InputClass->IsKeyDown(VK_RETURN))
	{
		if (m_CanAdd)
		{
			m_CanAdd = false;
			ModelType type = ModelType::SimplePointType;
			AddModel(type);
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
				XMFLOAT4 offset = XMFLOAT4(0, 0, factor, 0);
				translateModels(activeModels, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(0, 0, -factor, 0);
				//m_Torus->Translate(offset);
				translateModels(activeModels, offset);
			}
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT4 offset = XMFLOAT4(0, factor, 0, 0);
				translateModels(activeModels, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(0, -factor, 0, 0);
				translateModels(activeModels, offset);
			}
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT4 offset = XMFLOAT4(factor, 0, 0, 0);
				translateModels(activeModels, offset);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(-factor, 0, 0, 0);
				translateModels(activeModels, offset);
			}
		}
	}

	if (feature == Rotation)
	{
		if (axis == AxisZ)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
			}
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
			}
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
			}
		}
	}

	if (feature == Scale)
	{
		if (m_InputClass->IsKeyDown(VK_UP)){
			scaleModels(activeModels, (1 - factorScale));
		}
		if (m_InputClass->IsKeyDown(VK_DOWN)){
			scaleModels(activeModels, (1 + factorScale));
		}
	}
}

void SceneHelper::CheckMouse()
{
	if (m_InputClass->IsMouseButtonDown())
	{
		POINT position = m_InputClass->GetMousePosition();
		if (m_previousPoint.x == position.x && m_previousPoint.y == position.y)
			return;
		m_previousPoint = position;
		findClosestModelWithMouse(position);
	}
}

void SceneHelper::CheckSelectedByTreeView()
{
	int id = m_InputClass->GetSelectedModel();
	if (id != -1)
	{
		selectNewModel(m_modelsManager.GetModelById(id));
	}
}