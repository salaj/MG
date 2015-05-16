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
	void OnBSplineSurfaceAdded();
	void OnBezierPatchAdded();
	void OnBSplinePatchAdded();

	void SetActiveModels(vector<int>&);
	void SetC0CurvePoints(int, vector<int>&);
	void SetC2CurvePoints(int, vector<int>&);
	void SetC2InterpolatedCurvePoints(int, vector<int>&);
	void SetPatchPoints(int, int, vector<int>&);
	void SetBSplinePatchPoints(int, int, vector<int>&);
	void SetDimensionsForBezierSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth);
	void TranslateBezierSurfacePoints(int, bool isSurfacePlane);
	void SetDimensionsForBSplineSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth);
	void TranslateBSplineSurfacePoints(int, bool isSurfacePlane);

	//template <typename T>
	//void SetPoints<T>(int, vector<int>&);
	void RemoveModel(int id);

private:
	ModelsManager* m_ModelsManager;
};
