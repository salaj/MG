#include "modelsManager.h"

using namespace gk2;

void* ModelsManager::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void ModelsManager::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void ModelsManager::Initialize(Service& service)
{
	m_service = service;
}

ModelsManager::ModelsManager()
{
}

void ModelsManager::InitializeModels()
{for (map<int, ModelClass*> ::iterator it = m_models.begin(); it != m_models.end(); it++)
	
	{
		(*it).second -> Initialize();
	}
}
void ModelsManager::DrawModels()
{
	for (map<int, ModelClass*> ::iterator it = m_models.begin(); it != m_models.end(); it++)
	{
		(*it).second->Draw();
	}
}

void ModelsManager::createFakeSurfaceC0()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			point->Translate(XMFLOAT4(i * 5.0f, j * 5.0f, 0, 1));
			nodes.push_back(point);
		}
	m_service.shaderIndex = 5;
	BezierPatch* bezierPatch = new BezierPatch(m_service);
	bezierPatch->Initialize();
	dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
}

void ModelsManager::createFakeSurfaceC2()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	vector<ModelClass*> nodes_second = vector<ModelClass*>();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			point->Translate(XMFLOAT4(i * 5.0f, j * 5.0f, 0, 1));
			nodes.push_back(point);
		}

	nodes_second.push_back(nodes[4]);
	nodes_second.push_back(nodes[5]);
	nodes_second.push_back(nodes[6]);
	nodes_second.push_back(nodes[7]);

	nodes_second.push_back(nodes[8]);
	nodes_second.push_back(nodes[9]);
	nodes_second.push_back(nodes[10]);
	nodes_second.push_back(nodes[11]);

	nodes_second.push_back(nodes[12]);
	nodes_second.push_back(nodes[13]);
	nodes_second.push_back(nodes[14]);
	nodes_second.push_back(nodes[15]);

	for (int i = 1; i < 2; i++)
		for (int j = 0; j < 4; j++)
		{
			{
				point = AddModel(ModelType::SimplePointType);
				point->Translate(XMFLOAT4((i + 3) * 5.0f, j * 5.0f, 0, 1));
				nodes_second.push_back(point);
			}
		}
	m_service.shaderIndex = 6;
	BSplinePatch* bsplinePatch = new BSplinePatch(m_service);
	bsplinePatch->Initialize();
	dynamic_cast<BSplinePatch*>(bsplinePatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bsplinePatch->m_id, bsplinePatch));

	BSplinePatch* bsplinePatchSecond = new BSplinePatch(m_service);
	bsplinePatchSecond->Initialize();
	dynamic_cast<BSplinePatch*>(bsplinePatchSecond)->SetNodes(nodes_second);
	m_models.insert(pair<int, ModelClass*>(bsplinePatchSecond->m_id, bsplinePatchSecond));
}

void ModelsManager::createFakeC2Curve()
{

	ModelClass* point0 = AddModel(ModelType::SimplePointType);

	ModelClass* point1 = AddModel(ModelType::SimplePointType);
	point1->Translate(XMFLOAT4(10.0f, 10.0f, 0, 1));

	ModelClass* point2 = AddModel(ModelType::SimplePointType);
	point2->Translate(XMFLOAT4(20.0f, 15.0f, 0, 1));

	ModelClass* point3 = AddModel(ModelType::SimplePointType);
	point3->Translate(XMFLOAT4(30.0f, 5.0f, 0, 1));

	//ModelClass* point4 = AddModel(ModelType::SimplePointType);
	//point4->Translate(XMFLOAT4(40.0f, 10.0f, 0, 1));

	vector<ModelClass*> nodes = vector<ModelClass*>();
	nodes.push_back((point0));
	nodes.push_back((point1));
	nodes.push_back((point2));
	nodes.push_back((point3));
	//nodes.push_back((point4));

	m_service.shaderIndex = 4;
	BezierC2Curve* bezierCurve = new BezierC2Curve(m_service);
	dynamic_cast<BezierC2Curve*>(bezierCurve)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierCurve->m_id, bezierCurve));
	AddBezierCurve(bezierCurve);
}

