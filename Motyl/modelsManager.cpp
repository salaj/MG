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

BezierPatch* ModelsManager::createFakePatchC0()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			point->Translate(XMFLOAT4(i * 0.3f, j * 0.3f, 0, 0));
			nodes.push_back(point);
		}
	m_service.shaderIndex = 5;
	BezierPatch* bezierPatch = new BezierPatch(m_service);
	bezierPatch->Initialize();
	dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
	return bezierPatch;
}

BezierPatch* ModelsManager::createFakePatchC0_ForGregory()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			point->Translate(XMFLOAT4(i * 0.1f, j * 0.1f, 0, 1));
			nodes.push_back(point);
		}
	m_service.shaderIndex = 5;
	BezierPatch* bezierPatch = new BezierPatch(m_service);
	bezierPatch->Initialize();
	dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
	return bezierPatch;
}

BezierPatch* ModelsManager::createFakeCyllindricalPatchC0()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	ModelClass* pam;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			if (j == 0)
				pam = point;
			if (j == 3)
			{
				nodes.push_back(pam);
			}else
				nodes.push_back(point);
		}
	m_service.shaderIndex = 5;
	BezierPatch* bezierPatch = new BezierPatch(m_service);
	bezierPatch->Initialize();
	dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
	return bezierPatch;
}

BezierPatch* ModelsManager::createFakeCyllindricalDoublePatchC0()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	ModelClass* pam;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			if (j == 0)
				pam = point;
			if (j == 3)
			{
				nodes.push_back(pam);
			}
			else
				nodes.push_back(point);
		}
	m_service.shaderIndex = 5;
	BezierPatch* bezierPatch = new BezierPatch(m_service);
	bezierPatch->Initialize();
	dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
	return bezierPatch;
}

vector<BezierPatch*> ModelsManager::createFakeCyllindricalPatches_N_M_C0(int rows, int cols)
{
	vector<BezierPatch*> bezierPatches;
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();

	ModelClass*** items = new ModelClass**[rows * 3 + 1];

	for (int i = 0; i < rows * 3 + 1; i++)
	{
		items[i] = new ModelClass*[cols * 3];
		for (int j = 0; j < cols * 3; j++)
		{
			items[i][j] = AddModel(ModelType::SimplePointType);
		}
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			m_service.shaderIndex = 5;
			BezierPatch* bezierPatch = new BezierPatch(m_service);
			bezierPatch->Initialize();
			nodes.clear();

			int row = i * 3;
			for (int n = 0; n < 4; n++)
			{
				int col = j * 3;
				for (int m = 0; m < 4; m++)
				{
					if (col == cols * 3)
						nodes.push_back(items[row][0]);
					else
						nodes.push_back(items[row][col]);
					col++;
				}
				row++;
			}

			dynamic_cast<BezierPatch*>(bezierPatch)->SetNodes(nodes);
			m_models.insert(pair<int, ModelClass*>(bezierPatch->m_id, bezierPatch));
			bezierPatches.push_back(bezierPatch);
		}
	}
	return bezierPatches;
}

GregoryPatch* ModelsManager::createFakeGregoryPatch()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0, 0, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.0f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.0f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.3f, 0.0f, 0, 1));
	nodes.push_back(point);
	///////////
	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0, 0.1f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.1f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.1f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.1f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.1f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.3f, 0.1f, 0, 1));
	nodes.push_back(point);
	///////////
	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0, 0.2f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.2f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.2f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.2f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.2f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.3f, 0.2f, 0, 1));
	nodes.push_back(point);

	////////////////
	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0, 0.3f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.1f, 0.3f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.2f, 0.3f, 0, 1));
	nodes.push_back(point);

	point = AddModel(ModelType::SimplePointType);
	point->Translate(XMFLOAT4(0.3f, 0.3f, 0, 1));
	nodes.push_back(point);

	m_service.shaderIndex = 7;
	GregoryPatch* gregoryPatch = new GregoryPatch(m_service);
	gregoryPatch->Initialize();
	gregoryPatch->SetNodes(nodes);
	m_models.insert(pair<int, ModelClass*>(gregoryPatch->m_id, gregoryPatch));
	return gregoryPatch;
}

void ModelsManager::createFakeGregorySurface()
{
	GregorySurface* gregorySurface = new GregorySurface();
	GregoryPatch* gregoryPatch = createFakeGregoryPatch();
	gregorySurface->AddPatch(gregoryPatch);

	gregoryPatch = createFakeGregoryPatch();
	gregorySurface->AddPatch(gregoryPatch);
	gregoryPatch = createFakeGregoryPatch();
	gregorySurface->AddPatch(gregoryPatch);

	vector<BezierSurface*> bezierSurfaces = createTripleSurfaceHole();
	gregorySurface->SetBezierSurfaces(bezierSurfaces);
	gregorySurface->TranslatePoints(TranslatedBezier::right);
	m_gregory_surfaces.insert(pair<int, GregorySurface*>(gregorySurface->m_id, gregorySurface));
	m_models.insert(pair<int, ModelClass*>(gregorySurface->m_id, gregorySurface));
}

