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

void SceneHelper::Initialize(Service& service, GUIUpdater* guiUpdater)
{
	m_InputClass = service.InputClass;
	m_GUIUpdater = guiUpdater;
	m_modelsManager.Initialize(service);
}

SceneHelper::SceneHelper()
{
}

SceneHelper::~SceneHelper()
{
	delete m_InputClass;
}

ModelsManager* SceneHelper :: GetModelsManager()
{
	return &m_modelsManager;
}

map<int, ModelClass*>& SceneHelper::GetModels()
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

void SceneHelper::selectNewAndDeselectOldModel(ModelClass* model)
{
	vector<int> single = vector<int>();
	single.push_back(model->m_id);
	m_modelsManager.SetActiveModels(single);
}

void SceneHelper::deselectCurrentModel()
{
	vector<int> single = vector<int>();
	m_modelsManager.SetActiveModels(single);
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

	map<int, ModelClass*>& models = m_modelsManager.GetModels();
	float minSquareDistance = 0.02f;
	float minVal = FLT_MAX;
	int index = -1;
	for (map<int, ModelClass*> ::iterator it = ++(models.begin()); it != models.end(); it++)
	//for (int i = 1; i < models.size(); i++)//we omit cursor
	{
		ModelClass* model = (*it).second;// models[i];
		if (model->m_Type != ModelType::SimplePointType)
			continue;
		float val = ModelClass::GetSquareDistanceBetweenModels(&cursor, model);
		if (val < minVal)
		{
			index = (*it).first;
			minVal = val;
		}
	}
	if (minVal < minSquareDistance)
	{
		selectNewAndDeselectOldModel(models[index]);
	}
}

void SceneHelper::findClosestModelWithCursor()
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	map<int, ModelClass*>& models = m_modelsManager.GetModels();
	float minSquareDistance = 0.02f;
	float minVal = FLT_MAX;
	int index = -1;
	//for (int i = 1; i < models.size(); i++)//we omit cursor
	for (map<int, ModelClass*> ::iterator it = ++(models.begin()); it != models.end(); it++)
	{
		ModelClass* model = (*it).second;//models[i];
		if (model->m_Type != ModelType::SimplePointType)
			continue;
		float val = ModelClass::GetSquareDistanceBetweenModels(cursor, model);
		if (val < minVal)
		{
			index = (*it).first; 
			minVal = val;
		}
	}
	if (minVal < minSquareDistance)
	{
		selectNewAndDeselectOldModel(models[index]);
	}
}


void SceneHelper::translateModels(vector<ModelClass*>& models, XMFLOAT4 offset)
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	for (int i = 0; i < models.size(); i++)
	{
		XMFLOAT4 scale = ModelClass::GetRelativeScaleVector(cursor, models[i]);
		models[i]->Translate(XMFLOAT4(
			offset.x * scale.x,
			offset.y * scale.y,
			offset.z * scale.z,
			offset.w * scale.w
			));
	}
	translatePostActions(models);
}

void SceneHelper::translatePostActions(vector<ModelClass*>& models)
{
	vector<BezierCurve*> curves = m_modelsManager.GetBezierCurves();
	for (int i = 0; i < curves.size(); i++)
	{
		for (int j = 0; j < models.size(); j++)
		{
			if (models[j]->m_Type == ModelType::SimplePointType)
				curves[i]->UpdateNode(dynamic_cast<SimplePoint*>(models[j]));
		}
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
	XMFLOAT4 position = m_modelsManager.GetCursor()->GetTranslatedPosition(m_modelsManager.GetCursor());
	m_GUIUpdater->AddModel(position);
}

void SceneHelper::CheckInput()
{
	//map<int, ModelClass*>& models = m_modelsManager.GetModels();

	ModelClass* cursor = m_modelsManager.GetCursor();

	//vector<ModelClass*> activeModels;
	//activeModels.push_back(cursor);

	vector<ModelClass*>activeModels = m_modelsManager.GetActiveModels();
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
			if (m_modelsManager.selected == NULL)
			{
				findClosestModelWithCursor();
			}
			else
			{
				deselectCurrentModel();
			}
			m_CanSelect = false;
		}
	}
	else
	{
		m_CanSelect = true;
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

//void SceneHelper::CheckSelectedByTreeView()
//{
//	//int id = m_InputClass->GetSelectedModel();
//	//if (id != -1)
//	//{
//	//	selectNewAndDeselectOldModel(m_modelsManager.GetModelById(id));
//	//}
//}