void ModelsManager::createFakeInterpolatedC2Curve()
{

	ModelClass* point0 = AddModel(ModelType::SimplePointType);

	ModelClass* point1 = AddModel(ModelType::SimplePointType);
	point1->Translate(XMFLOAT4(10.0f, 10.0f, 0, 1));

	ModelClass* point2 = AddModel(ModelType::SimplePointType);
	point2->Translate(XMFLOAT4(20.0f, 15.0f, 0, 1));

	ModelClass* point3 = AddModel(ModelType::SimplePointType);
	point3->Translate(XMFLOAT4(30.0f, 5.0f, 0, 1));

	//ModelClass* point4 = AddModel(ModelType::SimplePointType);
	//point4->Translate(XMFLOAT4(40.0f, 10.0f, 0, 1));

	vector<ModelClass*> nodes = vector<ModelClass*>();
	nodes.push_back((point0));
	nodes.push_back((point1));
	nodes.push_back((point2));
	nodes.push_back((point3));
	//nodes.push_back((point4));

	m_service.shaderIndex = 4;
	InterpolatedC2Curve* bezierCurve = new InterpolatedC2Curve(m_service);
	bezierCurve->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierCurve->m_id, bezierCurve));
	AddBezierCurve(bezierCurve);
}

void ModelsManager::CreateModels()
{
	float scaleFactor = 0.1;
	m_service.shaderIndex = 3;
	ModelClass* model = new Cursor(m_service);
	model->Initialize();
	model->Scale(scaleFactor);
	m_models.insert(pair<int, ModelClass*>(model->m_id, model));

	//m_service.shaderIndex = 2;
	//for (int i = 0; i < 2; i++)
	//{
	//	ModelClass* model = new SimplePoint(m_service);
	//	m_models.push_back(model);
	//}

	//m_models[0]->Translate(XMFLOAT3(-0.8f, -0.8f, 0.0f));
	//m_models[1]->Translate(XMFLOAT3(-0.8f, +0.8f, 0.0f));
	////m_models[2]->Translate(XMFLOAT3(+0.8f, +0.8f, 0.0f));
	////m_models[3]->Translate(XMFLOAT3(+0.8f, -0.8f, 0.0f));

	//m_models[0]->Scale(scaleFactor);
	//m_models[1]->Scale(scaleFactor);
	//m_models[2]->Scale(scaleFactor);
	//m_models[3]->Scale(scaleFactor);


	//SetModelAtOtherModelPosition(m_models[1], m_models[0]);
	//setModelToPosition(m_models[1], XMFLOAT3(0.2f, 0.0f, 0.0f));


	//////////////
	//m_service.shaderIndex = 0;
	//ModelClass* torus = new Torus(m_service);
	//torus->Initialize();
	//m_models.push_back(torus);
	//////////////

	/*SimplePoint* point;
	point = dynamic_cast<SimplePoint*>(AddModel(ModelType::SimplePointType));
	point->Scale(0.7f);
	bezierCurve->m_Bezier.push_back(point);
	point = dynamic_cast<SimplePoint*>(AddModel(ModelType::SimplePointType));
	point->Scale(0.7f);
	bezierCurve->m_Bezier.push_back(point);
	point = dynamic_cast<SimplePoint*>(AddModel(ModelType::SimplePointType));
	point->Scale(0.7f);
	bezierCurve->m_Bezier.push_back(point);
	point = dynamic_cast<SimplePoint*>(AddModel(ModelType::SimplePointType));
	point->Scale(0.7f);
	bezierCurve->m_Bezier.push_back(point);*/

	//createFakeC2Curve();
	//createFakeInterpolatedC2Curve();
	//createFakeSurfaceC0();
	//createFakeSurfaceC2();

	//BezierCurve* bezierCurve = new BezierCurve(m_service);
	//dynamic_cast<BezierCurve*>(bezierCurve)->SetNodes(nodes);
	//bezierCurve->Initialize();
	//m_models.push_back(bezierCurve);
	//AddBezierCurve(bezierCurve);
}

ModelsManager::~ModelsManager()
{

}

void ModelsManager::setModelToPosition(ModelClass *model, XMFLOAT3 position)
{
	//XMVECTOR posFirst = XMVector3Transform(XMVectorZero(), m_models[0]->m_modelMatrix);
	//XMVECTOR pos = XMVector3Transform(XMVectorZero(), model->m_modelMatrix);
	model->m_modelMatrix._41 = position.x;
	model->m_modelMatrix._42 = position.y;
	model->m_modelMatrix._43 = position.z;
}

void ModelsManager::SetModelAtOtherModelPosition(ModelClass *model, ModelClass *relative_model)
{
	//XMVECTOR pos = XMVector3Transform(XMVectorZero(), relative_model->m_modelMatrix);
	model->m_modelMatrix._41 = relative_model->m_modelMatrix._41;
	model->m_modelMatrix._42 = relative_model->m_modelMatrix._42;
	model->m_modelMatrix._43 = relative_model->m_modelMatrix._43;
}