BezierSurface* ModelsManager::createFakeSurfaceC0()
{
	BezierPatch* bezierPatch = createFakePatchC0();
	BezierSurface* bezierSurface = new BezierSurface();
	bezierSurface->AddPatch(bezierPatch);
	m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
	AddBezierSurface(bezierSurface);
	return bezierSurface;
}

BezierSurface* ModelsManager::createFakeSurfaceC0_ForGregory()
{
	BezierPatch* bezierPatch = createFakePatchC0_ForGregory();
	BezierSurface* bezierSurface = new BezierSurface();
	bezierSurface->AddPatch(bezierPatch);
	m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
	AddBezierSurface(bezierSurface);
	return bezierSurface;
}

BezierSurface* ModelsManager::createFakeCyllindricalSurfaceC0()
{
	BezierPatch* bezierPatch = createFakeCyllindricalPatchC0();
	BezierSurface* bezierSurface = new BezierSurface();
	bezierSurface->AddPatch(bezierPatch);
	bezierSurface->SetDimensions(1, 1, 0.6f, 0.6f);
	bezierSurface->TranslateCyllinderPoints();
	bezierSurface->Reset();
	m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
	AddBezierSurface(bezierSurface);
	return bezierSurface;
}

BezierSurface* ModelsManager::createFakeCyllindricalSurfaceDoubleIntersectionC0()
{
	BezierPatch* bezierPatch = createFakeCyllindricalDoublePatchC0();
	BezierSurface* bezierSurface = new BezierSurface();
	bezierSurface->AddPatch(bezierPatch);
	bezierSurface->SetDimensions(1, 1, 0.2f, 1.0f);
	bezierSurface->TranslateCyllinderPoints();

	vector<SimplePoint*> points = bezierSurface->GetNodes();

	float xTranslation = 0.4f;

	points[9]->SetPosition(points[0]->GetPosition());
	points[9]->Translate(XMFLOAT4(xTranslation, 0.0f, 0.0f, 0.0f));
	points[10]->SetPosition(points[1]->GetPosition());
	points[10]->Translate(XMFLOAT4(xTranslation, 0.0f, 0.0f, 0.0f));
	points[11]->SetPosition(points[2]->GetPosition());
	points[11]->Translate(XMFLOAT4(xTranslation, 0.0f, 0.0f, 0.0f));

	bezierSurface->Reset();
	m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
	AddBezierSurface(bezierSurface);
	return bezierSurface;
}

BezierSurface* ModelsManager::createFakeCyllindricalSurface_N_M_C0()
{
	int rows, cols;
	rows = 1;
	cols = 5;
	vector<BezierPatch*> bezierPatches = createFakeCyllindricalPatches_N_M_C0(rows, cols);
	BezierSurface* bezierSurface = new BezierSurface();
	for (int i = 0; i < bezierPatches.size(); i++)
		bezierSurface->AddPatch(bezierPatches[i]);

	bezierSurface->SetDimensions(rows, cols, 0.4f, 0.4f);
	bezierSurface->TranslateCyllinderPoints();
	bezierSurface->Reset();
	m_models.insert(pair<int, ModelClass*>(bezierSurface->m_id, bezierSurface));
	AddBezierSurface(bezierSurface);
	return bezierSurface;
}

IntersectionSurface* ModelsManager::createFakeIntersectionManual()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);

	vector<BezierSurface*> bezierSurfaces = GetBezierSurfaces();
	vector<ModelClass*> activeModels = GetActiveModels();
	int counter = 0;
	//for (int i = 0; i < activeModels.size(); i++)
	//{
	//	if (activeModels[i]->m_Type == ModelType::BezierSurfaceType)
	//	{
	//		intersecionSurface->AddSurface(bezierSurfaces[i]);
	//		counter++;
	//	}
	//}

	for (map<int, BezierSurface*>::iterator it = m_activeBezier_surfaces.begin(); it != m_activeBezier_surfaces.end(); it++)
	{
		intersecionSurface->AddSurface((*it).second);
		counter++;
	}

	//for (int i = 0; i < m_activeBezier_surfaces.size(); i++)
	//{
	//	intersecionSurface->AddSurface(m_activeBezier_surfaces[i]);
	//	counter++;
	//}

	if (counter != 2)
	{
		delete intersecionSurface;
		if (bezierSurfaces.size() < 2)
			return nullptr;
		intersecionSurface = new IntersectionSurface(m_service);
		intersecionSurface->AddSurface(bezierSurfaces[0]);
		intersecionSurface->AddSurface(bezierSurfaces[1]);
	}
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	float u, v, s, t;
	intersecionSurface->FindFirstPoint(u, v, s, t, GetCursor()->GetPosition());

	intersecionSurface->SetOrigin(u, v, s, t);

	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}


