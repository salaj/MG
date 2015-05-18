#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "settingsHelper.h"
#include "modelclass.h"

using namespace std;

enum ParseType{
	ParsePoint,
	ParseCurve,
	ParseC2Curve,
	ParseC2Interpolated,
	ParseTorus,
	ParseRoot,
	ParsePatch,
	ParseBSplinePatch,
	ParseBezierSurface,
	ParseBSplineSurface
};

class ModelParams{
public:
	ModelParams();
	~ModelParams()
	{
		delete[] name;
		delete[] color;
		delete controlPoints;
	}
	ModelParams(ParseType parseType, int id, char* name, float& Width, float& Height, int& PatchesXCount, int& PatchesYCount, float& R, float& r, float& X, float& Y, float& Z, XMMATRIX& modelMatrix, char* color, vector<int>* controlPoints)
	{
		this->parseType = parseType;
		this->id = id;
		this->name = name;
		this->Width = Width;
		this->Height = Height;
		this->PatchesXCount = PatchesXCount;
		this->PatchesYCount = PatchesYCount;
		this->R = R;
		this->r = r;
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->modelMatrix = modelMatrix;
		this->color = color;
		this->controlPoints = controlPoints;
	}
	static void* operator new(size_t size)
	{
		return Utils::New16Aligned(size);
	}
		static void operator delete(void* ptr)
	{
		Utils::Delete16Aligned(ptr);
	}
//private:

	ParseType parseType;

	int id;
	const char* name;
	float Width, Height;
	int PatchesXCount, PatchesYCount;
	float R;
	float r;
	float X;
	float Y;
	float Z;
	XMMATRIX modelMatrix;
	const char* color;
	vector<int>* controlPoints;
};

class Loader{
public:
	Loader();
	void Initialize(SettingsHelper* settingsHelper);
	void LoadScene(string pathToLoadFile);
private:
	ModelParams* parseModel(FILE* myReadfile, ParseType parseType);
	XMMATRIX& parseModelMatrix(FILE* myReadfile);
	vector<int>* parseModelControlPoints(FILE* myReadfile);
	void recreateModel(ModelParams* params);

	SettingsHelper* m_SettingsHelper;
	map<int, InsertionParams*> m_ListViewItemMapper;
};