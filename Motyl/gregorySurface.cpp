#include "gregorySurface.h"


GregorySurface::GregorySurface()
{
	m_Type = ModelType::GregorySurfaceType;
	ModelClass::Initialize();
}


GregorySurface::~GregorySurface()
{
}

GregorySurface::GregorySurface(const GregorySurface&)
{
}

void* GregorySurface::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void GregorySurface::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void GregorySurface::Reset()
{
	for (int i = 0; i < m_gregoryPatches.size(); i++)
	{
		m_gregoryPatches[i]->Reset();
	}
}

void GregorySurface::UpdateNode(SimplePoint* point)
{
	for (int i = 0; i < m_gregoryPatches.size(); i++)
	{
		vector<SimplePoint*> patchNodes = m_gregoryPatches[i]->GetNodes();
		for (int j = 0; j < patchNodes.size(); j++)
		{
			if (patchNodes[j]->m_id == point->m_id)
			{
				m_gregoryPatches[i]->Reset();
				break;
			}
		}
	}
}

void GregorySurface::AddPatch(GregoryPatch* patch)
{
	m_gregoryPatches.push_back(patch);
	//vector<SimplePoint*> patchNodes = patch->GetNodes();
	//for (int i = 0; i < patchNodes.size(); i++)
	//{
	//	m_nodes.insert(pair<int, SimplePoint*>(patchNodes[i]->m_id, patchNodes[i]));
	//}
}



TranslatedBezier GregorySurface::checkTranslatedPoint(SimplePoint* p)
{
	vector<SimplePoint*> nodes = m_bezierSurfaces[0]->GetNodes();
	for (int i = 0; i < nodes.size(); i++)
	{
		if (p->m_id == nodes[i]->m_id)
			return TranslatedBezier::left;
	}
	nodes = m_bezierSurfaces[1]->GetNodes();
	for (int i = 0; i < nodes.size(); i++)
	{
		if (p->m_id == nodes[i]->m_id)
			return TranslatedBezier::right;
	}
	nodes = m_bezierSurfaces[2]->GetNodes();
	for (int i = 0; i < nodes.size(); i++)
	{
		if (p->m_id == nodes[i]->m_id)
			return TranslatedBezier::bottom;
	}
}

void GregorySurface::Translate(SimplePoint* p)
{
	TranslatePoints(checkTranslatedPoint(p));
}

enum dirs{
	dir_3_15,//OK
	dir_15_3,//OK

	dir_12_15, //OK
	dir_15_12, //OK

	dir_0_3, //OK
	dir_3_0, //OK

	dir_0_12, //OK
	dir_12_0 //OK
};

