#pragma once
#include "modelsManager.h"

using namespace std;


class SceneService
{
public:

	SceneService();
	virtual ~SceneService();

	ModelsManager* ModelsManager;
};