void ModelsManager::SetActiveModels(vector<int>& activeModels)
{
	float scaleFactor = 2.0f;
	//first scale down
	vector<ModelClass*> oldActiveModels = GetActiveModels();
	for (int i = 0; i < oldActiveModels.size(); i++)
	{
		if (oldActiveModels[i]->m_Type == ModelType::SimplePointType)
			oldActiveModels[i]->Scale(1 / scaleFactor);
	}
	//clear old active
	m_active_models.clear();
	//scale up newly selected
	for (int i = 0; i < activeModels.size(); i++)
	{
		AddActiveModel(activeModels[i]);
		if (m_models[activeModels[i]]->m_Type == ModelType::SimplePointType)
		{
			m_models[activeModels[i]]->Scale(scaleFactor);
		}
	}

	if (activeModels.size() == 0)
	{
		selected = NULL;
	}
	else
	{
		//put cursor at first active
		ModelClass* cursor = GetCursor();
		if (m_active_models.size() > 0)
		{
			selected = (*m_active_models.begin()).second;
			cursor->SetPosition(selected->GetPosition());
		}
	}
}


void ModelsManager::AddActiveModel(int id)
{
	m_active_models.insert(pair<int, ModelClass*>(id, m_models[id]));
}

void ModelsManager::AddBezierCurve(BezierCurve* curve)
{
	m_bezier_curves.insert(pair<int, BezierCurve*>(curve->m_id, curve));
}

void ModelsManager::AddBezierSurface(BezierSurface* bezierSurface)
{
	m_bezier_surfaces.insert(pair<int, BezierSurface*>(bezierSurface->m_id, bezierSurface));
}

void ModelsManager::AddBSplineSurface(BSplineSurface* bsplineSurface)
{
	m_bspline_surfaces.insert(pair<int, BSplineSurface*>(bsplineSurface->m_id, bsplineSurface));
}

void ModelsManager::RemoveActiveModel(int id)
{
	ModelClass* model = m_active_models[id];
	if (model != NULL)
	{
		//m_active_models.erase(model->m_id);
		m_active_models[id] = NULL;
	}
}

void ModelsManager::RemoveBezierCurve(int id)
{
	m_bezier_curves.erase(id);
}


void ModelsManager::RemoveModel(int id)
{
	ModelType type = m_models[id]->m_Type;
	m_models.erase(id);
	if (type == ModelType::BezierSurfaceType)
		m_bezier_surfaces.erase(id);
}

vector<ModelClass*>& ModelsManager::GetActiveModels()
{
	vector<ModelClass*>* activeModels = new vector<ModelClass*>();
	for (map<int, ModelClass*> ::iterator it = m_active_models.begin(); it != m_active_models.end(); it++)
	{
		activeModels->push_back((*it).second);
	}
	return *activeModels;
}

vector<BezierCurve*>& ModelsManager::GetBezierCurves()
{
	vector<BezierCurve*>* curves = new vector<BezierCurve*>();
	for (map<int, BezierCurve*> ::iterator it = m_bezier_curves.begin(); it != m_bezier_curves.end(); it++)
	{
		curves->push_back((*it).second);
	}
	return *curves;
}

vector<BezierSurface*>& ModelsManager::GetBezierSurfaces()
{
	vector<BezierSurface*>* surfaces = new vector<BezierSurface*>();
	for (map<int, BezierSurface*> ::iterator it = m_bezier_surfaces.begin(); it != m_bezier_surfaces.end(); it++)
	{
		surfaces->push_back((*it).second);
	}
	return *surfaces;
}

vector<BSplineSurface*>& ModelsManager::GetBSplineSurfaces()
{
	vector<BSplineSurface*>* surfaces = new vector<BSplineSurface*>();
	for (map<int, BSplineSurface*> ::iterator it = m_bspline_surfaces.begin(); it != m_bspline_surfaces.end(); it++)
	{
		surfaces->push_back((*it).second);
	}
	return *surfaces;
}


void ModelsManager::CopyOtherModelAfinityMatrix(ModelClass *model, ModelClass *relative_model)
{
	//XMVECTOR pos = XMVector3Transform(XMVectorZero(), relative_model->m_modelMatrix);
	model->m_modelMatrix = relative_model->m_modelMatrix;
}

