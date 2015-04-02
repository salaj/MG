#pragma once
#include "modelsManager.h"
#include "scene_service.h"

class EngineNotifier
{
public:
	EngineNotifier(SceneService*);
	void Initialize();

	void OnSimplePointAdded();
	void OnBezierCurveAdded();
	void OnTorusAdded();

	void SetActiveModels(vector<int>&);
	void SetBezierPoints(int, vector<int>&);

private:
	ModelsManager* m_ModelsManager;
};