IntersectionSurface* ModelsManager::createFakeIntersectionPlanePlaneSurface()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	vector<SimplePoint*> points = bezierSurface->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{

		////OK CALOSC
		points[i]->RotateY(-0.5f);
		points[i]->Translate(XMFLOAT4(0.05f, -0.2f, +0.6f, 0.0f));

		////OK CALOSC
		//points[i]->RotateY(0.5f);
		//points[i]->Translate(XMFLOAT4(0.05f, -0.2f, +0.4f, 0.0f));

		//OK WYCHYLENIE W PRAWO
		//points[i]->RotateY(0.5f);
		//points[i]->Translate(XMFLOAT4(0.4f, -0.2f, +0.4f, 0.0f));

		//OK WYCHYLENIE W LEWO
		//points[i]->RotateY(0.5f);
		//points[i]->Translate(XMFLOAT4(-0.4f, -0.2f, +0.4f, 0.0f));
	}
	bezierSurface->Reset();


	BezierSurface* bezierSurface1 = createFakeSurfaceC0();
	vector<SimplePoint*> points3 = bezierSurface1->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{


		points3[i]->RotateX(-XM_PIDIV2);
		//points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	bezierSurface1->Reset();
	//GetCursor()->SetPosition(0.2f, 0.2f, 0.0f);
	intersecionSurface->AddSurface(bezierSurface);
	intersecionSurface->AddSurface(bezierSurface1);

	//intersecionSurface->FindIntersection();

	float u, v, s, t;
	//u = 0.9f;
	//v = 0.1f;
	//s = 0.1f;
	//t = 0.9f;

	u = 0.5f;
	v = 0.2f;
	s = 0.5f;
	t = 0.5f;

	//bezierSurface->GetNodes()[0]->SetPosition(bezierSurface->Q(u, v));

	//bezierSurface1->GetNodes()[0]->SetPosition(bezierSurface1->Q(s, t));

	intersecionSurface->SetOrigin(u, v, s, t);

	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}



IntersectionSurface* ModelsManager::createFakeIntersectionPlaneCyllinderSurface()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	vector<SimplePoint*> points3 = bezierSurface->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		points3[i]->Translate(XMFLOAT4(0.0f, -0.3f, 0.0f, 0.0f));
		points3[i]->RotateX(-0.5f);

		//points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	bezierSurface->Reset();
	//GetCursor()->SetPosition(0.2f, 0.2f, 0.0f);
	intersecionSurface->AddSurface(bezierSurface);

	BezierSurface* cyllindricalSurface = createFakeCyllindricalSurfaceC0();
	intersecionSurface->AddSurface(cyllindricalSurface);
	vector<SimplePoint*> points = cyllindricalSurface->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{
		points[i]->RotateX(XM_PI / 2.0f);
		points[i]->Translate(XMFLOAT4(0.05f, 0.15f, 0.0f, 0.0f));
	}
	cyllindricalSurface->Reset();

	float u, v, s, t;
	//u = 0.9f;
	//v = 0.1f;
	//s = 0.1f;
	//t = 0.9f;

	u = 0.1f;
	v = 0.1f;
	s = 0.1f;
	t = 0.1f;

	cyllindricalSurface->GetNodes()[0]->SetPosition(cyllindricalSurface->Q(s, t));

	intersecionSurface->SetOrigin(u, v, s, t);

	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}

IntersectionSurface* ModelsManager::createFakeIntersectionPlane_Double_CyllinderSurface()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	vector<SimplePoint*> points3 = bezierSurface->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		//points3[i]->Translate(XMFLOAT4(0.0f, -0.3f, 0.0f, 0.0f));
		points3[i]->RotateX(XM_PIDIV2);

		//points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	bezierSurface->Reset();
	//GetCursor()->SetPosition(0.2f, 0.2f, 0.0f);
	intersecionSurface->AddSurface(bezierSurface);

	BezierSurface* cyllindricalSurface = createFakeCyllindricalSurfaceDoubleIntersectionC0();
	intersecionSurface->AddSurface(cyllindricalSurface);
	vector<SimplePoint*> points = cyllindricalSurface->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{
		//points[i]->RotateX(XM_PI / 2.0f);
		points[i]->Translate(XMFLOAT4(0.2f, 0.4f, -0.4f, 0.0f));
	}
	cyllindricalSurface->Reset();

	float u, v, s, t;
	//DO JEDNEGO
	u = 0.1f;
	v = 0.1f;
	s = 0.1f;
	t = 0.1f;


	//DO DRUGIEGO
	//u = 0.9f;
	//v = 0.9f;
	//s = 0.9f;
	//t = 0.9f;

	//cyllindricalSurface->GetNodes()[0]->SetPosition(cyllindricalSurface->Q(s, t));

	intersecionSurface->SetOrigin(u, v, s, t);

	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}