ModelClass* ModelsManager::GetCursor()
{
	return m_models[0];
}

ModelClass* ModelsManager::AddModel(ModelType type)
{
	float scaleFactor = 0.02f;
	ModelClass* model;
	BezierCurve* bezierCurve;
	BezierC2Curve* bezierC2Curve;
	InterpolatedC2Curve* interpolatedC2Curve;
	BezierPatch* bezierPatch;
	BezierSurface* bezierSurface;
	BSplinePatch* bsplinePatch;
	BSplineSurface* bsplineSurface;
	ModelClass* cursor = GetCursor();
	//chcemy przestawiæ w dok³adnie to miejsce na ekranie, cursor wczeœniej mia³ nadan¹ skalê
	XMFLOAT4 cursorPosition = cursor->GetPosition();
	switch (type)
	{
	case SimplePointType:

		m_service.shaderIndex = 2;
		model = new SimplePoint(m_service);
		model->Initialize();

		model->Translate(cursorPosition);
		model->Scale(scaleFactor);

		m_models.insert(pair<int, ModelClass*>(model->m_id, model));
		return model;
	case TorusType:
		m_service.shaderIndex = 0;
		model = new Torus(m_service);
		model->Initialize();
		m_models.insert(pair<int, ModelClass*>(model->m_id, model));
		return model;
	case BezierType:
		m_service.shaderIndex = 4;
		bezierCurve = new BezierCurve(m_service);
		bezierCurve->Initialize();
		m_models.insert(pair<int, ModelClass*>(bezierCurve->m_id, bezierCurve));
		AddBezierCurve(bezierCurve);
		return bezierCurve;
	case BezierC2Type:
		m_service.shaderIndex = 4;
		bezierC2Curve = new BezierC2Curve(m_service);
		bezierC2Curve->Initialize();
		m_models.insert(pair<int, ModelClass*>(bezierC2Curve->m_id, bezierC2Curve));
		AddBezierCurve(bezierC2Curve);
		return bezierC2Curve;
	case InterpolatedC2Type:
		m_service.shaderIndex = 4;
		interpolatedC2Curve = new InterpolatedC2Curve(m_service);
		interpolatedC2Curve->Initialize();
		m_models.insert(pair<int, ModelClass*>(interpolatedC2Curve->m_id, interpolatedC2Curve));
		AddBezierCurve(interpolatedC2Curve);
		return interpolatedC2Curve;

	case BezierPatchType:
		m_service.shaderIndex = 5;
		bezierPatch = new BezierPatch(m_service);
		bezierPatch->Initialize();
		m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
		return bezierPatch;
	case BezierSurfaceType:
		bezierSurface = new BezierSurface();
		m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
		AddBezierSurface(bezierSurface);
		return bezierSurface;

	case BSplinePatchType:
		m_service.shaderIndex = 6;
		bsplinePatch = new BSplinePatch(m_service);
		bsplinePatch->Initialize();
		m_models.insert(pair<int, ModelClass*>(bsplinePatch->m_id, bsplinePatch));
		return bsplinePatch;
	case BSplineSurfaceType:
		bsplineSurface = new BSplineSurface();
		m_models.insert(pair<int, ModelClass*>(bsplineSurface->m_id, bsplineSurface));
		AddBSplineSurface(bsplineSurface);
		return bsplineSurface;
	//case ElipsoidType:
	//	m_service.shaderIndex = 1;
	//	m_models.push_back(new Elipsoid(m_service));
	//	break;
	default:
		break;
	}
	return NULL;
}




map<int, ModelClass*>& ModelsManager::GetModels()
{
	map<int, ModelClass*>* extendedSetOfModels = new map<int, ModelClass*>;
	for (map<int, ModelClass*> ::iterator it = ++(m_models.begin()); it != m_models.end(); it++)
	{
		extendedSetOfModels->insert(*it);
		if ((*it).second->m_Type == ModelType::BezierC2Type || (*it).second->m_Type == ModelType::InterpolatedC2Type)
		{
			BezierC2Curve* curve = dynamic_cast<BezierC2Curve*>((*it).second);
			for (int i = 0; i < curve->m_Bezier.size(); i++)
			{
				extendedSetOfModels->insert(pair<int, ModelClass*>(curve->m_Bezier[i]->m_id, curve->m_Bezier[i]));
			}
		}
	}
	return *extendedSetOfModels;
}

ModelClass* ModelsManager::GetModelById(int id)
{
	return m_models[id];
}
