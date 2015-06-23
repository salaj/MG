#pragma once
#include "modelsManager.h"
#include "scene_service.h"

class EngineNotifier
{
public:
	EngineNotifier(SceneService*);
	void Initialize();

	ModelClass* OnSimplePointAdded();
	ModelClass* OnBezierCurveAdded();
	ModelClass* OnBezierC2CurveAdded();
	ModelClass* OnC2InterpolatedAdded();
	ModelClass* OnTorusAdded();
	ModelClass* OnBezierSurfaceAdded();
	ModelClass* OnBSplineSurfaceAdded();
	ModelClass* OnGregorySurfaceAdded();
	ModelClass* OnIntersectionCurveAdded();
	ModelClass* OnBezierPatchAdded();
	ModelClass* OnGregoryPatchAdded();
	ModelClass* OnBSplinePatchAdded();

	void SetActiveModels(vector<int>&);
	void SetC0CurvePoints(int, vector<int>&);
	void SetC2CurvePoints(int, vector<int>&);
	void SetC2InterpolatedCurvePoints(int, vector<int>&);
	void SetPatchPoints(int, int, vector<int>&);
	void SetGregoryPatchPoints(int, int, vector<int>&);
	void SetBSplinePatchPoints(int, int, vector<int>&);
	void SetDimensionsForBezierSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth);
	void TranslateBezierSurfacePoints(int, bool isSurfacePlane);
	void SetDimensionsForBSplineSurface(int surfaceId, int rows, int cols, double surfaceWidth, double surfaceHeigth);
	void TranslateBSplineSurfacePoints(int, bool isSurfacePlane);
	void TranslateGregoryPoints(int);

	//template <typename T>
	//void SetPoints<T>(int, vector<int>&);
	void RemoveModel(int id);
	ModelsManager* GetModelsManager();
private:
	ModelsManager* m_ModelsManager;
};
