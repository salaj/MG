#include "engine_Notifier.h"

using namespace std;

EngineNotifier::EngineNotifier(SceneService* service)
{
	m_ModelsManager = service->ModelsManager;
}


void EngineNotifier::Initialize()
{
	
}

ModelClass* EngineNotifier::OnSimplePointAdded()
{
	return m_ModelsManager->AddModel(ModelType::SimplePointType);
}

ModelClass* EngineNotifier::OnBezierCurveAdded()
{
	return m_ModelsManager->AddModel(ModelType::BezierType);
}

ModelClass* EngineNotifier::OnBezierC2CurveAdded()
{
	return m_ModelsManager->AddModel(ModelType::BezierC2Type);
}

ModelClass* EngineNotifier::OnC2InterpolatedAdded()
{
	return m_ModelsManager->AddModel(ModelType::InterpolatedC2Type);
}

ModelClass* EngineNotifier::OnTorusAdded()
{
	return m_ModelsManager->AddModel(ModelType::TorusType);
}

ModelClass* EngineNotifier::OnGregoryPatchAdded()
{
	return m_ModelsManager->AddModel(ModelType::GregoryPatchType);
}

ModelClass* EngineNotifier::OnBezierPatchAdded()
{
	return m_ModelsManager->AddModel(ModelType::BezierPatchType);
}

ModelClass* EngineNotifier::OnBezierSurfaceAdded()
{
	return m_ModelsManager->AddModel(ModelType::BezierSurfaceType);
}

ModelClass* EngineNotifier::OnBSplinePatchAdded()
{
	return m_ModelsManager->AddModel(ModelType::BSplinePatchType);
}

ModelClass* EngineNotifier::OnBSplineSurfaceAdded()
{
	return m_ModelsManager->AddModel(ModelType::BSplineSurfaceType);
}

ModelClass* EngineNotifier::OnGregorySurfaceAdded()
{
	return m_ModelsManager->AddModel(ModelType::GregorySurfaceType);
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

void EngineNotifier::SetGregoryPatchPoints(int surfaceId, int patchId, vector<int>& ids)
{
	if (ids.size() == 0)
		return;
	GregorySurface* gregorySurface = dynamic_cast<GregorySurface*>(m_ModelsManager->GetModelById(surfaceId));
	GregoryPatch* gregoryPatch = dynamic_cast<GregoryPatch*>(m_ModelsManager->GetModelById(patchId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	gregoryPatch->SetNodes(simplePoints);
	gregorySurface->AddPatch(gregoryPatch);
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

void EngineNotifier::SetDimensionsForBezierSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth)
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

void EngineNotifier::SetDimensionsForBSplineSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth)
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

void EngineNotifier::TranslateGregoryPoints(int surfaceId)
{
	GregorySurface* gregorySurface = dynamic_cast<GregorySurface*>(m_ModelsManager->GetModelById(surfaceId));
	gregorySurface->SetBezierSurfaces(m_ModelsManager->GetBezierSurfaces());
	gregorySurface->TranslatePoints(TranslatedBezier::right);
}

void EngineNotifier::RemoveModel(int id)
{
	m_ModelsManager->RemoveModel(id);
}

ModelsManager* EngineNotifier::GetModelsManager()
{
	return m_ModelsManager;
}