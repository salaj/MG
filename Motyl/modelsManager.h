#pragma once

#include "applicationBase.h"
#include "modelclass.h"
#include "service.h"
#include <vector>
#include <xnamath.h>
#include "torus.h"
#include "elipsoid.h"
#include "shader_torus.h"
#include "shader_elipsoid.h"

using namespace std;

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

private:
	vector<ModelClass*> m_models;
	Service m_service;
};