IntersectionSurface* ModelsManager::createFakeIntersection_N_M_PlaneCyllinderSurface()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	vector<SimplePoint*> points3 = bezierSurface->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		points3[i]->RotateX(XM_PIDIV2);
		points3[i]->Translate(XMFLOAT4(-0.45f, 0.0f, 0.4f, 0.0f));
		//points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	bezierSurface->Reset();
	//GetCursor()->SetPosition(0.2f, 0.2f, 0.0f);
	intersecionSurface->AddSurface(bezierSurface);



	BezierSurface* cyllindricalSurface = createFakeCyllindricalSurface_N_M_C0();
	intersecionSurface->AddSurface(cyllindricalSurface);
	vector<SimplePoint*> points = cyllindricalSurface->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{
		//points[i]->RotateX(XM_PI / 2.0f);
		//points[i]->Translate(XMFLOAT4(0.05f, 0.15f, 0.0f, 0.0f));
	}
	cyllindricalSurface->Reset();

	float u, v, s, t;
	//u = 0.9f;
	//v = 0.1f;
	//s = 0.1f;
	//t = 0.9f;

	u = 0.1f;
	v = 0.1f;
	s = 0.33f;
	t = 0.33f;

	//cyllindricalSurface->GetNodes()[0]->SetPosition(cyllindricalSurface->Q(s, t));

	intersecionSurface->SetOrigin(u, v, s, t);


	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}

IntersectionSurface* ModelsManager::createFakeIntersectionPlaneHalfCyllinderSurface()
{
	m_service.shaderIndex = 8;
	IntersectionSurface* intersecionSurface = new IntersectionSurface(m_service);
	m_models.insert(pair<int, ModelClass*>(intersecionSurface->m_id, intersecionSurface));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	vector<SimplePoint*> points3 = bezierSurface->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		points3[i]->RotateX(-XM_PIDIV2);
	}
	bezierSurface->Reset();
	intersecionSurface->AddSurface(bezierSurface);

	BezierSurface* cyllindricalSurface = createFakeCyllindricalSurfaceC0();
	intersecionSurface->AddSurface(cyllindricalSurface);
	vector<SimplePoint*> points = cyllindricalSurface->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{
		points[i]->RotateZ(XM_PI);
		points[i]->Translate(XMFLOAT4(0.05f, 0.15f, 0.3f, 0.0f));
	}
	cyllindricalSurface->Reset();

	float u, v, s, t;
	u = 0.1f;
	v = 0.1f;
	s = 0.4f;
	t = 0.4f;

	cyllindricalSurface->GetNodes()[0]->SetPosition(cyllindricalSurface->Q(s, t));

	intersecionSurface->SetOrigin(u, v, s, t);

	intersecionSurface->FindIntersection(u, v, s, t);

	return intersecionSurface;
}

IntersectionSurface* ModelsManager::createFakeIntersection()
{
	IntersectionSurface* intersecionSurface = new IntersectionSurface();
	m_service.shaderIndex = 3;//shall be changed;
	Line* line = new Line(m_service);
	line->X = 0.2;
	line->Y = 0.2;
	intersecionSurface->P = line;
	m_models.insert(pair<int, ModelClass*>(line->m_id, line));

	BezierSurface* bezierSurface = createFakeSurfaceC0();
	XMFLOAT4 p = bezierSurface->Q(0.2, 0.0f);
	vector<SimplePoint*> points3 = bezierSurface->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		points3[i]->RotateX(-0.5f);
		//points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	bezierSurface->Reset();
	GetCursor()->SetPosition(0.2f, 0.2f, 0.0f);
	intersecionSurface->AddSurface(bezierSurface);

	intersecionSurface->NewtonLineSurface();

	return intersecionSurface;
}

