#include "engine_Notifier.h"

using namespace std;

EngineNotifier::EngineNotifier(SceneService* service)
{
	m_ModelsManager = service->ModelsManager;
}


void EngineNotifier::Initialize()
{
	
}

void EngineNotifier::OnSimplePointAdded()
{
	m_ModelsManager->AddModel(ModelType::SimplePointType);
}

void EngineNotifier::OnBezierCurveAdded()
{
	m_ModelsManager->AddModel(ModelType::BezierType);
}

void EngineNotifier::OnBezierC2CurveAdded()
{
	m_ModelsManager->AddModel(ModelType::BezierC2Type);
}

void EngineNotifier::OnC2InterpolatedAdded()
{
	m_ModelsManager->AddModel(ModelType::InterpolatedC2Type);
}

void EngineNotifier::OnTorusAdded()
{
	m_ModelsManager->AddModel(ModelType::TorusType);
}

void EngineNotifier::OnBezierPatchAdded()
{
	m_ModelsManager->AddModel(ModelType::BezierPatchType);
}

void EngineNotifier::OnBezierSurfaceAdded()
{
	//int numberOfRows, numberOfColumns;
	//numberOfRows = 2;
	//numberOfColumns = 3;
	//int numberOfPatches = numberOfRows * numberOfColumns;
	//int numberOfControlPoints = numberOfPatches * 16 - (
	//	numberOfRows * (numberOfColumns - 1) * 4 +
	//	numberOfColumns * (numberOfRows - 1) * 4);
	//vector<BezierPatch*> bezierPatches;
	//vector<SimplePoint*> controlPoints;
	//for (int i = 0; i < numberOfControlPoints; i++)
	//{
	//	ModelClass* model = m_ModelsManager->AddModel(ModelType::SimplePointType);
	//	controlPoints.push_back(dynamic_cast<SimplePoint*>(model));
	//}
	//for (int i = 0; i < numberOfPatches; i++)
	//{
	//	ModelClass* model = m_ModelsManager->AddModel(ModelType::BezierPatchType);
	//	bezierPatches.push_back(dynamic_cast<BezierPatch*>(model));
	//}
	ModelClass* model = m_ModelsManager->AddModel(ModelType::BezierSurfaceType);

	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(model);
}

void EngineNotifier::SetActiveModels(vector<int>& activeModels)
{
	m_ModelsManager->SetActiveModels(activeModels);
}

void EngineNotifier::SetC0CurvePoints(int curveId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>( m_ModelsManager->GetModelById(curveId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	bezierCurve->SetNodes(simplePoints);
}

void EngineNotifier::SetC2CurvePoints(int curveId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	BezierC2Curve* bezierCurve = dynamic_cast<BezierC2Curve*>(m_ModelsManager->GetModelById(curveId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	bezierCurve->SetNodes(simplePoints);
}

void EngineNotifier::SetC2InterpolatedCurvePoints(int curveId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	InterpolatedC2Curve* curve = dynamic_cast<InterpolatedC2Curve*>(m_ModelsManager->GetModelById(curveId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	curve->SetNodes(simplePoints);
}

void EngineNotifier::SetPatchPoints(int surfaceId, int patchId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(m_ModelsManager->GetModelById(surfaceId));
	BezierPatch* bezierPatch = dynamic_cast<BezierPatch*>(m_ModelsManager->GetModelById(patchId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	bezierPatch->SetNodes(simplePoints);
	bezierSurface->AddPatch(bezierPatch);
}

void EngineNotifier::SetDimensions(int surfaceId, int rows, int cols, int surfaceWidth, int surfaceHeigth)
{
	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(m_ModelsManager->GetModelById(surfaceId));
	bezierSurface->SetDimensions(rows, cols, surfaceWidth, surfaceHeigth);
}

void EngineNotifier::TranslateSurfacePoints(int surfaceId, bool isSurfacePlane)
{
	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(m_ModelsManager->GetModelById(surfaceId));
	if (isSurfacePlane)
		bezierSurface->TranslateSurfacePoints();
	else
		bezierSurface->TranslateCyllinderPoints();
}


//
//template<typename T>
//void EngineNotifier::SetPoints(int, vector<int>&)
//{
//	if (ids.size() == 0)
//		return;
//	T* curve = dynamic_cast<T*>(m_ModelsManager->GetModelById(curveId));
//	vector<ModelClass*> simplePoints = vector<ModelClass*>();
//	for (int i = 0; i < ids.size(); i++)
//	{
//		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
//	}
//	curve->SetNodes(simplePoints);
//}

void EngineNotifier::RemoveModel(int id)
{
	m_ModelsManager->RemoveModel(id);
}