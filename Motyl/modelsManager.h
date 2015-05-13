#pragma once

#include "applicationBase.h"
#include "modelclass.h"
#include "service.h"
#include <vector>
#include <map>
#include <xnamath.h>
#include "torus.h"
#include "elipsoid.h"
#include "simple_point .h"
#include "cursor.h"
#include "bezier_curve.h"
#include "bezierC2_curve.h"
#include "bezierPatch.h"
#include "bezierSurface.h"
#include "bsplinePatch.h"
#include "bsplineSurface.h"
#include "interpolatedC2_curve.h"
#include "shader_torus.h"
#include "shader_elipsoid.h"
#include "shader_simple_point.h"
#include "shader_cursor.h"
#include "shader_bezier_curve.h"
#include "shader_bspline_patch.h"

using namespace std;


class ModelsManager
{
public:
	ModelsManager();
	virtual ~ModelsManager();

	void Initialize(Service& service);
	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	map<int, ModelClass*>& GetModels();
	vector<ModelClass*>& GetActiveModels();
	vector<BezierCurve*>& GetBezierCurves();
	vector<BezierSurface*>& GetBezierSurfaces();
	vector<BSplineSurface*>& GetBSplineSurfaces();
	void CreateModels();
	void SetActiveModels(vector<int>&);
	void AddActiveModel(int);
	void AddBezierCurve(BezierCurve*);
	void AddBezierSurface(BezierSurface*);
	void AddBSplineSurface(BSplineSurface*);
	void RemoveActiveModel(int);
	void RemoveBezierCurve(int);
	void InitializeModels();
	void DrawModels();
	ModelClass* GetModelById(int);

	void SetModelAtOtherModelPosition(ModelClass *model, ModelClass *relative_model);
	void CopyOtherModelAfinityMatrix(ModelClass *model, ModelClass *relative_model);
	ModelClass* AddModel(ModelType);
	void RemoveModel(int);

	static void setModelToPosition(ModelClass*, XMFLOAT3);

	ModelClass* GetCursor();
	ModelClass* selected = NULL;
private:
	map<int, ModelClass*> m_models;
	map<int, ModelClass*> m_active_models;
	map<int, BezierCurve*> m_bezier_curves;
	map<int, BezierSurface*> m_bezier_surfaces;
	map<int, BSplineSurface*> m_bspline_surfaces;
	Service m_service;

	void createFakeC2Curve();
	void createFakeInterpolatedC2Curve();
	void createFakeSurfaceC0();
	void createFakeSurfaceC2();
};
