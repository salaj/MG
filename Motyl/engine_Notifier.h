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
	void OnC2InterpolatedAdded();
	void OnTorusAdded();
	void OnBezierSurfaceAdded();
	void OnBezierPatchAdded();

	void SetActiveModels(vector<int>&);
	void SetC0CurvePoints(int, vector<int>&);
	void SetC2CurvePoints(int, vector<int>&);
	void SetC2InterpolatedCurvePoints(int, vector<int>&);
	void SetPatchPoints(int, int, vector<int>&);
	void SetDimensions(int surfaceId, int rows, int cols, int surfaceWidth, int surfaceHeigth);
	void TranslateSurfacePoints(int, bool isSurfacePlane);

	//template <typename T>
	//void SetPoints<T>(int, vector<int>&);
	void RemoveModel(int id);

private:
	ModelsManager* m_ModelsManager;
};
