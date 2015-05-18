#include "loader.h"

Loader::Loader()
{

}

void Loader::Initialize(SettingsHelper* settingsHelper)
{
	m_SettingsHelper = settingsHelper;
}

void Loader::LoadScene(string pathToLoadFile)
{
	FILE* myReadfile;
	myReadfile = fopen(pathToLoadFile.c_str(), "r");
	string line = string();
	char last[255];
	while (fscanf(myReadfile, "%s\n", &line) != EOF)
	{
		ModelParams* params = nullptr;
		if (strcmp(line.c_str(), "Torus")== 0)
		{
			params = parseModel(myReadfile, ParseType::ParseTorus);
		}
		else if (strcmp(line.c_str(), "Point") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParsePoint);
		}
		else if (strcmp(line.c_str(), "BezierCurveC0") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParseCurve);
		}
		else if (strcmp(line.c_str(), "InterpolationCurve") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParseC2Interpolated);
		}
		else if (strcmp(line.c_str(), "BezierCurveC2") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParseC2Curve);
		}
		else if (strcmp(line.c_str(), "BezierSurfaceC0") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParseBezierSurface);
		}
		else if (strcmp(line.c_str(), "BSplineSurface") == 0)
		{
			params = parseModel(myReadfile, ParseType::ParseBSplineSurface);
		}
		recreateModel(params);
		strcpy(last, line.c_str());
	}
	fclose(myReadfile);
}

void swap(float& a, float& b)
{
	float tmp = a;
	a = b;
	b = tmp;
}

XMMATRIX& Loader::parseModelMatrix(FILE* myReadfile)
{
	fscanf(myReadfile, "TMtx=");
	XMMATRIX m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			float val;
			fscanf(myReadfile, "%f", &val);
			m(i, j) = val;
		}
	//swap(m(3, 0), m(0, 3));
	//swap(m(3, 1), m(1, 3));
	//swap(m(3, 2), m(2, 3));
	 
	fscanf(myReadfile, "\n");
	return m;
}

vector<int>* Loader::parseModelControlPoints(FILE* myReadfile)
{
	vector<int>* controlPointsId = new vector<int>();
	int cpId;
	while (fscanf(myReadfile, "CP=%d\n", &cpId) != 0)
	{
		if (controlPointsId->size() > 0 && cpId == (*controlPointsId)[controlPointsId->size() - 1])
			break;
		controlPointsId->push_back(cpId);
	}
	return controlPointsId;
}

ModelParams* Loader::parseModel(FILE* myReadFile, ParseType parseType)
{
	int id;
	fscanf(myReadFile, "Id=%d\n", &id);
	//string name;
	char* name = new char[255];
	fscanf(myReadFile, "Name=%s\n", name);

	float Width, Height;
	int PatchesXCount, PatchesYCount;
	string isSurfaceCyllindricalText;
	bool isSurfaceCyllindrical = false;
	if (parseType == ParseType::ParseBezierSurface||
		parseType == ParseType::ParseBSplineSurface)
	{
		fscanf(myReadFile, "Width=%f\n", &Width);
		fscanf(myReadFile, "Height=%f\n", &Height);
		fscanf(myReadFile, "PatchesXCount=%d\n", &PatchesXCount);
		fscanf(myReadFile, "PatchesYCount=%d\n", &PatchesYCount);
		fscanf(myReadFile, "Cylindrical=%s\n", &isSurfaceCyllindricalText);
		if (strcmp(isSurfaceCyllindricalText.c_str(), "True") == 0)
			isSurfaceCyllindrical = true;
		//remember to retain old values
		m_SettingsHelper->m_controller.view.m_surfaceWidth = Width;
		m_SettingsHelper->m_controller.view.m_surfaceHeight = Height;
		m_SettingsHelper->m_controller.view.m_cols = PatchesXCount;
		m_SettingsHelper->m_controller.view.m_rows = PatchesYCount;
		m_SettingsHelper->isSurfacePlane = !isSurfaceCyllindrical;
		m_SettingsHelper->m_controller.view.isSurfacePlane = !isSurfaceCyllindrical;
	}



	float R;
	float r;
	if (parseType == ParseType::ParseTorus)
	{
		fscanf(myReadFile, "R=%f\n", &R);
		fscanf(myReadFile, "r=%f\n", &r);
	}
	float X;
	float Y;
	float Z;
	double XD;
	double YD;
	double ZD;
	fscanf(myReadFile, "X=%f\n", &X);
	fscanf(myReadFile, "Y=%f\n", &Y);
	fscanf(myReadFile, "Z=%f\n", &Z);
	//X = (float)XD;
	//Y = (float)YD;
	//Z = (float)ZD;
	XMMATRIX modelMatrix = parseModelMatrix(myReadFile);
	char* color = new char[255];
	fscanf(myReadFile, "Color=%s\n", color);
	vector<int>* controlPoints = nullptr;
	if (parseType == ParseType::ParseCurve ||
		parseType == ParseType::ParseC2Interpolated||
		parseType == ParseType::ParseC2Curve||
		parseType == ParseType::ParseBezierSurface||
		parseType == ParseType::ParseBSplineSurface)
	{
		controlPoints = parseModelControlPoints(myReadFile);
	}
	fscanf(myReadFile, "\n");
	int a = 10;
	return new ModelParams(parseType, id, name, Width, Height, PatchesXCount, PatchesYCount, R, r, X, Y, Z, modelMatrix, color, controlPoints);
}