vector<BezierSurface*> ModelsManager::createTripleSurfaceHole()
{
	vector<BezierSurface*> bezierSurfaces;
	BezierSurface* surface1 = createFakeSurfaceC0_ForGregory();
	vector<SimplePoint*> points = surface1->GetNodes();
	for (int i = 0; i < points.size(); i++)
	{ 
		//15->3
		//points[i]->RotateZ(XM_PI / 2.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//3->15
		//points[i]->m_modelMatrix = points[i]->m_modelMatrix * XMMatrixScaling(-1.0, 1.0f, 1.0f);//3->15
		//points[i]->RotateZ(XM_PI / 2.0f);//3->15
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));//3->15

		//0->12
		//points[i]->RotateZ(-XM_PI / 2.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//12->0
		//points[i]->RotateZ(-XM_PI / 2.0f);
		//points[i]->m_modelMatrix = points[i]->m_modelMatrix * XMMatrixScaling(1.0, -1.0f, 1.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//0->3
		//points[i]->m_modelMatrix = points[i]->m_modelMatrix * XMMatrixScaling(-1.0, 1.0f, 1.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//3->0
		//points[i]->m_modelMatrix = points[i]->m_modelMatrix * XMMatrixScaling(-1.0, -1.0f, 1.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//12->15
		points[i]->RotateZ(XM_PI / 6.0f);
		points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));

		//15->12
		//points[i]->m_modelMatrix = points[i]->m_modelMatrix * XMMatrixScaling(1.0, -1.0f, 1.0f);
		//points[i]->RotateZ(-XM_PI / 6.0f);
		//points[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));
	}
	surface1->Reset();
	BezierSurface* surface2 = createFakeSurfaceC0_ForGregory();
	vector<SimplePoint*> points2 = surface2->GetNodes();
	for (int i = 0; i < points2.size(); i++)
	{
		//12->15
		points2[i]->RotateZ(XM_PI / 6.0f);
		points2[i]->Translate(XMFLOAT4(-0.6f, 0.0f, 0.0f, 0.0f));
		points2[i]->m_modelMatrix = points2[i]->m_modelMatrix * XMMatrixScaling(-1.0, 1.0f, 1.0f);

		//0->3
		//points2[i]->RotateZ(-XM_PI / 6.0f); 
		//points2[i]->Translate(XMFLOAT4(0.6f, 0.0f, 0.0f, 0.0f)); //0->3


		//3->0
		//points2[i]->RotateZ(-XM_PI / 6.0f); //0->3
		//points2[i]->m_modelMatrix = points2[i]->m_modelMatrix * XMMatrixScaling(1.0, -1.0f, 1.0f);
		//points2[i]->Translate(XMFLOAT4(0.6f, 0.0f, 0.0f, 0.0f)); //0->3

		////12->0
		//points2[i]->RotateZ(2 * XM_PI / 6.0f);
		//points2[i]->Translate(XMFLOAT4(0.6f, 0.3f, 0.0f, 0.0f));

		//0->12
		//points2[i]->m_modelMatrix = points2[i]->m_modelMatrix * XMMatrixScaling(1.0, -1.0f, 1.0f);
		//points2[i]->RotateZ(-XM_PI / 2.0f);
		//points2[i]->Translate(XMFLOAT4(0.6f, 0.3f, 0.0f, 0.0f));
	}

	surface2->Reset();
	vector<SimplePoint*>toCollapse;
	//toCollapse.push_back(surface1->GetNodes()[15]); //3->15
	//toCollapse.push_back(surface1->GetNodes()[3]); //15->3
	//toCollapse.push_back(surface1->GetNodes()[12]);//0->12
	//toCollapse.push_back(surface1->GetNodes()[3]);//0->3
	toCollapse.push_back(surface1->GetNodes()[15]);//12->15
	//toCollapse.push_back(surface1->GetNodes()[12]);//15->12
	//toCollapse.push_back(surface1->GetNodes()[0]);//3->0
	//toCollapse.push_back(surface1->GetNodes()[0]);//12->0

	//toCollapse.push_back(surface2->GetNodes()[3]);

	//toCollapse.push_back(surface1->GetNodes()[15]);
	toCollapse.push_back(surface2->GetNodes()[15]);
	collapseMultiSelected(toCollapse);
	//return bezierSurfaces;
	BezierSurface* surface3 = createFakeSurfaceC0_ForGregory();
	vector<SimplePoint*> points3 = surface3->GetNodes();
	for (int i = 0; i < points3.size(); i++)
	{
		//points3[i]->m_modelMatrix = points3[i]->m_modelMatrix * XMMatrixScaling(1.0, -1.0f, 1.0f);
		points3[i]->Translate(XMFLOAT4(-0.15f, -0.6f, 0.0f, 0.0f));
	}
	surface3->Reset();

	vector<SimplePoint*>toCollapse2;
	toCollapse2.push_back(surface2->GetNodes()[12]);
	//toCollapse2.push_back(surface2->GetNodes()[0]);
	toCollapse2.push_back(surface3->GetNodes()[15]);
	collapseMultiSelected(toCollapse2);


	vector<SimplePoint*>toCollapse3;
	//toCollapse3.push_back(surface1->GetNodes()[0]); //0->12
	//toCollapse3.push_back(surface1->GetNodes()[0]);//0->3
	toCollapse3.push_back(surface1->GetNodes()[12]);//12->15
	//toCollapse3.push_back(surface1->GetNodes()[15]);//15->12
	//toCollapse3.push_back(surface1->GetNodes()[3]);//3->0
	//toCollapse3.push_back(surface1->GetNodes()[3]); //3->15
	//toCollapse3.push_back(surface1->GetNodes()[15]); //15->3
	//toCollapse3.push_back(surface1->GetNodes()[12]); //12->0

	//toCollapse3.push_back(surface1->GetNodes()[12]);
	toCollapse3.push_back(surface3->GetNodes()[3]);
	collapseMultiSelected(toCollapse3);

	bezierSurfaces.push_back(surface1);
	bezierSurfaces.push_back(surface2);
	bezierSurfaces.push_back(surface3);

	return bezierSurfaces;
}

