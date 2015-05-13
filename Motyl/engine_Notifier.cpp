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
	m_ModelsManager->AddModel(ModelType::BezierSurfaceType);
}

void EngineNotifier::OnBSplinePatchAdded()
{
	m_ModelsManager->AddModel(ModelType::BSplinePatchType);
}

void EngineNotifier::OnBSplineSurfaceAdded()
{
	m_ModelsManager->AddModel(ModelType::BSplineSurfaceType);
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

void EngineNotifier::SetBSplinePatchPoints(int surfaceId, int patchId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	BSplineSurface* bsplineSurface = dynamic_cast<BSplineSurface*>(m_ModelsManager->GetModelById(surfaceId));
	BSplinePatch* bsplinePatch = dynamic_cast<BSplinePatch*>(m_ModelsManager->GetModelById(patchId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	bsplinePatch->SetNodes(simplePoints);
	bsplineSurface->AddPatch(bsplinePatch);
}

void EngineNotifier::SetDimensionsForBezierSurface(int surfaceId, int rows, int cols, int surfaceWidth, int surfaceHeigth)
{
	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(m_ModelsManager->GetModelById(surfaceId));
	bezierSurface->SetDimensions(rows, cols, surfaceWidth, surfaceHeigth);
}

void EngineNotifier::TranslateBezierSurfacePoints(int surfaceId, bool isSurfacePlane)
{
	BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(m_ModelsManager->GetModelById(surfaceId));
	if (isSurfacePlane)
		bezierSurface->TranslateSurfacePoints();
	else
		bezierSurface->TranslateCyllinderPoints();
}

void EngineNotifier::SetDimensionsForBSplineSurface(int surfaceId, int rows, int cols, int surfaceWidth, int surfaceHeigth)
{
	BSplineSurface* bsplineSurface = dynamic_cast<BSplineSurface*>(m_ModelsManager->GetModelById(surfaceId));
	bsplineSurface->SetDimensions(rows, cols, surfaceWidth, surfaceHeigth);
}

void EngineNotifier::TranslateBSplineSurfacePoints(int surfaceId, bool isSurfacePlane)
{
	BSplineSurface* bsplineSurface = dynamic_cast<BSplineSurface*>(m_ModelsManager->GetModelById(surfaceId));
	if (isSurfacePlane)
		bsplineSurface->TranslateSurfacePoints();
	else
		bsplineSurface->TranslateCyllinderPoints();
}
void EngineNotifier::RemoveModel(int id)
{
	m_ModelsManager->RemoveModel(id);
}