#include "saver.h"

Saver::Saver()
{

}

void Saver::Initialize(SettingsHelper* settingsHelper, ModelsManager* modelsManager)
{
	m_SettingsHelper = settingsHelper;
	this->modelsManager = modelsManager;
}

void Saver::SaveScene(string pathToSaveFile)
{
	FILE* myWritefile;
	myWritefile = fopen(pathToSaveFile.c_str(), "w");
	
	map<int, ModelClass*> models =  modelsManager->GetModels();
	vector<ModelClass*> defferedModels;
	for (map<int, ModelClass*>::iterator it = models.begin(); it != models.end(); it++)
	{
		ModelClass* model = it->second;
		if (model->m_Type == ModelType::SimplePointType)
			parseModel(myWritefile, model);
		else
			defferedModels.push_back(model);
	}

	for (int i = 0; i < defferedModels.size(); i++)
		parseModel(myWritefile, defferedModels[i]);

	fclose(myWritefile);
}

//void swap(float& a, float& b)
//{
//	float tmp = a;
//	a = b;
//	b = tmp;
//}
//
void Saver::parseModelMatrix(FILE* myWritefile, XMMATRIX& m)
{
	fprintf(myWritefile, "TMtx=");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fprintf(myWritefile, "%f ", m(i, j));
		}
		fprintf(myWritefile, "\n");
	}
	//swap(m(3, 0), m(0, 3));
	//swap(m(3, 1), m(1, 3));
	//swap(m(3, 2), m(2, 3));
}

void Saver::parseModelControlPoints(FILE* myReadfile, vector<SimplePoint*>& nodes)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		fprintf(myReadfile, "CP=%d\n", nodes[i]->m_id);
	}
}