void ModelsManager::createFakePatchC2()
{
	ModelClass* point;
	vector<ModelClass*> nodes = vector<ModelClass*>();
	vector<ModelClass*> nodes_second = vector<ModelClass*>();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			point = AddModel(ModelType::SimplePointType);
			point->Translate(XMFLOAT4(i * 0.1f, j * 0.1f, 0, 1));
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
				point->Translate(XMFLOAT4((i + 3) * 0.1f, j * 0.1f, 0, 1));
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
	point1->Translate(XMFLOAT4(0.1f, 0.1f, 0, 1));

	ModelClass* point2 = AddModel(ModelType::SimplePointType);
	point2->Translate(XMFLOAT4(0.2f, 0.15f, 0, 1));

	ModelClass* point3 = AddModel(ModelType::SimplePointType);
	point3->Translate(XMFLOAT4(0.3f, 0.5f, 0, 1));

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
	//createFakePatchC0();
	//createFakePatchC2();
	//createFakeSurfaceC0();

	//createFakeGregoryPatch();
	//createFakeGregorySurface();

	//createTripleSurfaceHole();

	//createFakeIntersection();
	//createFakeIntersectionPlaneCyllinderSurface();
	//createFakeIntersectionPlanePlaneSurface();
	//createFakeIntersectionPlaneHalfCyllinderSurface();
	//createFakeIntersection_N_M_PlaneCyllinderSurface();
	//createFakeIntersectionPlane_Double_CyllinderSurface();

	//BezierCurve* bezierCurve = new BezierCurve(m_service);
	//dynamic_cast<BezierCurve*>(bezierCurve)->SetNodes(nodes);
	//bezierCurve->Initialize();
	//m_models.push_back(bezierCurve);
	//AddBezierCurve(bezierCurve);
}

bool ModelsManager::isGregorySurfaceReadyToCreate()
{
	vector<BezierSurface*> bezierSurfaces = GetBezierSurfaces();
	if (bezierSurfaces.size() < 3)
		return false;
	if (bezierSurfaces[0]->firstCollapse != 0 && bezierSurfaces[0]->firstCollapse != 3 && bezierSurfaces[0]->firstCollapse != 12 && bezierSurfaces[0]->firstCollapse != 15)
	{
		return false;
	}
	if (bezierSurfaces[1]->firstCollapse != 0 && bezierSurfaces[1]->firstCollapse != 3 && bezierSurfaces[1]->firstCollapse != 12 && bezierSurfaces[1]->firstCollapse != 15)
	{
		return false;
	}
	if (bezierSurfaces[2]->firstCollapse != 0 && bezierSurfaces[2]->firstCollapse != 3 && bezierSurfaces[2]->firstCollapse != 12 && bezierSurfaces[2]->firstCollapse != 15)
	{
		return false;
	}

	if (bezierSurfaces[0]->secondCollapse != 0 && bezierSurfaces[0]->secondCollapse != 3 && bezierSurfaces[0]->secondCollapse != 12 && bezierSurfaces[0]->secondCollapse != 15)
	{
		return false;
	}
	if (bezierSurfaces[1]->secondCollapse != 0 && bezierSurfaces[1]->secondCollapse != 3 && bezierSurfaces[1]->secondCollapse != 12 && bezierSurfaces[1]->secondCollapse != 15)
	{
		return false;
	}
	if (bezierSurfaces[2]->secondCollapse != 0 && bezierSurfaces[2]->secondCollapse != 3 && bezierSurfaces[2]->secondCollapse != 12 && bezierSurfaces[2]->secondCollapse != 15)
	{
		return false;
	}
	return true;
}

