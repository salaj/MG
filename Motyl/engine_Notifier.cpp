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

void EngineNotifier::OnTorusAdded()
{
	m_ModelsManager->AddModel(ModelType::TorusType);
}

void EngineNotifier::SetActiveModels(vector<int>& activeModels)
{
	m_ModelsManager->SetActiveModels(activeModels);
}

void EngineNotifier::SetBezierPoints(int curveId, vector<int>& ids)
{
	BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>( m_ModelsManager->GetModelById(curveId));
	vector<ModelClass*> simplePoints = vector<ModelClass*>();
	for (int i = 0; i < ids.size(); i++)
	{
		simplePoints.push_back(m_ModelsManager->GetModelById(ids[i]));
	}
	bezierCurve->SetNodes(simplePoints);
}