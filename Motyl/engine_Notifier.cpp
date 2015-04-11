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

void EngineNotifier::OnTorusAdded()
{
	m_ModelsManager->AddModel(ModelType::TorusType);
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

void EngineNotifier::RemoveModel(int id)
{
	m_ModelsManager->RemoveModel(id);
}