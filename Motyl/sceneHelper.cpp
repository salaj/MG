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
	lastTime = (double)clock() / CLOCKS_PER_SEC;
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
	refreshTime();
	m_modelsManager.DrawModels();
}

void SceneHelper::selectNewAndDeselectOldModel(ModelClass* model)
{
	vector<int> single = vector<int>();
	single.push_back(model->m_id);
	if (m_activeVirtualModel != nullptr)
		m_activeVirtualModel->ScaleDown();
	if (model->m_isGenuine)
	{
		m_activeVirtualModel = nullptr;
		m_modelsManager.SetActiveModels(single);
	}
	else
	{
		deselectCurrentModel();
		m_activeVirtualModel = model;
		m_activeVirtualModel->ScaleUp();
		ModelClass* cursor = m_modelsManager.GetCursor();
		cursor->SetPosition(m_activeVirtualModel->GetPosition());
	}
}

void SceneHelper::deselectCurrentModel()
{
	vector<int> single = vector<int>();
	m_modelsManager.SetActiveModels(single);
	mirroredModel = nullptr;
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

void SceneHelper::findClosestWithMouse(XMVECTOR orig, XMVECTOR dir)
{
	float minSquareDistance = 0.2f;
	float minVal = FLT_MAX;
	int index = -1;
	map<int, ModelClass*>& models = m_modelsManager.GetModels();
	for (int k = 0; k < 1000; k++)
	{
		XMVECTOR res = orig + dir * k;
		XMFLOAT3 clickPosition = XMFLOAT3(XMVectorGetX(res), XMVectorGetY(res), XMVectorGetZ(res));
		Cursor cursor = Cursor(); //tmp cursor
		ModelsManager::setModelToPosition(dynamic_cast<ModelClass*>(&cursor), clickPosition);

		for (map<int, ModelClass*> ::iterator it = (models.begin()); it != models.end(); it++)
		{
			ModelClass* model = (*it).second;
			if (model->m_Type != ModelType::SimplePointType)
				continue;
			float val = ModelClass::GetSquareDistanceBetweenModels(&cursor, model);
			if (val < minVal)
			{
				index = (*it).first;
				minVal = val;
			}
		}
	}
	if (minVal < minSquareDistance)
	{
		selectNewAndDeselectOldModel(models[index]);
		//delete &models;
	}
}

void SceneHelper::findClosestModelWithMouse(POINT mousePosition)
{
	//return;
	XMFLOAT2 translatedMousePosition = translatePoint(mousePosition);
	Cursor cursor = Cursor(); //tmp cursor
	int numberOfSamples = 50;
	for (int i = 0; i < numberOfSamples; i++)
	{
		ModelsManager::setModelToPosition(dynamic_cast<ModelClass*>(&cursor),
			XMFLOAT3(translatedMousePosition.x, translatedMousePosition.y, -1.0f + 2.0f * ((float)i / (float)numberOfSamples)));

		map<int, ModelClass*>& models = m_modelsManager.GetModels();
		float minSquareDistance = 0.02f;
		float minVal = FLT_MAX;
		int index = -1;
		for (map<int, ModelClass*> ::iterator it = (models.begin()); it != models.end(); it++)
 		{
			ModelClass* model = (*it).second;
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
			break;
		}
	}
}

ModelClass* SceneHelper::findMirroredModel(map<int, ModelClass*>& models, ModelClass* relativeTo)
{
	Cursor cursor = Cursor(); //tmp cursor
	ModelsManager::setModelToPosition(dynamic_cast<ModelClass*>(&cursor), relativeTo->GetPosition3());
	cursor.m_modelMatrix(3, 2) *= -1; //mirror in z;
	float minVal = FLT_MAX;
	int index = -1;
	float minSquareDistance = 0.02f;
	for (map<int, ModelClass*> ::iterator it = (models.begin()); it != models.end(); it++)
	{
		ModelClass* model = (*it).second;
		float val = ModelClass::GetSquareDistanceBetweenModels(&cursor, model);
		if (val < minVal)
		{
			index = (*it).first;
			minVal = val;
		}
	}
	if (minVal < minSquareDistance)
	{
		return models[index];
	}
	else
		return nullptr;
}

void SceneHelper::findClosestModelWithCursor()
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	map<int, ModelClass*>& models = m_modelsManager.GetModels();
	float minSquareDistance = 0.02f;
	float minVal = FLT_MAX;
	int index = -1;
	for (map<int, ModelClass*> ::iterator it = ++(models.begin()); it != models.end(); it++)
	{
		ModelClass* model = (*it).second;
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
		mirroredModel = findMirroredModel(models, models[index]);
		if (mirroredModel == models[index])
			mirroredModel = nullptr;
		selectNewAndDeselectOldModel(models[index]);
	}
}

