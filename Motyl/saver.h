#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "settingsHelper.h"
#include "modelclass.h"
#include "loader.h"

using namespace std;

class Saver{
public:
	Saver();
	void Initialize(SettingsHelper* settingsHelper, ModelsManager* modelsManager);
	void SaveScene(string pathToLoadFile);
private:
	void parseModel(FILE* myWritefile, ModelClass* model);
	void parseModelMatrix(FILE* myWritefile, XMMATRIX& m);
	void parseModelControlPoints(FILE* myReadfile,vector<SimplePoint*>& nodes);
	//void recreateModel(ModelParams* params);

	SettingsHelper* m_SettingsHelper;
	map<int, InsertionParams*> m_ListViewItemMapper;
	ModelsManager* modelsManager;
};