void Loader::recreateModel(ModelParams* params)
{
	if (params == nullptr)
		return;
	if (params->parseType != ParsePoint && params->parseType != ParseBezierSurface && params->parseType != ParseBSplineSurface)
		return;
	string color = string(params->color);
	InsertionParams* insertionParams;
	switch (params->parseType){
	case ParseType::ParseTorus:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::TorusName.c_str()));
		break;
	case ParseType::ParsePoint:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::SimplePointName.c_str()));
		break;
	case ParseType::ParseCurve:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierEdgeName.c_str()));
		break;
	case ParseType::ParseC2Interpolated:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierC2Interpolated.c_str()));
		break;
	case ParseType::ParseC2Curve:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierC2EdgeName.c_str()));
		break;
	case ParseType::ParseBezierSurface:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierSurface.c_str()));
		break;
	case ParseType::ParseBSplineSurface:
		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BSplineSurface.c_str()));
		break;
	}
	m_ListViewItemMapper.insert(pair<int, InsertionParams*>(params->id, insertionParams));
	ModelClass* model = insertionParams->model;
	model->m_modelMatrix = params->modelMatrix;
	if (params->parseType == ParseType::ParsePoint)
		model->ScaleToDefault();
	model->SetPosition(XMVectorSet(params->X, params->Y, params->Z, 1));


	//////COPY CHILDREN//////
	vector<int>* controlPoints = params->controlPoints;
	if (params->parseType == ParseType::ParseCurve||
		params->parseType == ParseType::ParseC2Interpolated||
		params->parseType == ParseType::ParseC2Curve)
	{
		for (int i = 0; i < controlPoints->size(); i++)
		{
			int id = (*controlPoints)[i];
			m_SettingsHelper->CopyItem(m_ListViewItemMapper[id]->item, insertionParams->item);
			m_ListViewItemMapper[id]->model->m_modelMatrix = m_ListViewItemMapper[id]->model->m_modelMatrix * model->m_modelMatrix;
		}
	}
	if (params->parseType == ParseType::ParseBezierSurface)
	{
		BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(model);
		vector<SimplePoint*> nodes = bezierSurface->GetNodes();
		for (int i = 0; i < controlPoints->size(); i++)
		{
			int id = (*controlPoints)[i];
			nodes[i]->m_modelMatrix = m_ListViewItemMapper[id]->model->m_modelMatrix * bezierSurface->m_modelMatrix;
			m_SettingsHelper->m_controller.removeItem(m_ListViewItemMapper[id]->item);
			delete m_ListViewItemMapper[id];
		}
		bezierSurface->Reset();
	}
	if (params->parseType == ParseType::ParseBSplineSurface)
	{
		BSplineSurface* bsplineSurface = dynamic_cast<BSplineSurface*>(model);
		vector<SimplePoint*> nodes = bsplineSurface->GetNodes();
		for (int i = 0; i < controlPoints->size(); i++)
		{
			int id = (*controlPoints)[i];
			nodes[i]->m_modelMatrix = m_ListViewItemMapper[id]->model->m_modelMatrix * bsplineSurface->m_modelMatrix;
			m_SettingsHelper->m_controller.removeItem(m_ListViewItemMapper[id]->item);
			delete m_ListViewItemMapper[id];
		}
		bsplineSurface->Reset();
	}

	delete params;
}