void SceneHelper::refreshTime()
{
	clock_t t;
	t = clock();
	double currentTime = (double)t / CLOCKS_PER_SEC;
	deltaTime += currentTime - lastTime;
	double copyDelta = deltaTime;
	if (deltaTime > refreshRate)
	{
		shallBeRefreshed = true;
		deltaTime = 0;
	}
	if (copyDelta > refreshRateLong)
	{
		shallBeRefreshedLong = true;
	}
	lastTime = currentTime;
}


void SceneHelper::translateModels(vector<ModelClass*>& models, XMFLOAT4 offset, bool forceRefresh = true)
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Translate(offset);
	}
	if (mirroredModel != nullptr)
	{
		mirroredModel->Translate(XMFLOAT4(offset.x, offset.y, -offset.z, offset.w));
	}
	if (forceRefresh && shallBeRefreshed)
	{
		translatePostActions(models);
		shallBeRefreshed = false;
	}
}

void SceneHelper::translatePostActions(vector<ModelClass*>& models)
{
	vector<BezierCurve*> curves = m_modelsManager.GetBezierCurves();
	for (int i = 0; i < curves.size(); i++)
	{
		for (int j = 0; j < models.size(); j++)
		{
			if (models[j]->m_Type == ModelType::SimplePointType)
			{
				models[j]->ScaleDown();
				curves[i]->UpdateNode(dynamic_cast<SimplePoint*>(models[j]));
				models[j]->ScaleUp();
			}
		}
	}

	SimplePoint* mirroredPoint = nullptr;
	if (mirroredModel != nullptr)
		mirroredPoint = dynamic_cast<SimplePoint*>(mirroredModel);



	vector<BezierSurface*> bezierSurfaces = m_modelsManager.GetBezierSurfaces();
	for (int i = 0; i < bezierSurfaces.size(); i++)
	{
		for (int j = 0; j < models.size(); j++)
		{
			if (models[j]->m_Type == ModelType::SimplePointType)
			{
				bezierSurfaces[i]->UpdateNode(dynamic_cast<SimplePoint*>(models[j]));
			}
		}
	}

	vector<BSplineSurface*> bsplineSurfaces = m_modelsManager.GetBSplineSurfaces();
	for (int i = 0; i < bsplineSurfaces.size(); i++)
	{
		for (int j = 0; j < models.size(); j++)
		{
			if (models[j]->m_Type == ModelType::SimplePointType)
			{
				bsplineSurfaces[i]->UpdateNode(dynamic_cast<SimplePoint*>(models[j]));
				if (mirroredPoint != nullptr)
					bsplineSurfaces[i]->UpdateNode(mirroredPoint);
			}
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

void SceneHelper::toggleRotation(bool rotationUp)
{
	if (!isRotationPending)
	{
		isRotationPending = true;
		isRotationUp = rotationUp;
	}
	else
	{
		if (isRotationUp != rotationUp)
		{
			isRotationPending = false;
			translatePostActions(m_modelsManager.GetActiveModels());
		}
	}
}

void SceneHelper::toggleTranslation(bool translationUp)
{
	if (!isTranslationPending)
	{
		isTranslationPending = true;
		isTranslationUp = translationUp;
	}
	else
	{
		if (isTranslationUp != translationUp)
		{
			isTranslationPending = false;
			translatePostActions(m_modelsManager.GetActiveModels());
		}
	}
}
void SceneHelper::CheckInput()
{
	ModelClass* cursor = m_modelsManager.GetCursor();
	vector<ModelClass*>activeModels;
	if (m_activeVirtualModel != nullptr)
		activeModels.push_back(m_activeVirtualModel);
	else
		activeModels = m_modelsManager.GetActiveModels();

	activeModels.push_back(cursor);

	ActiveFeature feature = m_InputClass->getActiveFeature();
	ActiveAxis axis = m_InputClass->getActiveAxis();
	ActiveRadius radius = m_InputClass->getActiveRadius();
	float rotation = (float)XM_PI * 0.0002f;
	float factor = 0.001f;
	float factorScale = factor;

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

	if (feature == Translation)
	{
		if (axis == AxisZ)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT4 offset = XMFLOAT4(0, 0, factor, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(0, 0, -factor, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (isTranslationPending)
				toggleTranslation(false);
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT4 offset = XMFLOAT4(0, factor, 0, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(0, -factor, 0, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (isTranslationPending)
				toggleTranslation(false);
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				XMFLOAT4 offset = XMFLOAT4(factor, 0, 0, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				XMFLOAT4 offset = XMFLOAT4(-factor, 0, 0, 0);
				translateModels(activeModels, offset, false);
				toggleTranslation(true);
				return;
			}
			if (isTranslationPending)
				toggleTranslation(false);
		}
	}

	if (feature == Rotation)
	{
		if (axis == AxisZ)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
				toggleRotation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
				toggleRotation(true);
				return;
			}
			if (isRotationPending)
				toggleRotation(false);
		}
		else if (axis == AxisY)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
				toggleRotation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
				toggleRotation(true);
				return;
			}
			if (isRotationPending)
				toggleRotation(false);
		}
		else if (axis == AxisX)
		{
			if (m_InputClass->IsKeyDown(VK_UP)){
				rotateModels(activeModels, rotation, axis);
				toggleRotation(true);
				return;
			}
			if (m_InputClass->IsKeyDown(VK_DOWN)){
				rotateModels(activeModels, -rotation, axis);
				toggleRotation(true);
				return;
			}
			if (isRotationPending)
				toggleRotation(false);
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

void SceneHelper::redrawCurves()
{
	vector<BezierCurve*>& curves = m_modelsManager.GetBezierCurves();
	for (int i = 0; i < curves.size(); i++)
	{
		if (curves[i]->m_Type == ModelType::BezierC2Type)
		{
			curves[i]->Reset();
		}
	}
}

void SceneHelper::RefreshSpaces()
{
	int verticalSpaces = m_InputClass->GetVerticalSpaces();
	int horizontalSpaces = m_InputClass->GetHorizontalSpaces();
	map<int, ModelClass*> models = m_modelsManager.GetModels();
	for (map<int, ModelClass*> ::iterator it = models.begin(); it != models.end(); it++)
	{
		if ((*it).second->m_Type == ModelType::BezierPatchType)
		{
			BezierPatch* bezierPatch = dynamic_cast<BezierPatch*>((*it).second);
			if (bezierPatch->verticalSpaces != verticalSpaces)
			{
				bezierPatch->verticalSpaces = verticalSpaces;
				bezierPatch->Reset();
			}
			if (bezierPatch->horizontalSpaces != horizontalSpaces)
			{
				bezierPatch->horizontalSpaces = horizontalSpaces;
				bezierPatch->Reset();
			}
		}
		else if ((*it).second->m_Type == ModelType::BSplinePatchType)
		{
			BSplinePatch* bsplinePatch = dynamic_cast<BSplinePatch*>((*it).second);
			if (bsplinePatch->verticalSpaces != verticalSpaces)
			{
				bsplinePatch->verticalSpaces = verticalSpaces;
				bsplinePatch->Reset();
			}
			if (bsplinePatch->horizontalSpaces != horizontalSpaces)
			{
				bsplinePatch->horizontalSpaces = horizontalSpaces;
				bsplinePatch->Reset();
			}
		}
	}
}

void SceneHelper::IsBaseChanged()
{
	bool isBernstein = m_InputClass->isBernsteinBaseSet;
	if (isBernstein)
	{
		if (m_base == CurveBase::BSpline)
		{
			m_base = CurveBase::Bezier;
			BezierC2Curve::m_base = m_base;
			//notify to redraw
			redrawCurves();
		}
	}
	else
	{
		if (m_base == CurveBase::Bezier)
		{
			m_base = CurveBase::BSpline;
			BezierC2Curve::m_base = m_base;
			//notify to redraw
			redrawCurves();
		}
	}
}