void Saver::parseModel(FILE* myReadFile, ModelClass* model)
{
	string name;
	if (model->m_Type == ModelType::BSplineSurfaceType)
	{
		name = "BSplineSurface";
	}
	else if (model->m_Type == ModelType::SimplePointType)
	{
		name = "Point";
	}
	else if (model->m_Type == ModelType::BezierSurfaceType)
	{
		name = "BezierSurfaceC0";
	}
	else if (model->m_Type == ModelType::BezierC2Type)
	{
		name = "BezierCurveC2";
	}
	else if (model->m_Type == ModelType::InterpolatedC2Type)
	{
		name = "InterpolationCurve";
	}
	else if (model->m_Type == ModelType::BezierType)
	{
		name = "BezierCurveC0";
	}
	else if (model->m_Type == ModelType::TorusType)
	{
		name = "Torus";
	}
	else
		return;
	fprintf(myReadFile, "%s\n", name.c_str());
	int id;
	fprintf(myReadFile, "Id=%d\n", model->m_id);
	fprintf(myReadFile, "Name=some_name\n");

	if (model->m_Type == ModelType::TorusType)
	{
		fprintf(myReadFile, "R=0.3\n");
		fprintf(myReadFile, "r=0.15\n");
	}

	if (model->m_Type == ModelType::BSplineSurfaceType)
	{
		BSplineSurface* surface = dynamic_cast<BSplineSurface*>(model);
		fprintf(myReadFile, "Width=%f\n", (float)surface->m_sizeX);
		fprintf(myReadFile, "Height=%f\n", (float)surface->m_sizeY);
		fprintf(myReadFile, "PatchesXCount=%d\n", surface->m_cols);
		fprintf(myReadFile, "PatchesYCount=%d\n",surface->m_rows);
		if (!surface->isCyllindrical)
			fprintf(myReadFile, "Cylindrical=False\n");
		else
			fprintf(myReadFile, "Cylindrical=True\n");
	}
	if (model->m_Type == ModelType::BezierSurfaceType)
	{
		BezierSurface* surface = dynamic_cast<BezierSurface*>(model);
		fprintf(myReadFile, "Width=%f\n", (float)surface->m_sizeX);
		fprintf(myReadFile, "Height=%f\n", (float)surface->m_sizeY);
		fprintf(myReadFile, "PatchesXCount=%d\n", surface->m_cols);
		fprintf(myReadFile, "PatchesYCount=%d\n", surface->m_rows);
		if (!surface->isCyllindrical)
			fprintf(myReadFile, "Cylindrical=False\n");
		else
			fprintf(myReadFile, "Cylindrical=True\n");
	}
	XMFLOAT3 position = model->GetPosition3();
	fprintf(myReadFile, "X=%f\n", position.x);
	fprintf(myReadFile, "Y=%f\n", position.y);
	fprintf(myReadFile, "Z=%f\n", position.z);

	if (model->m_Type == SimplePointType)
	{
		model->ScaleBackToDefault();
	}
	parseModelMatrix(myReadFile, model->m_modelMatrix);
	if (model->m_Type == SimplePointType)
	{
		model->ScaleToDefault();
	}
	fprintf(myReadFile, "Color=White\n");

	if (model->m_Type == ModelType::BSplineSurfaceType)
	{
		BSplineSurface* surface = dynamic_cast<BSplineSurface*>(model);
		vector<SimplePoint*> nodes = surface->GetNodes();
		parseModelControlPoints(myReadFile, nodes);
	}
	if (model->m_Type == ModelType::BezierSurfaceType)
	{
		BezierSurface* surface = dynamic_cast<BezierSurface*>(model);
		vector<SimplePoint*> nodes = surface->GetNodes();
		parseModelControlPoints(myReadFile, nodes);
	}
	if (model->m_Type == ModelType::BezierType)
	{
		BezierCurve* curve = dynamic_cast<BezierCurve*>(model);
		vector<SimplePoint*> nodes = curve->GetNodes();
		parseModelControlPoints(myReadFile, nodes);
	}
	if (model->m_Type == ModelType::BezierC2Type)
	{
		BezierC2Curve* curve = dynamic_cast<BezierC2Curve*>(model);
		vector<SimplePoint*> nodes = curve->GetDeboors();
		parseModelControlPoints(myReadFile, nodes);
	}
	if (model->m_Type == ModelType::InterpolatedC2Type)
	{
		InterpolatedC2Curve* curve = dynamic_cast<InterpolatedC2Curve*>(model);
		vector<SimplePoint*> nodes = curve->GetInterpolants();
		parseModelControlPoints(myReadFile, nodes);
	}

	fprintf(myReadFile, "\n");

	//float X;
	//float Y;
	//float Z;
	//fscanf(myReadFile, "X=%f\n", &X);
	//fscanf(myReadFile, "Y=%f\n", &Y);
	//fscanf(myReadFile, "Z=%f\n", &Z);
	//XMMATRIX modelMatrix = parseModelMatrix(myReadFile);
	//char* color = new char[255];
	//fscanf(myReadFile, "Color=%s\n", color);
	//vector<int>* controlPoints = nullptr;
	//if (parseType == ParseType::ParseCurve ||
	//	parseType == ParseType::ParseC2Interpolated||
	//	parseType == ParseType::ParseC2Curve||
	//	parseType == ParseType::ParseBezierSurface)
	//{
	//	controlPoints = parseModelControlPoints(myReadFile);
	//}
	//fscanf(myReadFile, "\n");
	//int a = 10;
	//return new ModelParams(parseType, id, name, Width, Height, PatchesXCount, PatchesYCount, R, r, X, Y, Z, modelMatrix, color, controlPoints);
}
//
//void Loader::recreateModel(ModelParams* params)
//{
//	if (params == nullptr)
//		return;
//	if (params->parseType != ParsePoint && params->parseType != ParseBezierSurface)
//		return;
//	string color = string(params->color);
//	InsertionParams* insertionParams;
//	switch (params->parseType){
//	case ParseType::ParseTorus:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::TorusName.c_str()));
//		break;
//	case ParseType::ParsePoint:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::SimplePointName.c_str()));
//		break;
//	case ParseType::ParseCurve:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierEdgeName.c_str()));
//		break;
//	case ParseType::ParseC2Interpolated:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierC2Interpolated.c_str()));
//		break;
//	case ParseType::ParseC2Curve:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierC2EdgeName.c_str()));
//		break;
//	case ParseType::ParseBezierSurface:
//		insertionParams = m_SettingsHelper->AddNewModelToTreeView(const_cast<wchar_t*>(SettingsHelper::BezierSurface.c_str()));
//		break;
//	}
//	m_ListViewItemMapper.insert(pair<int, InsertionParams*>(params->id, insertionParams));
//	ModelClass* model = insertionParams->model;
//	model->m_modelMatrix = params->modelMatrix;
//	if (params->parseType == ParseType::ParsePoint)
//		model->ScaleToDefault();
//	model->SetPosition(XMVectorSet(params->X, params->Y, params->Z, 1));
//
//
//	//////COPY CHILDREN//////
//	vector<int>* controlPoints = params->controlPoints;
//	if (params->parseType == ParseType::ParseCurve||
//		params->parseType == ParseType::ParseC2Interpolated||
//		params->parseType == ParseType::ParseC2Curve)
//	{
//		for (int i = 0; i < controlPoints->size(); i++)
//		{
//			int id = (*controlPoints)[i];
//			m_SettingsHelper->CopyItem(m_ListViewItemMapper[id]->item, insertionParams->item);
//			m_ListViewItemMapper[id]->model->m_modelMatrix = m_ListViewItemMapper[id]->model->m_modelMatrix * model->m_modelMatrix;
//		}
//	}
//	if (params->parseType == ParseType::ParseBezierSurface)
//	{
//		BezierSurface* bezierSurface = dynamic_cast<BezierSurface*>(model);
//		vector<SimplePoint*> nodes = bezierSurface->GetNodes();
//		for (int i = 0; i < controlPoints->size(); i++)
//		{
//			int id = (*controlPoints)[i];
//			nodes[i]->m_modelMatrix = m_ListViewItemMapper[id]->model->m_modelMatrix * bezierSurface->m_modelMatrix;
//			m_SettingsHelper->m_controller.removeItem(m_ListViewItemMapper[id]->item);
//			delete m_ListViewItemMapper[id];
//		}
//		bezierSurface->Reset();
//	}
//
//	delete params;
//}