void GregorySurface::TranslatePoints(TranslatedBezier direction)
{
	float bezierSurface_0_inner = 2.0f / 3.0f;
	float bezierSurface_0_outer = 1.0f;

	float bezierSurface_1_inner = 2.0f / 3.0f;
	float bezierSurface_1_outer = 1.0f;

	float bezierSurface_2_inner = 2.0f / 3.0f;
	float bezierSurface_2_outer = 1.0f;

	bool simple_0 = true;
	bool reverse_0 = false;

	bool simple_1 = true;
	bool reverse_1 = false;

	bool simple_2 = true;
	bool reverse_2 = false;

	dirs dir0, dir1, dir2;
	dir0 = dir1 = dir2 = dir_12_15;


	///////////////FIRST BEZIER//////////////

	if (m_bezierSurfaces[0]->firstCollapse == 12 && m_bezierSurfaces[0]->secondCollapse == 15)
	{
		dir0 = dir_15_12;
		reverse_0 = true;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 3 && m_bezierSurfaces[0]->secondCollapse == 0)
	{
		bezierSurface_0_inner = 1.0f / 3.0f;
		bezierSurface_0_outer = 0.0f;
		dir0 = dir_0_3;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 0 && m_bezierSurfaces[0]->secondCollapse == 3)
	{
		bezierSurface_0_inner = 1.0f / 3.0f;
		bezierSurface_0_outer = 0.0f;
		dir0 = dir_3_0;
		reverse_0 = true;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 12 && m_bezierSurfaces[0]->secondCollapse == 0)
	{
		bezierSurface_0_inner = 1.0f / 3.0f;
		bezierSurface_0_outer = 0.0f;
		simple_0 = false;
		dir0 = dir_0_12;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 0 && m_bezierSurfaces[0]->secondCollapse == 12)
	{
		bezierSurface_0_inner = 1.0f / 3.0f;
		bezierSurface_0_outer = 0.0f;
		simple_0 = false;
		reverse_0 = true;
		dir0 = dir_12_0;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 3 && m_bezierSurfaces[0]->secondCollapse == 15)
	{
		bezierSurface_0_inner = 2.0f / 3.0f;
		bezierSurface_0_outer = 1.0f;
		simple_0 = false;
		reverse_0 = true;
		dir0 = dir_15_3;
	}

	if (m_bezierSurfaces[0]->firstCollapse == 15 && m_bezierSurfaces[0]->secondCollapse == 3)
	{
		bezierSurface_0_inner = 2.0f / 3.0f;
		bezierSurface_0_outer = 1.0f;
		simple_0 = false;
		dir0 = dir_3_15;
	}




///////////////SECOND BEZIER//////////////
	if (m_bezierSurfaces[1]->firstCollapse == 12 && m_bezierSurfaces[1]->secondCollapse == 15)
	{
		dir1 = dir_15_12;
		reverse_1 = true;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 3 && m_bezierSurfaces[1]->secondCollapse == 0)
	{
		bezierSurface_1_inner = 1.0f / 3.0f;
		bezierSurface_1_outer = 0.0f;
		dir1 = dir_0_3;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 0 && m_bezierSurfaces[1]->secondCollapse == 3)
	{
		bezierSurface_1_inner = 1.0f / 3.0f;
		bezierSurface_1_outer = 0.0f;
		dir1 = dir_3_0;
		reverse_1 = true;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 12 && m_bezierSurfaces[1]->secondCollapse == 0)
	{
		bezierSurface_1_inner = 1.0f / 3.0f;
		bezierSurface_1_outer = 0.0f;
		simple_1 = false;
		dir1 = dir_0_12;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 0 && m_bezierSurfaces[1]->secondCollapse == 12)
	{
		bezierSurface_1_inner = 1.0f / 3.0f;
		bezierSurface_1_outer = 0.0f;
		simple_1 = false;
		reverse_1 = true;
		dir1 = dir_12_0;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 3 && m_bezierSurfaces[1]->secondCollapse == 15)
	{
		bezierSurface_1_inner = 2.0f / 3.0f;
		bezierSurface_1_outer = 1.0f;
		simple_1 = false;
		reverse_1 = true;
		dir1 = dir_15_3;
	}

	if (m_bezierSurfaces[1]->firstCollapse == 15 && m_bezierSurfaces[1]->secondCollapse == 3)
	{
		bezierSurface_1_inner = 2.0f / 3.0f;
		bezierSurface_1_outer = 1.0f;
		simple_1 = false;
		dir1 = dir_3_15;
	}


	///////////////THIRD BEZIER//////////////
	if (m_bezierSurfaces[2]->firstCollapse == 12 && m_bezierSurfaces[2]->secondCollapse == 15)
	{
		dir2 = dir_12_15;
		reverse_2 = true;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 3 && m_bezierSurfaces[2]->secondCollapse == 0)
	{
		bezierSurface_2_inner = 1.0f / 3.0f;
		bezierSurface_2_outer = 0.0f;
		dir2 = dir_3_0;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 0 && m_bezierSurfaces[2]->secondCollapse == 3)
	{
		bezierSurface_2_inner = 1.0f / 3.0f;
		bezierSurface_2_outer = 0.0f;
		dir2 = dir_0_3;
		reverse_2 = true;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 12 && m_bezierSurfaces[2]->secondCollapse == 0)
	{
		bezierSurface_2_inner = 1.0f / 3.0f;
		bezierSurface_2_outer = 0.0f;
		simple_2 = false;
		dir2 = dir_12_0;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 0 && m_bezierSurfaces[2]->secondCollapse == 12)
	{
		bezierSurface_2_inner = 1.0f / 3.0f;
		bezierSurface_2_outer = 0.0f;
		simple_2 = false;
		reverse_2 = true;
		dir2 = dir_0_12;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 3 && m_bezierSurfaces[2]->secondCollapse == 15)
	{
		bezierSurface_2_inner = 2.0f / 3.0f;
		bezierSurface_2_outer = 1.0f;
		simple_2 = false;
		reverse_2 = true;
		dir2 = dir_3_15;
	}

	if (m_bezierSurfaces[2]->firstCollapse == 15 && m_bezierSurfaces[2]->secondCollapse == 3)
	{
		bezierSurface_2_inner = 2.0f / 3.0f;
		bezierSurface_2_outer = 1.0f;
		simple_2 = false;
		dir2 = dir_15_3;
	}




	XMFLOAT4 centerBoundary0;
	if (simple_0)
	{
		centerBoundary0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f);
	}
	else
	{
		centerBoundary0 = m_bezierSurfaces[0]->Q(0.5f, bezierSurface_0_outer);
	}
	m_gregoryPatches[0]->p3->SetPosition(centerBoundary0);
	XMVECTOR P0_0 = XMLoadFloat4(&centerBoundary0);
	m_gregoryPatches[1]->p3->SetPosition(P0_0);

	XMFLOAT4 centerFirstInner0;
	if (simple_0)
	{
		centerFirstInner0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f);
	}
	else
	{
		centerFirstInner0 = m_bezierSurfaces[0]->Q(0.5f, bezierSurface_0_inner);
	}
	XMVECTOR P0_1 = 2 * XMLoadFloat4(&centerBoundary0) - XMLoadFloat4(&centerFirstInner0);
	m_gregoryPatches[0]->e3minus->SetPosition(P0_1);
	m_gregoryPatches[1]->e3plus->SetPosition(P0_1);

	XMVECTOR Q0 = (3 * P0_1 - P0_0) / 2.0f;
	////////////////////////
	XMFLOAT4 centerBoundary1;
	if (simple_1)
	{
		centerBoundary1 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f);
	}
	else
	{
		centerBoundary1 = m_bezierSurfaces[1]->Q(0.5f, bezierSurface_1_outer);
	}
	m_gregoryPatches[1]->p1->SetPosition(centerBoundary1);
	XMVECTOR P1_0 = XMLoadFloat4(&centerBoundary1);
	m_gregoryPatches[2]->p2->SetPosition(centerBoundary1);

	XMFLOAT4 centerFirstInner1;
	if (simple_1)
	{
		centerFirstInner1 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f);
	}
	else
	{
		centerFirstInner1 = m_bezierSurfaces[1]->Q(0.5f, bezierSurface_1_inner);
	}

	XMVECTOR P1_1 = 2 * XMLoadFloat4(&centerBoundary1) - XMLoadFloat4(&centerFirstInner1);
	m_gregoryPatches[1]->e1minus->SetPosition(P1_1);
	m_gregoryPatches[2]->e2plus->SetPosition(P1_1);

	XMVECTOR Q1 = (3 * P1_1 - P1_0) / 2.0f;

	////////////////////////
	XMFLOAT4 centerBoundary2;
	if (!simple_2)
	{
		centerBoundary2 = m_bezierSurfaces[2]->Q(0.5f, bezierSurface_2_outer);
	}
	else
	{
		centerBoundary2 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f);
	}
	m_gregoryPatches[2]->p0->SetPosition(centerBoundary2);
	XMVECTOR P2_0 = XMLoadFloat4(&centerBoundary2);
	m_gregoryPatches[0]->p1->SetPosition(centerBoundary2);

	XMFLOAT4 centerFirstInner2;
	if (!simple_2)
	{
		centerFirstInner2 = m_bezierSurfaces[2]->Q(0.5f, bezierSurface_2_inner);
	}
	else
	{
		centerFirstInner2 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f);
	}
	XMVECTOR P2_1 = 2 * XMLoadFloat4(&centerBoundary2) - XMLoadFloat4(&centerFirstInner2);
	m_gregoryPatches[2]->e0minus->SetPosition(P2_1);
	m_gregoryPatches[0]->e1plus->SetPosition(P2_1);

	XMVECTOR Q2 = (3 * P2_1 - P2_0) / 2.0f;

	XMVECTOR Q = (Q0 + Q1 + Q2) / 3.0f;
	m_gregoryPatches[0]->p2->SetPosition(Q);
	m_gregoryPatches[1]->p0->SetPosition(Q);
	m_gregoryPatches[2]->p3->SetPosition(Q);

	m_gregoryPatches[0]->e2plus->SetPosition((2 * Q0 + Q) / 3.0f);
	m_gregoryPatches[1]->e0minus->SetPosition((2 * Q0 + Q) / 3.0f);
	m_gregoryPatches[1]->e0plus->SetPosition((2 * Q1 + Q) / 3.0f);
	m_gregoryPatches[2]->e3minus->SetPosition((2 * Q1 + Q) / 3.0f);
	m_gregoryPatches[2]->e3plus->SetPosition((2 * Q2 + Q) / 3.0f);
	m_gregoryPatches[0]->e2minus->SetPosition((2 * Q2 + Q) / 3.0f);


	/////Z UKLADU//////
	XMFLOAT4 downBoundary0;
	XMFLOAT4 Boundary_1_3_0;
	XMFLOAT4 Boundary_2_3_0;

	if (simple_0)
	{
		downBoundary0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.0f);
		Boundary_1_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (1.0f / 3.0f));
		Boundary_2_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (2.0f / 3.0f));
		if (reverse_0)
		{
			downBoundary0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.0f + 0.5f);
			Boundary_1_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_2_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
			centerBoundary0 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 1.0f);
			swap(centerBoundary0, downBoundary0);
			swap(Boundary_1_3_0, Boundary_2_3_0);
		}
	}
	else
	{
		downBoundary0 = m_bezierSurfaces[0]->Q(0.0f, bezierSurface_0_outer);
		Boundary_1_3_0 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f), bezierSurface_0_outer);
		Boundary_2_3_0 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f), bezierSurface_0_outer);
		if (reverse_0)
		{
			downBoundary0 = m_bezierSurfaces[0]->Q(0.0f + 0.5f, bezierSurface_0_outer);
			Boundary_1_3_0 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_0_outer);
			Boundary_2_3_0 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_0_outer);
			centerBoundary0 = m_bezierSurfaces[0]->Q(1.0f, bezierSurface_0_outer);
			swap(centerBoundary0, downBoundary0);
			swap(Boundary_1_3_0, Boundary_2_3_0);
		}
	}
	XMVECTOR y0_0 = XMLoadFloat4(&downBoundary0);
	XMVECTOR y3_0 = XMLoadFloat4(&centerBoundary0);
	XMVECTOR y1_0 = (-5 * y0_0 + 18 * XMLoadFloat4(&Boundary_1_3_0) - 9 * XMLoadFloat4(&Boundary_2_3_0) + 2 * y3_0) / 6.0f;
	XMVECTOR y2_0 = (2 * y0_0 - 9 * XMLoadFloat4(&Boundary_1_3_0) + 18 * XMLoadFloat4(&Boundary_2_3_0) - 5 * y3_0) / 6.0f;

	m_gregoryPatches[0]->p0->SetPosition(y0_0);
	m_gregoryPatches[0]->e0minus->SetPosition(y1_0);
	m_gregoryPatches[0]->e3plus->SetPosition(y2_0);
	m_gregoryPatches[0]->p3->SetPosition(y3_0); //to ju¿ by³o


	XMFLOAT4 downBoundary1;
	XMFLOAT4 Boundary_1_3_1;
	XMFLOAT4 Boundary_2_3_1;
	XMFLOAT4 upBoundary1;
	if (simple_0)
	{
		downBoundary1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.0f + 0.5f);
		Boundary_1_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_2_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
		upBoundary1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 1.0f);
		if (reverse_0)
		{
			downBoundary1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.0f );
			Boundary_1_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (1.0f / 3.0f));
			Boundary_2_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f * (2.0f / 3.0f));
			upBoundary1 = m_bezierSurfaces[0]->Q(bezierSurface_0_outer, 0.5f);
			swap(upBoundary1, downBoundary1);
			swap(Boundary_1_3_1, Boundary_2_3_1);
		}
	}
	else
	{
		downBoundary1 = m_bezierSurfaces[0]->Q(0.0f + 0.5f, bezierSurface_0_outer);
		Boundary_1_3_1 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_0_outer);
		Boundary_2_3_1 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_0_outer);
		upBoundary1 = m_bezierSurfaces[0]->Q(1.0f, bezierSurface_0_outer);
		if (reverse_0)
		{
			downBoundary1 = m_bezierSurfaces[0]->Q(0.0f, bezierSurface_0_outer);
			Boundary_1_3_1 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f), bezierSurface_0_outer);
			Boundary_2_3_1 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f), bezierSurface_0_outer);
			upBoundary1 = m_bezierSurfaces[0]->Q(0.5f, bezierSurface_0_outer);
			swap(upBoundary1, downBoundary1);
			swap(Boundary_1_3_1, Boundary_2_3_1);
		}
	}
	XMVECTOR y0_1 = XMLoadFloat4(&downBoundary1);
	XMVECTOR y3_1 = XMLoadFloat4(&upBoundary1);
	XMVECTOR y1_1 = (-5 * y0_1 + 18 * XMLoadFloat4(&Boundary_1_3_1) - 9 * XMLoadFloat4(&Boundary_2_3_1) + 2 * y3_1) / 6.0f;
	XMVECTOR y2_1 = (2 * y0_1 - 9 * XMLoadFloat4(&Boundary_1_3_1) + 18 * XMLoadFloat4(&Boundary_2_3_1) - 5 * y3_1) / 6.0f;

	m_gregoryPatches[1]->p3->SetPosition(y0_1); //to ju¿ by³o
	m_gregoryPatches[1]->e3minus->SetPosition(y1_1);
	m_gregoryPatches[1]->e2plus->SetPosition(y2_1);
	m_gregoryPatches[1]->p2->SetPosition(y3_1);


	///////START////
	XMFLOAT4 downBoundary2;
	XMFLOAT4 Boundary_1_3_2;
	XMFLOAT4 Boundary_2_3_2;
	XMFLOAT4 upBoundary2;

	if (simple_1)
	{
		downBoundary2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.0f + 0.5f);
		Boundary_1_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_2_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
		upBoundary2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * 1.0f + 0.5f);

		if (reverse_1)
		{
			downBoundary2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.0f);
			Boundary_1_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (1.0f / 3.0f));
			Boundary_2_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (2.0f / 3.0f));
			upBoundary2 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * 1.0f);
			swap(upBoundary2, downBoundary2);
			swap(Boundary_1_3_2, Boundary_2_3_2);
		}
	}
	else
	{

		downBoundary2 = m_bezierSurfaces[1]->Q(0.0f + 0.5f, bezierSurface_1_outer);
		Boundary_1_3_2 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_1_outer);
		Boundary_2_3_2 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_1_outer);
		upBoundary2 = m_bezierSurfaces[1]->Q(1.0f, bezierSurface_1_outer);
		if (reverse_1)
		{
			downBoundary2 = m_bezierSurfaces[1]->Q(0.0f, bezierSurface_1_outer);
			Boundary_1_3_2 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f), bezierSurface_1_outer);
			Boundary_2_3_2 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f), bezierSurface_1_outer);
			upBoundary2 = m_bezierSurfaces[1]->Q(0.5f * 1.0f, bezierSurface_1_outer);

			swap(upBoundary2, downBoundary2);
			swap(Boundary_1_3_2, Boundary_2_3_2);
		}
	}
	///END///



	XMVECTOR y0_2 = XMLoadFloat4(&downBoundary2);
	XMVECTOR y3_2 = XMLoadFloat4(&upBoundary2);
	XMVECTOR y1_2 = (-5 * y0_2 + 18 * XMLoadFloat4(&Boundary_1_3_2) - 9 * XMLoadFloat4(&Boundary_2_3_2) + 2 * y3_2) / 6.0f;
	XMVECTOR y2_2 = (2 * y0_2 - 9 * XMLoadFloat4(&Boundary_1_3_2) + 18 * XMLoadFloat4(&Boundary_2_3_2) - 5 * y3_2) / 6.0f;

	m_gregoryPatches[1]->p1->SetPosition(y0_2);
	m_gregoryPatches[1]->e1plus->SetPosition(y1_2);
	m_gregoryPatches[1]->e2minus->SetPosition(y2_2);
	m_gregoryPatches[1]->p2->SetPosition(y3_2); //to ju¿ by³o

	XMFLOAT4 downBoundary3;
	XMFLOAT4 Boundary_1_3_3;
	XMFLOAT4 Boundary_2_3_3;
	XMFLOAT4 upBoundary3;

	if (simple_1)
	{


		downBoundary3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.0f);
		Boundary_1_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (1.0f / 3.0f));
		Boundary_2_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (2.0f / 3.0f));
		upBoundary3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * 1.0f);
		
		if (reverse_1)
		{
			downBoundary3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.0f + 0.5f);
			Boundary_1_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_2_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
			upBoundary3 = m_bezierSurfaces[1]->Q(bezierSurface_1_outer, 0.5f * 1.0f + 0.5f);

			swap(upBoundary3, downBoundary3);
			swap(Boundary_1_3_3, Boundary_2_3_3);
		}
	}
	else
	{

		downBoundary3 = m_bezierSurfaces[1]->Q(0.0f, bezierSurface_1_outer);
		Boundary_1_3_3 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f), bezierSurface_1_outer);
		Boundary_2_3_3 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f), bezierSurface_1_outer);
		upBoundary3 = m_bezierSurfaces[1]->Q(0.5f * 1.0f, bezierSurface_1_outer);
		if (reverse_1)
		{
			downBoundary3 = m_bezierSurfaces[1]->Q(0.0f + 0.5f, bezierSurface_1_outer);
			Boundary_1_3_3 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_1_outer);
			Boundary_2_3_3 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_1_outer);
			upBoundary3 = m_bezierSurfaces[1]->Q(1.0f, bezierSurface_1_outer);

			swap(upBoundary3, downBoundary3);
			swap(Boundary_1_3_3, Boundary_2_3_3);
		}
	}



	XMVECTOR y0_3 = XMLoadFloat4(&downBoundary3);
	XMVECTOR y3_3 = XMLoadFloat4(&upBoundary3);
	XMVECTOR y1_3 = (-5 * y0_3 + 18 * XMLoadFloat4(&Boundary_1_3_3) - 9 * XMLoadFloat4(&Boundary_2_3_3) + 2 * y3_3) / 6.0f;
	XMVECTOR y2_3 = (2 * y0_3 - 9 * XMLoadFloat4(&Boundary_1_3_3) + 18 * XMLoadFloat4(&Boundary_2_3_3) - 5 * y3_3) / 6.0f;

	m_gregoryPatches[2]->p1->SetPosition(y0_3);
	m_gregoryPatches[2]->e1plus->SetPosition(y1_3);
	m_gregoryPatches[2]->e2minus->SetPosition(y2_3);
	m_gregoryPatches[2]->p2->SetPosition(y3_3); //to ju¿ by³o

	XMFLOAT4 downBoundary4;
	XMFLOAT4 Boundary_1_3_4;
	XMFLOAT4 Boundary_2_3_4;
	XMFLOAT4 upBoundary4;

	if (!simple_2)
	{
		downBoundary4 = m_bezierSurfaces[2]->Q(0.0f + 0.5f, bezierSurface_2_outer);
		Boundary_1_3_4 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_2_outer);
		Boundary_2_3_4 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_2_outer);
		upBoundary4 = m_bezierSurfaces[2]->Q(1.0f, bezierSurface_2_outer);
		if (reverse_2)
		{

			downBoundary4 = m_bezierSurfaces[2]->Q(0.0f, bezierSurface_2_outer);
			Boundary_1_3_4 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f), bezierSurface_2_outer);
			Boundary_2_3_4 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f), bezierSurface_2_outer);
			upBoundary4 = m_bezierSurfaces[2]->Q(0.5f * 1.0f, bezierSurface_2_outer);
			swap(upBoundary4, downBoundary4);
			swap(Boundary_1_3_4, Boundary_2_3_4);
		}
	}
	else
	{


		downBoundary4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.0f + 0.5f);
		Boundary_1_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_2_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
		upBoundary4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * 1.0f + 0.5f);

		if (reverse_2)
		{
			downBoundary4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.0f);
			Boundary_1_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (1.0f / 3.0f));
			Boundary_2_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (2.0f / 3.0f));
			upBoundary4 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * 1.0f);

			swap(upBoundary4, downBoundary4);
			swap(Boundary_1_3_4, Boundary_2_3_4);
		}

	}


	XMVECTOR y0_4 = XMLoadFloat4(&downBoundary4);
	XMVECTOR y3_4 = XMLoadFloat4(&upBoundary4);
	XMVECTOR y1_4 = (-5 * y0_4 + 18 * XMLoadFloat4(&Boundary_1_3_4) - 9 * XMLoadFloat4(&Boundary_2_3_4) + 2 * y3_4) / 6.0f;
	XMVECTOR y2_4 = (2 * y0_4 - 9 * XMLoadFloat4(&Boundary_1_3_4) + 18 * XMLoadFloat4(&Boundary_2_3_4) - 5 * y3_4) / 6.0f;

	m_gregoryPatches[2]->p0->SetPosition(y0_4); //to ju¿ by³o
	m_gregoryPatches[2]->e0plus->SetPosition(y1_4);
	m_gregoryPatches[2]->e1minus->SetPosition(y2_4);
	m_gregoryPatches[2]->p1->SetPosition(y3_4);

	XMFLOAT4 downBoundary5 = m_bezierSurfaces[2]->Q(0.0f, bezierSurface_2_outer);
	XMFLOAT4 Boundary_1_3_5 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f), bezierSurface_2_outer);
	XMFLOAT4 Boundary_2_3_5 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f), bezierSurface_2_outer);
	XMFLOAT4 upBoundary5 = m_bezierSurfaces[2]->Q(0.5f, bezierSurface_2_outer);

	if (!simple_2)
	{
		downBoundary5 = m_bezierSurfaces[2]->Q(0.0f, bezierSurface_2_outer);
		Boundary_1_3_5 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f), bezierSurface_2_outer);
		Boundary_2_3_5 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f), bezierSurface_2_outer);
		upBoundary5 = m_bezierSurfaces[2]->Q(0.5f, bezierSurface_2_outer);
		if (reverse_2)
		{

			downBoundary5 = m_bezierSurfaces[2]->Q(0.0f + 0.5f, bezierSurface_2_outer);
			Boundary_1_3_5 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_2_outer);
			Boundary_2_3_5 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_2_outer);
			upBoundary5 = m_bezierSurfaces[2]->Q(0.5f * 1.0f + 0.5f, bezierSurface_2_outer);
			swap(upBoundary5, downBoundary5);
			swap(Boundary_1_3_5, Boundary_2_3_5);
		}
	}
	else
	{
		downBoundary5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.0f);
		Boundary_1_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (1.0f / 3.0f));
		Boundary_2_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (2.0f / 3.0f));
		upBoundary5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * 1.0f);

		if (reverse_2)
		{
			downBoundary5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.0f + 0.5f);
			Boundary_1_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_2_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * (2.0f / 3.0f) + 0.5f);
			upBoundary5 = m_bezierSurfaces[2]->Q(bezierSurface_2_outer, 0.5f * 1.0f + 0.5f);

			swap(upBoundary5, downBoundary5);
			swap(Boundary_1_3_5, Boundary_2_3_5);
		}

	}

	XMVECTOR y0_5 = XMLoadFloat4(&downBoundary5);
	XMVECTOR y3_5 = XMLoadFloat4(&upBoundary5);
	XMVECTOR y1_5 = (-5 * y0_5 + 18 * XMLoadFloat4(&Boundary_1_3_5) - 9 * XMLoadFloat4(&Boundary_2_3_5) + 2 * y3_5) / 6.0f;
	XMVECTOR y2_5 = (2 * y0_5 - 9 * XMLoadFloat4(&Boundary_1_3_5) + 18 * XMLoadFloat4(&Boundary_2_3_5) - 5 * y3_5) / 6.0f;

	m_gregoryPatches[0]->p0->SetPosition(y0_5); //to ju¿ by³o
	m_gregoryPatches[0]->e0plus->SetPosition(y1_5);
	m_gregoryPatches[0]->e1minus->SetPosition(y2_5);
	m_gregoryPatches[0]->p1->SetPosition(y3_5);

	XMFLOAT4 Boundary_firstInner_0_3_0;
	XMFLOAT4 Boundary_firstInner_1_3_0;
	XMFLOAT4 Boundary_firstInner_2_3_0;
	XMFLOAT4 Boundary_firstInner_3_3_0;


	////////ŒRODKI////////////
	if (simple_0)
	{
		Boundary_firstInner_0_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.0f);
		Boundary_firstInner_1_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (1.0f / 3.0f));
		Boundary_firstInner_2_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (2.0f / 3.0f));
		Boundary_firstInner_3_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (3.0f / 3.0f));
		if (reverse_0)
		{
			Boundary_firstInner_0_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.0f + 0.5f);
			Boundary_firstInner_1_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_firstInner_2_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
			Boundary_firstInner_3_3_0 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (3.0f / 3.0f) + 0.5f);
			swap(Boundary_firstInner_0_3_0, Boundary_firstInner_3_3_0);
			swap(Boundary_firstInner_1_3_0, Boundary_firstInner_2_3_0);
		}
	}
	else
	{
		Boundary_firstInner_0_3_0 = m_bezierSurfaces[0]->Q(0.0f, bezierSurface_0_inner);
		Boundary_firstInner_1_3_0 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f), bezierSurface_0_inner);
		Boundary_firstInner_2_3_0 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f), bezierSurface_0_inner);
		Boundary_firstInner_3_3_0 = m_bezierSurfaces[0]->Q(0.5f * (3.0f / 3.0f), bezierSurface_0_inner);
		if (reverse_0)
		{
			Boundary_firstInner_0_3_0 = m_bezierSurfaces[0]->Q(0.0f + 0.5f, bezierSurface_0_inner);
			Boundary_firstInner_1_3_0 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
			Boundary_firstInner_2_3_0 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
			Boundary_firstInner_3_3_0 = m_bezierSurfaces[0]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
			swap(Boundary_firstInner_0_3_0, Boundary_firstInner_3_3_0);
			swap(Boundary_firstInner_1_3_0, Boundary_firstInner_2_3_0);
		}
	}
	XMVECTOR y0_firstInner_0 = XMLoadFloat4(&Boundary_firstInner_0_3_0);
	XMVECTOR y3_firstInner_0 = XMLoadFloat4(&Boundary_firstInner_3_3_0);
	XMVECTOR y1_firstInner_0 = (-5 * y0_firstInner_0 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_0) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_0) + 2 * y3_firstInner_0) / 6.0f;
	XMVECTOR y2_firstInner_0 = (2 * y0_firstInner_0 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_0) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_0) - 5 * y3_firstInner_0) / 6.0f;

	XMVECTOR f0minus = 2 * y1_0 - y1_firstInner_0; // XMLoadFloat4(&FirstInner_1_3_0);
	m_gregoryPatches[0]->f0minus->SetPosition(f0minus);

	XMVECTOR f3plus = 2 * y2_0 - y2_firstInner_0;//XMLoadFloat4(&FirstInner_2_3_0);
	m_gregoryPatches[0]->f3plus->SetPosition(f3plus);

	XMFLOAT4 Boundary_firstInner_0_3_1;
	XMFLOAT4 Boundary_firstInner_1_3_1;
	XMFLOAT4 Boundary_firstInner_2_3_1;
	XMFLOAT4 Boundary_firstInner_3_3_1;


	if (simple_0)
	{
		Boundary_firstInner_0_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.0f + 0.5f);
		Boundary_firstInner_1_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_firstInner_2_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
		Boundary_firstInner_3_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (3.0f / 3.0f) + 0.5f);
		if (reverse_0)
		{
			Boundary_firstInner_0_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.0f);
			Boundary_firstInner_1_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (1.0f / 3.0f));
			Boundary_firstInner_2_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (2.0f / 3.0f));
			Boundary_firstInner_3_3_1 = m_bezierSurfaces[0]->Q(bezierSurface_0_inner, 0.5f * (3.0f / 3.0f));
			swap(Boundary_firstInner_0_3_1, Boundary_firstInner_3_3_1);
			swap(Boundary_firstInner_1_3_1, Boundary_firstInner_2_3_1);
		}
	}
	else
	{
		Boundary_firstInner_0_3_1 = m_bezierSurfaces[0]->Q(0.0f + 0.5f, bezierSurface_0_inner);
		Boundary_firstInner_1_3_1 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
		Boundary_firstInner_2_3_1 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
		Boundary_firstInner_3_3_1 = m_bezierSurfaces[0]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_0_inner);
		if (reverse_0)
		{
			Boundary_firstInner_0_3_1 = m_bezierSurfaces[0]->Q(0.0f, bezierSurface_0_inner);
			Boundary_firstInner_1_3_1 = m_bezierSurfaces[0]->Q(0.5f * (1.0f / 3.0f), bezierSurface_0_inner);
			Boundary_firstInner_2_3_1 = m_bezierSurfaces[0]->Q(0.5f * (2.0f / 3.0f), bezierSurface_0_inner);
			Boundary_firstInner_3_3_1 = m_bezierSurfaces[0]->Q(0.5f * (3.0f / 3.0f), bezierSurface_0_inner);
			swap(Boundary_firstInner_0_3_1, Boundary_firstInner_3_3_1);
			swap(Boundary_firstInner_1_3_1, Boundary_firstInner_2_3_1);
		}
	}
	XMVECTOR y0_firstInner_1 = XMLoadFloat4(&Boundary_firstInner_0_3_1);
	XMVECTOR y3_firstInner_1 = XMLoadFloat4(&Boundary_firstInner_3_3_1);
	XMVECTOR y1_firstInner_1 = (-5 * y0_firstInner_1 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_1) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_1) + 2 * y3_firstInner_1) / 6.0f;
	XMVECTOR y2_firstInner_1 = (2 * y0_firstInner_1 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_1) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_1) - 5 * y3_firstInner_1) / 6.0f;

	XMVECTOR f3minus = 2 * y1_1 - y1_firstInner_1;//XMLoadFloat4(&FirstInner_1_3_1);
	m_gregoryPatches[1]->f3minus->SetPosition(f3minus);

	XMVECTOR f2plus = 2 * y2_1 - y2_firstInner_1;// XMLoadFloat4(&FirstInner_2_3_1);
	m_gregoryPatches[1]->f2plus->SetPosition(f2plus);


	XMFLOAT4 Boundary_firstInner_0_3_2;
	XMFLOAT4 Boundary_firstInner_1_3_2;
	XMFLOAT4 Boundary_firstInner_2_3_2;
	XMFLOAT4 Boundary_firstInner_3_3_2;

	if (simple_1)
	{
		Boundary_firstInner_0_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.0f + 0.5f);
		Boundary_firstInner_1_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_firstInner_2_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
		Boundary_firstInner_3_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (3.0f / 3.0f) + 0.5f);
		if (reverse_1)
		{
			Boundary_firstInner_0_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.0f);
			Boundary_firstInner_1_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (1.0f / 3.0f));
			Boundary_firstInner_2_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (2.0f / 3.0f));
			Boundary_firstInner_3_3_2 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (3.0f / 3.0f));
			swap(Boundary_firstInner_0_3_2, Boundary_firstInner_3_3_2);
			swap(Boundary_firstInner_1_3_2, Boundary_firstInner_2_3_2);
		}
	}
	else
	{
		Boundary_firstInner_0_3_2 = m_bezierSurfaces[1]->Q(0.0f + 0.5f, bezierSurface_1_inner);
		Boundary_firstInner_1_3_2 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_1_inner);
		Boundary_firstInner_2_3_2 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_1_inner);
		Boundary_firstInner_3_3_2 = m_bezierSurfaces[1]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_1_inner);
		if (reverse_1)
		{
			Boundary_firstInner_0_3_2 = m_bezierSurfaces[1]->Q(0.0f, bezierSurface_0_inner);
			Boundary_firstInner_1_3_2 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f), bezierSurface_1_inner);
			Boundary_firstInner_2_3_2 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f), bezierSurface_1_inner);
			Boundary_firstInner_3_3_2 = m_bezierSurfaces[1]->Q(0.5f * (3.0f / 3.0f), bezierSurface_1_inner);
			swap(Boundary_firstInner_0_3_2, Boundary_firstInner_3_3_2);
			swap(Boundary_firstInner_1_3_2, Boundary_firstInner_2_3_2);
		}
	}


	XMVECTOR y0_firstInner_2 = XMLoadFloat4(&Boundary_firstInner_0_3_2);
	XMVECTOR y3_firstInner_2 = XMLoadFloat4(&Boundary_firstInner_3_3_2);
	XMVECTOR y1_firstInner_2 = (-5 * y0_firstInner_2 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_2) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_2) + 2 * y3_firstInner_2) / 6.0f;
	XMVECTOR y2_firstInner_2 = (2 * y0_firstInner_2 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_2) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_2) - 5 * y3_firstInner_2) / 6.0f;

	XMVECTOR f1plus = 2 * y1_2 - y1_firstInner_2;// XMLoadFloat4(&FirstInner_1_3_2);
	m_gregoryPatches[1]->f1plus->SetPosition(f1plus);

	XMVECTOR f2minus = 2 * y2_2 - y2_firstInner_2;// XMLoadFloat4(&FirstInner_2_3_2);
	m_gregoryPatches[1]->f2minus->SetPosition(f2minus);

	XMFLOAT4 Boundary_firstInner_0_3_3;
	XMFLOAT4 Boundary_firstInner_1_3_3;
	XMFLOAT4 Boundary_firstInner_2_3_3;
	XMFLOAT4 Boundary_firstInner_3_3_3;

	if (simple_1)
	{
		Boundary_firstInner_0_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.0f);
		Boundary_firstInner_1_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (1.0f / 3.0f));
		Boundary_firstInner_2_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (2.0f / 3.0f));
		Boundary_firstInner_3_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (3.0f / 3.0f));
		if (reverse_1)
		{
			Boundary_firstInner_0_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.0f + 0.5f);
			Boundary_firstInner_1_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_firstInner_2_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
			Boundary_firstInner_3_3_3 = m_bezierSurfaces[1]->Q(bezierSurface_1_inner, 0.5f * (3.0f / 3.0f) + 0.5f);

			swap(Boundary_firstInner_0_3_3, Boundary_firstInner_3_3_3);
			swap(Boundary_firstInner_1_3_3, Boundary_firstInner_2_3_3);
		}
	}
	else
	{
		Boundary_firstInner_0_3_3 = m_bezierSurfaces[1]->Q(0.0f, bezierSurface_1_inner);
		Boundary_firstInner_1_3_3 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f), bezierSurface_1_inner);
		Boundary_firstInner_2_3_3 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f), bezierSurface_1_inner);
		Boundary_firstInner_3_3_3 = m_bezierSurfaces[1]->Q(0.5f * (3.0f / 3.0f), bezierSurface_1_inner);
		if (reverse_1)
		{
			Boundary_firstInner_0_3_3 = m_bezierSurfaces[1]->Q(0.0f + 0.5f, bezierSurface_1_inner);
			Boundary_firstInner_1_3_3 = m_bezierSurfaces[1]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_1_inner);
			Boundary_firstInner_2_3_3 = m_bezierSurfaces[1]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_1_inner);
			Boundary_firstInner_3_3_3 = m_bezierSurfaces[1]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_1_inner);

			swap(Boundary_firstInner_0_3_3, Boundary_firstInner_3_3_3);
			swap(Boundary_firstInner_1_3_3, Boundary_firstInner_2_3_3);
		}
	}

	XMVECTOR y0_firstInner_3 = XMLoadFloat4(&Boundary_firstInner_0_3_3);
	XMVECTOR y3_firstInner_3 = XMLoadFloat4(&Boundary_firstInner_3_3_3);
	XMVECTOR y1_firstInner_3 = (-5 * y0_firstInner_3 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_3) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_3) + 2 * y3_firstInner_3) / 6.0f;
	XMVECTOR y2_firstInner_3 = (2 * y0_firstInner_3 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_3) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_3) - 5 * y3_firstInner_3) / 6.0f;

	f1plus = 2 * y1_3 - y1_firstInner_3;// XMLoadFloat4(&FirstInner_1_3_3);
	m_gregoryPatches[2]->f1plus->SetPosition(f1plus);

	f2minus = 2 * y2_3 - y2_firstInner_3;// XMLoadFloat4(&FirstInner_2_3_3);
	m_gregoryPatches[2]->f2minus->SetPosition(f2minus);

	XMFLOAT4 Boundary_firstInner_0_3_4;
	XMFLOAT4 Boundary_firstInner_1_3_4;
	XMFLOAT4 Boundary_firstInner_2_3_4;
	XMFLOAT4 Boundary_firstInner_3_3_4;

	if (!simple_2)
	{

		Boundary_firstInner_0_3_4 = m_bezierSurfaces[2]->Q(0.0f + 0.5f, bezierSurface_2_inner);
		Boundary_firstInner_1_3_4 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_2_inner);
		Boundary_firstInner_2_3_4 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_2_inner);
		Boundary_firstInner_3_3_4 = m_bezierSurfaces[2]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_2_inner);

		if (reverse_2)
		{
			Boundary_firstInner_0_3_4 = m_bezierSurfaces[2]->Q(0.0f, bezierSurface_2_inner);
			Boundary_firstInner_1_3_4 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f), bezierSurface_2_inner);
			Boundary_firstInner_2_3_4 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f), bezierSurface_2_inner);
			Boundary_firstInner_3_3_4 = m_bezierSurfaces[2]->Q(0.5f * (3.0f / 3.0f), bezierSurface_2_inner);

			swap(Boundary_firstInner_0_3_4, Boundary_firstInner_3_3_4);
			swap(Boundary_firstInner_1_3_4, Boundary_firstInner_2_3_4);
		}
	}
	else
	{
		Boundary_firstInner_0_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.0f + 0.5f);
		Boundary_firstInner_1_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
		Boundary_firstInner_2_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
		Boundary_firstInner_3_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (3.0f / 3.0f) + 0.5f);

		if (reverse_2)
		{
			Boundary_firstInner_0_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.0f);
			Boundary_firstInner_1_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (1.0f / 3.0f));
			Boundary_firstInner_2_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (2.0f / 3.0f));
			Boundary_firstInner_3_3_4 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (3.0f / 3.0f));

			swap(Boundary_firstInner_0_3_4, Boundary_firstInner_3_3_4);
			swap(Boundary_firstInner_1_3_4, Boundary_firstInner_2_3_4);
		}

	}

	XMVECTOR y0_firstInner_4 = XMLoadFloat4(&Boundary_firstInner_0_3_4);
	XMVECTOR y3_firstInner_4 = XMLoadFloat4(&Boundary_firstInner_3_3_4);
	XMVECTOR y1_firstInner_4 = (-5 * y0_firstInner_4 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_4) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_4) + 2 * y3_firstInner_4) / 6.0f;
	XMVECTOR y2_firstInner_4 = (2 * y0_firstInner_4 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_4) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_4) - 5 * y3_firstInner_4) / 6.0f;

	XMVECTOR f0plus = 2 * y1_4 - y1_firstInner_4;// XMLoadFloat4(&FirstInner_1_3_4);
	m_gregoryPatches[2]->f0plus->SetPosition(f0plus);

	XMVECTOR f1minus = 2 * y2_4 - y2_firstInner_4;// XMLoadFloat4(&FirstInner_2_3_4);
	m_gregoryPatches[2]->f1minus->SetPosition(f1minus);

	XMFLOAT4 Boundary_firstInner_0_3_5;
	XMFLOAT4 Boundary_firstInner_1_3_5;
	XMFLOAT4 Boundary_firstInner_2_3_5;
	XMFLOAT4 Boundary_firstInner_3_3_5;

	if (!simple_2)
	{


		Boundary_firstInner_0_3_5 = m_bezierSurfaces[2]->Q(0.0f, bezierSurface_2_inner);
		Boundary_firstInner_1_3_5 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f), bezierSurface_2_inner);
		Boundary_firstInner_2_3_5 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f), bezierSurface_2_inner);
		Boundary_firstInner_3_3_5 = m_bezierSurfaces[2]->Q(0.5f * (3.0f / 3.0f), bezierSurface_2_inner);


		if (reverse_2)
		{
			Boundary_firstInner_0_3_5 = m_bezierSurfaces[2]->Q(0.0f + 0.5f, bezierSurface_2_inner);
			Boundary_firstInner_1_3_5 = m_bezierSurfaces[2]->Q(0.5f * (1.0f / 3.0f) + 0.5f, bezierSurface_2_inner);
			Boundary_firstInner_2_3_5 = m_bezierSurfaces[2]->Q(0.5f * (2.0f / 3.0f) + 0.5f, bezierSurface_2_inner);
			Boundary_firstInner_3_3_5 = m_bezierSurfaces[2]->Q(0.5f * (3.0f / 3.0f) + 0.5f, bezierSurface_2_inner);


			swap(Boundary_firstInner_0_3_5, Boundary_firstInner_3_3_5);
			swap(Boundary_firstInner_1_3_5, Boundary_firstInner_2_3_5);
		}
	}
	else
	{
		
		Boundary_firstInner_0_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.0f);
		Boundary_firstInner_1_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (1.0f / 3.0f));
		Boundary_firstInner_2_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (2.0f / 3.0f));
		Boundary_firstInner_3_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (3.0f / 3.0f));


		if (reverse_2)
		{
			Boundary_firstInner_0_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.0f + 0.5f);
			Boundary_firstInner_1_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (1.0f / 3.0f) + 0.5f);
			Boundary_firstInner_2_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (2.0f / 3.0f) + 0.5f);
			Boundary_firstInner_3_3_5 = m_bezierSurfaces[2]->Q(bezierSurface_2_inner, 0.5f * (3.0f / 3.0f) + 0.5f);

			swap(Boundary_firstInner_0_3_5, Boundary_firstInner_3_3_5);
			swap(Boundary_firstInner_1_3_5, Boundary_firstInner_2_3_5);
		}

	}

	XMVECTOR y0_firstInner_5 = XMLoadFloat4(&Boundary_firstInner_0_3_5);
	XMVECTOR y3_firstInner_5 = XMLoadFloat4(&Boundary_firstInner_3_3_5);
	XMVECTOR y1_firstInner_5 = (-5 * y0_firstInner_5 + 18 * XMLoadFloat4(&Boundary_firstInner_1_3_5) - 9 * XMLoadFloat4(&Boundary_firstInner_2_3_5) + 2 * y3_firstInner_5) / 6.0f;
	XMVECTOR y2_firstInner_5 = (2 * y0_firstInner_5 - 9 * XMLoadFloat4(&Boundary_firstInner_1_3_5) + 18 * XMLoadFloat4(&Boundary_firstInner_2_3_5) - 5 * y3_firstInner_5) / 6.0f;

	f0plus = 2 * y1_5 - y1_firstInner_5;// XMLoadFloat4(&FirstInner_1_3_5);
	m_gregoryPatches[0]->f0plus->SetPosition(f0plus);

	f1minus = 2 * y2_5 - y2_firstInner_5;// XMLoadFloat4(&FirstInner_2_3_5);
	m_gregoryPatches[0]->f1minus->SetPosition(f1minus);

	//return;
	////////UZUPE£NIENIA/////

	m_gregoryPatches[0]->f1plus->SetPosition(m_gregoryPatches[0]->f1minus->GetPosition());
	m_gregoryPatches[0]->f2minus->SetPosition(m_gregoryPatches[0]->e2minus->GetPosition());
	m_gregoryPatches[0]->f3minus->SetPosition(m_gregoryPatches[0]->f3plus->GetPosition());
	m_gregoryPatches[0]->f2plus->SetPosition(m_gregoryPatches[0]->e2plus->GetPosition());


	m_gregoryPatches[1]->f3plus->SetPosition(m_gregoryPatches[1]->f3minus->GetPosition());
	m_gregoryPatches[1]->f0minus->SetPosition(m_gregoryPatches[1]->e0minus->GetPosition());
	m_gregoryPatches[1]->f1minus->SetPosition(m_gregoryPatches[1]->f1plus->GetPosition());
	m_gregoryPatches[1]->f0plus->SetPosition(m_gregoryPatches[1]->e0plus->GetPosition());


	m_gregoryPatches[2]->f0minus->SetPosition(m_gregoryPatches[2]->f0plus->GetPosition());
	m_gregoryPatches[2]->f3plus->SetPosition(m_gregoryPatches[2]->e3plus->GetPosition());
	m_gregoryPatches[2]->f2plus->SetPosition(m_gregoryPatches[2]->f2minus->GetPosition());
	m_gregoryPatches[2]->f3minus->SetPosition(m_gregoryPatches[2]->e3minus->GetPosition());


	//////////WYRÓWNANIE C1/////////////////

	if (direction == TranslatedBezier::left)
	{
		//RUSZAMY ZEROWYM
		BezierSegment bottomLeft;
		y0_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e0minus->GetPosition());
		y1_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e0plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f0minus->GetPosition());
		y2_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e1minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f1plus->GetPosition());
		y3_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e1plus->GetPosition());

		bottomLeft.A = y0_firstInner_5;
		bottomLeft.B = y1_firstInner_5;
		bottomLeft.C = y2_firstInner_5;
		bottomLeft.D = y3_firstInner_5;

		XMVECTOR y0_changed = y0_firstInner_5;
		XMVECTOR y1_changed = bottomLeft.Q(2.0f / 3.0f);

		BezierSegment bottomRight;
		y0_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e0minus->GetPosition());
		y1_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e0plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f0minus->GetPosition());
		y2_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e1minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f1plus->GetPosition());
		y3_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e1plus->GetPosition());
		bottomRight.A = y0_firstInner_4;
		bottomRight.B = y1_firstInner_4;
		bottomRight.C = y2_firstInner_4;
		bottomRight.D = y3_firstInner_4;

		XMVECTOR y2_changed = bottomRight.Q(1.0f / 3.0f);
		XMVECTOR y3_changed = y3_firstInner_4;

		XMVECTOR P0_firstInner_bottom = y0_changed;
		XMVECTOR P3_firstInner_bottom = y3_changed;
		XMVECTOR P1_firstInner_bottom = (-5 * P0_firstInner_bottom + 18 * y1_changed - 9 * y2_changed + 2 * P3_firstInner_bottom) / 6.0f;
		XMVECTOR P2_firstInner_bottom = (2 * P0_firstInner_bottom - 9 * y1_changed + 18 * y2_changed - 5 * P3_firstInner_bottom) / 6.0f;

		/*XMVECTOR P0_firstInner_bottom = y0_firstInner_5;
		XMVECTOR P3_firstInner_bottom = y3_firstInner_4;
		XMVECTOR P1_firstInner_bottom = (-624.0f * P0_firstInner_bottom + 150.0f * y1_firstInner_5 - 72.0f * y2_firstInner_4 + 40.0f * P3_firstInner_bottom) / 120.0f;
		XMVECTOR P2_firstInner_bottom = (40.0f * P0_firstInner_bottom - 72.0f * y1_firstInner_5 + 150.0f * y2_firstInner_4 - 624.0f * P3_firstInner_bottom) / 120.0f;*/

		//m_bezierSurfaces[2]->GetNodes()[2 + 4 * 0]->SetPosition(y0_firstInner_5);
		//m_bezierSurfaces[2]->GetNodes()[2 + 4 * 1]->SetPosition(y1_firstInner_5);
		//m_bezierSurfaces[2]->GetNodes()[2 + 4 * 2]->SetPosition(y2_firstInner_4);
		//m_bezierSurfaces[2]->GetNodes()[2 + 4 * 3]->SetPosition(y3_firstInner_4);

		if (dir2 ==  dir_15_12)
		{
			m_bezierSurfaces[2]->GetNodes()[8]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[11]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_12_15)
		{
			m_bezierSurfaces[2]->GetNodes()[11]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[8]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_3_15)
		{
			m_bezierSurfaces[2]->GetNodes()[14]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[2]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_15_3)
		{
			m_bezierSurfaces[2]->GetNodes()[2]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[14]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_0_12)
		{
			m_bezierSurfaces[2]->GetNodes()[13]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[1]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_12_0)
		{
			m_bezierSurfaces[2]->GetNodes()[1]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[13]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_3_0 )
		{
			m_bezierSurfaces[2]->GetNodes()[4]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[7]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_0_3)
		{
			m_bezierSurfaces[2]->GetNodes()[7]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[4]->SetPosition(y3_firstInner_4);
		}


		m_bezierSurfaces[2]->Reset();

		m_gregoryPatches[0]->f0plus->SetPosition(m_gregoryPatches[0]->f0minus->GetPosition());
		m_gregoryPatches[2]->f1minus->SetPosition(m_gregoryPatches[2]->f1plus->GetPosition());



		////////LEFT///////////
		BezierSegment leftDown;
		y0_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e1minus->GetPosition());
		y1_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e1plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f1minus->GetPosition());
		y2_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e2minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f2plus->GetPosition());
		y3_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e2plus->GetPosition());

		BezierSegment leftUp;
		y0_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e1minus->GetPosition());
		y1_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e1plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f1minus->GetPosition());
		y2_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e2minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f2plus->GetPosition());
		y3_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e2plus->GetPosition());

		if (dir1 == dir_12_15)
		{
			m_bezierSurfaces[1]->GetNodes()[8]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[11]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_15_12)
		{
			m_bezierSurfaces[1]->GetNodes()[11]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[8]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_15_3)
		{
			m_bezierSurfaces[1]->GetNodes()[14]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[2]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_3_15)
		{
			m_bezierSurfaces[1]->GetNodes()[2]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[14]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_0_12)
		{
			m_bezierSurfaces[1]->GetNodes()[1]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[13]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_12_0)
		{
			m_bezierSurfaces[1]->GetNodes()[13]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[1]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_0_3)
		{
			m_bezierSurfaces[1]->GetNodes()[4]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[7]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_3_0)
		{
			m_bezierSurfaces[1]->GetNodes()[7]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[4]->SetPosition(y3_firstInner_2);
		}
		m_bezierSurfaces[1]->Reset();

		m_gregoryPatches[1]->f2minus->SetPosition(m_gregoryPatches[1]->f2plus->GetPosition());

	}
	else if (direction == TranslatedBezier::right)
	{

		//RUSZAMY PIERWSZYM
		BezierSegment bottomLeft;
		y0_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e0minus->GetPosition());
		y1_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e0plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f0minus->GetPosition());
		y2_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e1minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f1plus->GetPosition());
		y3_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e1plus->GetPosition());

		bottomLeft.A = y0_firstInner_5;
		bottomLeft.B = y1_firstInner_5;
		bottomLeft.C = y2_firstInner_5;
		bottomLeft.D = y3_firstInner_5;

		XMVECTOR y0_changed = y0_firstInner_5;
		XMVECTOR y1_changed = bottomLeft.Q(2.0f / 3.0f);

		BezierSegment bottomRight;
		y0_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e0minus->GetPosition());
		y1_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e0plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f0minus->GetPosition());
		y2_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e1minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f1plus->GetPosition());
		y3_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e1plus->GetPosition());
		bottomRight.A = y0_firstInner_4;
		bottomRight.B = y1_firstInner_4;
		bottomRight.C = y2_firstInner_4;
		bottomRight.D = y3_firstInner_4;

		XMVECTOR y2_changed = bottomRight.Q(1.0f / 3.0f);
		XMVECTOR y3_changed = y3_firstInner_4;

		XMVECTOR P0_firstInner_bottom = y0_changed;
		XMVECTOR P3_firstInner_bottom = y3_changed;
		XMVECTOR P1_firstInner_bottom = (-5 * P0_firstInner_bottom + 18 * y1_changed - 9 * y2_changed + 2 * P3_firstInner_bottom) / 6.0f;
		XMVECTOR P2_firstInner_bottom = (2 * P0_firstInner_bottom - 9 * y1_changed + 18 * y2_changed - 5 * P3_firstInner_bottom) / 6.0f;

		if (dir2 == dir_15_12)
		{
			m_bezierSurfaces[2]->GetNodes()[8]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[11]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_12_15)
		{
			m_bezierSurfaces[2]->GetNodes()[11]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[8]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_3_15)
		{
			m_bezierSurfaces[2]->GetNodes()[14]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[2]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_15_3)
		{
			m_bezierSurfaces[2]->GetNodes()[2]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[10]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[14]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_0_12)
		{
			m_bezierSurfaces[2]->GetNodes()[13]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[1]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_12_0)
		{
			m_bezierSurfaces[2]->GetNodes()[1]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[9]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[13]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_3_0)
		{
			m_bezierSurfaces[2]->GetNodes()[4]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[7]->SetPosition(y3_firstInner_4);
		}
		else if (dir2 == dir_0_3)
		{
			m_bezierSurfaces[2]->GetNodes()[7]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[6]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[2]->GetNodes()[5]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[2]->GetNodes()[4]->SetPosition(y3_firstInner_4);
		}

		m_bezierSurfaces[2]->Reset();

		m_gregoryPatches[0]->f0plus->SetPosition(m_gregoryPatches[0]->f0minus->GetPosition());
		m_gregoryPatches[2]->f1minus->SetPosition(m_gregoryPatches[2]->f1plus->GetPosition());



		////////LEFT///////////
		BezierSegment leftDown;
		y0_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e0plus->GetPosition());
		y1_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e0minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f0plus->GetPosition());
		y2_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e3plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f3minus->GetPosition());
		y3_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p3->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e3minus->GetPosition());

		BezierSegment leftUp;
		y0_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p3->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e3plus->GetPosition());
		y1_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e3minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f3plus->GetPosition());
		y2_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e2plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f2minus->GetPosition());
		y3_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e2minus->GetPosition());

		if (dir0 == dir_12_15)
		{
			m_bezierSurfaces[0]->GetNodes()[8]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[0]->GetNodes()[11]->SetPosition(y3_firstInner_2);
		}
		else if (dir0 == dir_15_12)
		{
			m_bezierSurfaces[0]->GetNodes()[11]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[0]->GetNodes()[8]->SetPosition(y3_firstInner_2);
		}
		else if (dir0 == dir_15_3)
			{
				m_bezierSurfaces[0]->GetNodes()[14]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[2]->SetPosition(y3_firstInner_2);
			}
			else if (dir0 == dir_3_15)
			{
				m_bezierSurfaces[0]->GetNodes()[2]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[14]->SetPosition(y3_firstInner_2);
			}
			else if (dir0 == dir_0_12)
			{
				m_bezierSurfaces[0]->GetNodes()[1]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[13]->SetPosition(y3_firstInner_2);
			}
			else if (dir0 == dir_12_0)
			{
				m_bezierSurfaces[0]->GetNodes()[13]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[1]->SetPosition(y3_firstInner_2);
			}
			else if (dir0 == dir_0_3)
			{
				m_bezierSurfaces[0]->GetNodes()[4]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[7]->SetPosition(y3_firstInner_2);
			}
			else if (dir0 == dir_3_0)
			{
				m_bezierSurfaces[0]->GetNodes()[7]->SetPosition(y0_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y1_firstInner_3);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y2_firstInner_2);
				m_bezierSurfaces[0]->GetNodes()[4]->SetPosition(y3_firstInner_2);
			}
		m_bezierSurfaces[0]->Reset();

		m_gregoryPatches[1]->f2plus->SetPosition(m_gregoryPatches[1]->f2minus->GetPosition());

	}
	else if (direction == TranslatedBezier::bottom)
	{
		//RUSZAMY DRUGIM
		y0_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p0->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e0plus->GetPosition());
		y1_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e0minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f0plus->GetPosition());
		y2_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->e3plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->f3minus->GetPosition());
		y3_firstInner_5 = 2 * XMLoadFloat4(&m_gregoryPatches[0]->p3->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[0]->e3minus->GetPosition());

		y0_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p3->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e3plus->GetPosition());
		y1_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e3minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f3plus->GetPosition());
		y2_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e2plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f2minus->GetPosition());
		y3_firstInner_4 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e2minus->GetPosition());

		if (dir0 == dir_12_15)
		{
			m_bezierSurfaces[0]->GetNodes()[8]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[0]->GetNodes()[11]->SetPosition(y3_firstInner_4);
		}
		else if (dir0 == dir_15_12)
		{
			m_bezierSurfaces[0]->GetNodes()[11]->SetPosition(y0_firstInner_5);
			m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y1_firstInner_5);
			m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y2_firstInner_4);
			m_bezierSurfaces[0]->GetNodes()[8]->SetPosition(y3_firstInner_4);
		}
		else if (dir0 == dir_15_3)
			{
				m_bezierSurfaces[0]->GetNodes()[14]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[2]->SetPosition(y3_firstInner_4);
			}
			else if (dir0 == dir_3_15)
			{
				m_bezierSurfaces[0]->GetNodes()[2]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[10]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[14]->SetPosition(y3_firstInner_4);
			}
			else if (dir0 == dir_0_12)
			{
				m_bezierSurfaces[0]->GetNodes()[1]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[13]->SetPosition(y3_firstInner_4);
			}
			else if (dir0 == dir_12_0)
			{
				m_bezierSurfaces[0]->GetNodes()[13]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[9]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[1]->SetPosition(y3_firstInner_4);
			}
			else if (dir0 == dir_0_3)
			{
				m_bezierSurfaces[0]->GetNodes()[4]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[7]->SetPosition(y3_firstInner_4);
			}
			else if (dir0 == dir_3_0)
			{
				m_bezierSurfaces[0]->GetNodes()[7]->SetPosition(y0_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[6]->SetPosition(y1_firstInner_5);
				m_bezierSurfaces[0]->GetNodes()[5]->SetPosition(y2_firstInner_4);
				m_bezierSurfaces[0]->GetNodes()[4]->SetPosition(y3_firstInner_4);
			}
		m_bezierSurfaces[0]->Reset();

		m_gregoryPatches[0]->f0minus->SetPosition(m_gregoryPatches[0]->f0plus->GetPosition());

		////////LEFT///////////
		BezierSegment leftDown;
		y0_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e1minus->GetPosition());
		y1_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e1plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f1minus->GetPosition());
		y2_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->e2minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->f2plus->GetPosition());
		y3_firstInner_3 = 2 * XMLoadFloat4(&m_gregoryPatches[2]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[2]->e2plus->GetPosition());

		BezierSegment leftUp;
		y0_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p1->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e1minus->GetPosition());
		y1_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e1plus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f1minus->GetPosition());
		y2_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->e2minus->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->f2plus->GetPosition());
		y3_firstInner_2 = 2 * XMLoadFloat4(&m_gregoryPatches[1]->p2->GetPosition()) - XMLoadFloat4(&m_gregoryPatches[1]->e2plus->GetPosition());

	if (dir1 == dir_12_15)
		{
			m_bezierSurfaces[1]->GetNodes()[8]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[11]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_15_12)
		{
			m_bezierSurfaces[1]->GetNodes()[11]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[8]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_15_3)
		{
			m_bezierSurfaces[1]->GetNodes()[14]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[2]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_3_15)
		{
			m_bezierSurfaces[1]->GetNodes()[2]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[10]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[14]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_0_12)
		{
			m_bezierSurfaces[1]->GetNodes()[1]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[13]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_12_0)
		{
			m_bezierSurfaces[1]->GetNodes()[13]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[9]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[1]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_0_3)
		{
			m_bezierSurfaces[1]->GetNodes()[4]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[7]->SetPosition(y3_firstInner_2);
		}
		else if (dir1 == dir_3_0)
		{
			m_bezierSurfaces[1]->GetNodes()[7]->SetPosition(y0_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[6]->SetPosition(y1_firstInner_3);
			m_bezierSurfaces[1]->GetNodes()[5]->SetPosition(y2_firstInner_2);
			m_bezierSurfaces[1]->GetNodes()[4]->SetPosition(y3_firstInner_2);
		}
		m_bezierSurfaces[1]->Reset();

		m_gregoryPatches[2]->f1plus->SetPosition(m_gregoryPatches[2]->f1minus->GetPosition());
		m_gregoryPatches[1]->f2minus->SetPosition(m_gregoryPatches[1]->f2plus->GetPosition());

	}
	for (int i = 0; i < m_gregoryPatches.size(); i++)
	{
		m_gregoryPatches[i]->Reset();
	}
}


void GregorySurface::SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth)
{
	m_rows = rows;
	m_cols = cols;
	m_sizeX = surfaceWidth;
	m_sizeY = surfaceHeigth;
}

void GregorySurface::SetBezierSurfaces(vector<BezierSurface*> bezierSurfaces)
{
	m_bezierSurfaces = bezierSurfaces;
}

vector<SimplePoint*> GregorySurface::GetNodes()
{
	vector<SimplePoint*> nodes;
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
		nodes.push_back(it->second);
	return nodes;
}

void GregorySurface::Initialize()
{

}

void GregorySurface::Draw()
{
	//TranslatePoints(TranslatedBezier::bottom);
	//todo
}
void GregorySurface::setTriangleTopology()
{

}
void GregorySurface::setLineTopology()
{

}