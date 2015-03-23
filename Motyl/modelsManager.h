#pragma once

#include "applicationBase.h"
#include "modelclass.h"
#include "service.h"
#include <vector>
#include <xnamath.h>
#include "torus.h"
#include "elipsoid.h"
#include "simple_point .h"
#include "cursor.h"
#include "shader_torus.h"
#include "shader_elipsoid.h"
#include "shader_simple_point.h"
#include "shader_cursor.h"

using namespace std;

enum ModelType{
	TorusType,
	ElipsoidType,
	SimplePointType
};

class ModelsManager
{
public:

	ModelsManager();
	virtual ~ModelsManager();

	void Initialize(Service& service);
	static void* operator new(size_t size);
	static void operator delete(void* ptr);

	vector<ModelClass*>& GetModels();
	void CreateModels();
	void InitializeModels();
	void DrawModels();

	void SetModelAtOtherModelPosition(ModelClass *model, ModelClass *relative_model);
	void AddModel(ModelType);

	void setModelToPosition(ModelClass*, XMFLOAT3);

private:
	vector<ModelClass*> m_models;
	Service m_service;

	ModelClass* getCursorModel();
};