void ModelsManager::collapseMultiSelected(vector<SimplePoint*>& multiSelected)
{
	if (multiSelected.size() < 2)
		return;

	bool sentinel = false;
	XMVECTOR meanPos = (XMLoadFloat3(&(multiSelected[0]->GetPosition3())) + XMLoadFloat3(&(multiSelected[1]->GetPosition3()))) / 2.0f;
	vector<BezierSurface*> surfaces = GetBezierSurfaces();

	vector<SimplePoint*> points = surfaces[0]->GetNodes();
	for (int j = 0; j < points.size(); j++)
	{
		if (points[j]->m_id == multiSelected[0]->m_id)
		{
			if (j != 0 && j != 3 && j != 12 && j != 15)
			{
				sentinel = true;
				break;
			}
			if (surfaces[0]->firstCollapse == -1)
			{
				surfaces[0]->firstCollapse = j;
			}
			else
			{
				surfaces[0]->secondCollapse = j;
			}
		}
		if (points[j]->m_id == multiSelected[1]->m_id)
		{
			if (j != 0 && j != 3 && j != 12 && j != 15)
			{
				sentinel = true;
				break;
			}
			if (surfaces[0]->firstCollapse == -1)
			{
				surfaces[0]->firstCollapse = j;
			}
			else
			{
				surfaces[0]->secondCollapse = j;
			}
		}
	}

	points = surfaces[1]->GetNodes();
	for (int j = 0; j < points.size(); j++)
	{
		if (points[j]->m_id == multiSelected[0]->m_id)
		{
			if (j != 0 && j != 3 && j != 12 && j != 15)
			{
				sentinel = true;
				break;
			}
			if (surfaces[1]->firstCollapse == -1)
			{
				surfaces[1]->firstCollapse = j;
			}
			else
			{
				surfaces[1]->secondCollapse = j;
			}
		}
		if (points[j]->m_id == multiSelected[1]->m_id)
		{
			if (j != 0 && j != 3 && j != 12 && j != 15)
			{
				sentinel = true;
				break;
			}
			if (surfaces[1]->firstCollapse == -1)
			{
				surfaces[1]->firstCollapse = j;
			}
			else
			{
				surfaces[1]->secondCollapse = j;
			}
		}
	}
	if (surfaces.size() > 2)
	{
		points = surfaces[2]->GetNodes();
		for (int j = 0; j < points.size(); j++)
		{
			if (points[j]->m_id == multiSelected[0]->m_id)
			{
				if (j != 0 && j != 3 && j != 12 && j != 15)
				{
					sentinel = true;
					break;
				}
				if (surfaces[2]->firstCollapse == -1)
				{
					surfaces[2]->firstCollapse = j;
				}
				else
				{
					surfaces[2]->secondCollapse = j;
				}
			}
			if (points[j]->m_id == multiSelected[1]->m_id)
			{
				if (j != 0 && j != 3 && j != 12 && j != 15)
				{
					sentinel = true;
					break;
				}
				if (surfaces[2]->firstCollapse == -1)
				{
					surfaces[2]->firstCollapse = j;
				}
				else
				{
					surfaces[2]->secondCollapse = j;
				}
			}
		}
	}
	if (sentinel)
	{
		multiSelected[0]->ScaleDown();
		multiSelected[1]->ScaleDown();
		multiSelected.clear();
		return;
	}
	multiSelected[0]->SetPosition(meanPos);
	multiSelected[1]->SetPosition(meanPos);
	multiSelected[0]->ScaleDown();
	multiSelected[1]->ScaleDown();

	for (int i = 0; i < surfaces.size(); i++)
	{
		surfaces[i]->ReplaceNode(multiSelected[0], multiSelected[1]);

		surfaces[i]->Reset();
	}
	multiSelected[0]->SetPosition(XMFLOAT4(1000.0f, 0.0f, 0.0f, 0.0f));
	//RemoveModel(multiSelected[0]->m_id);
	multiSelected.clear();

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
	//first scale down
	vector<ModelClass*> oldActiveModels = GetActiveModels();
	map<int, BezierSurface*> tmp_activeBezier_surfaces;
	bool firstOccurence = true;
	for (int i = 0; i < oldActiveModels.size(); i++)
	{
		if (oldActiveModels[i]->m_Type == ModelType::SimplePointType)
			oldActiveModels[i]->ScaleDown();
	}
	//clear old active
	m_active_models.clear();
	//scale up newly selected
	for (int i = 0; i < activeModels.size(); i++)
	{
		AddActiveModel(activeModels[i]);
		if (m_models[activeModels[i]]->m_Type == ModelType::SimplePointType)
		{
			m_models[activeModels[i]]->ScaleUp();


			ModelClass* model = m_models[activeModels[i]];
			vector<BezierSurface*> surfaces = GetBezierSurfaces();
			for (int i = 0; i < surfaces.size(); i++)
			{
				vector<SimplePoint*> nodes = surfaces[i]->GetNodes();
				for (int j = 0; j < nodes.size(); j++)
				{
					if (nodes[j]->m_id == model->m_id)
					{
						tmp_activeBezier_surfaces.insert(pair<int, BezierSurface*>(surfaces[i]->m_id, surfaces[i]));
						break;
					}
				}
			}
		}
	}

	if (tmp_activeBezier_surfaces.size() == 2)
	{
		m_activeBezier_surfaces = tmp_activeBezier_surfaces;
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
	if (type == ModelType::BezierSurfaceType)
	{
		BezierSurface* surface = dynamic_cast<BezierSurface*>(m_models[id]);
		map<int, ModelClass*> models = GetModels();
		int idToRemove = -1;
		for (map<int, ModelClass*> ::iterator it = m_models.begin(); it != m_models.end(); it++)
		{
			if ((*it).second->m_Type == ModelType::IntersectionSurfaceType)
			{
				IntersectionSurface* intersectionSurface = dynamic_cast<IntersectionSurface*>((*it).second);
				vector<BezierSurface*> surfaces = intersectionSurface->GetSurfaces();
				if (surfaces[0]->m_id == surface->m_id)
					idToRemove = intersectionSurface->m_id;
				if (surfaces[1]->m_id == surface->m_id)
					idToRemove = intersectionSurface->m_id;
			}
		}
		if (idToRemove != -1)
			m_models.erase(idToRemove);
		m_bezier_surfaces.erase(id);
	}
	if (type == ModelType::BSplineSurfaceType)
		m_bspline_surfaces.erase(id);
	m_models.erase(id);
}

vector<ModelClass*> ModelsManager::GetActiveModels()
{
	vector<ModelClass*> activeModels;
	for (map<int, ModelClass*> ::iterator it = m_active_models.begin(); it != m_active_models.end(); it++)
	{
		activeModels.push_back((*it).second);
	}
	return activeModels;
}

vector<BezierCurve*> ModelsManager::GetBezierCurves()
{
	vector<BezierCurve*> curves;
	for (map<int, BezierCurve*> ::iterator it = m_bezier_curves.begin(); it != m_bezier_curves.end(); it++)
	{
		curves.push_back((*it).second);
	}
	return curves;
}

vector<BezierSurface*> ModelsManager::GetBezierSurfaces()
{
	vector<BezierSurface*> surfaces;
	for (map<int, BezierSurface*> ::iterator it = m_bezier_surfaces.begin(); it != m_bezier_surfaces.end(); it++)
	{
		surfaces.push_back((*it).second);
	}
	return surfaces;
}

vector<BSplineSurface*> ModelsManager::GetBSplineSurfaces()
{
	vector<BSplineSurface*>surfaces;
	for (map<int, BSplineSurface*> ::iterator it = m_bspline_surfaces.begin(); it != m_bspline_surfaces.end(); it++)
	{
		surfaces.push_back((*it).second);
	}
	return surfaces;
}

vector<GregorySurface*> ModelsManager::GetGregorySurfaces()
{
	vector<GregorySurface*>surfaces;
	for (map<int, GregorySurface*> ::iterator it = m_gregory_surfaces.begin(); it != m_gregory_surfaces.end(); it++)
	{
		surfaces.push_back((*it).second);
	}
	return surfaces;
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
static int op = 0;

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
	GregoryPatch* gregoryPatch;
	GregorySurface* gregorySurface;
	vector<SimplePoint*> points;
	IntersectionSurface* intersecionSurface;

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

	case GregoryPatchType:
		m_service.shaderIndex = 7;
		gregoryPatch = new GregoryPatch(m_service);
		gregoryPatch->Initialize();
		m_models.insert(pair<int, ModelClass*>(gregoryPatch->m_id, gregoryPatch));
		return gregoryPatch;
	case GregorySurfaceType:
		gregorySurface = new GregorySurface();
		m_models.insert(pair<int, ModelClass*>(gregorySurface->m_id, gregorySurface));
		m_gregory_surfaces.insert(pair<int, GregorySurface*>(gregorySurface->m_id, gregorySurface));
		return gregorySurface;
	case IntersectionSurfaceType:
		return createFakeIntersectionManual();

	//case ElipsoidType:
	//	m_service.shaderIndex = 1;
	//	m_models.push_back(new Elipsoid(m_service));
	//	break;
	default:
		break;
	}
	return NULL;
}




map<int, ModelClass*> ModelsManager::GetModels()
{
	map<int, ModelClass*> extendedSetOfModels;
	for (map<int, ModelClass*> ::iterator it = ++(m_models.begin()); it != m_models.end(); it++)
	{
		extendedSetOfModels.insert(*it);
		if ((*it).second->m_Type == ModelType::BezierC2Type || (*it).second->m_Type == ModelType::InterpolatedC2Type)
		{
			BezierC2Curve* curve = dynamic_cast<BezierC2Curve*>((*it).second);
			for (int i = 0; i < curve->m_Bezier.size(); i++)
			{
				extendedSetOfModels.insert(pair<int, ModelClass*>(curve->m_Bezier[i]->m_id, curve->m_Bezier[i]));
			}
		}
	}
	return extendedSetOfModels;
}

ModelClass* ModelsManager::GetModelById(int id)
{
	return m_models[id];
}
