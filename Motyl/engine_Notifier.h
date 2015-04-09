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
	void OnBezierC2CurveAdded();
	void OnTorusAdded();

	void SetActiveModels(vector<int>&);
	void SetC0CurvePoints(int, vector<int>&);
	void SetC2CurvePoints(int, vector<int>&);
	void RemoveModel(int id);

private:
	ModelsManager* m_ModelsManager;
};
