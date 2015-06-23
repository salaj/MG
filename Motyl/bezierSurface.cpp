#include "bezierSurface.h"


BezierSurface::BezierSurface()
{
	m_Type = ModelType::BezierSurfaceType;
	ModelClass::Initialize();
}


BezierSurface::~BezierSurface()
{
}

BezierSurface::BezierSurface(const BezierSurface&)
{
}

void* BezierSurface::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierSurface::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BezierSurface::Reset()
{
	for (int i = 0; i < m_bezierPatches.size(); i++)
	{
		m_bezierPatches[i]->Reset();
	}
}

void BezierSurface::UpdateNode(SimplePoint* point)
{
	for (int i = 0; i < m_bezierPatches.size(); i++)
	{
		vector<SimplePoint*> patchNodes = m_bezierPatches[i]->GetNodes();
		for (int j = 0; j < patchNodes.size(); j++)
		{
			if (patchNodes[j]->m_id == point->m_id)
			{
				m_bezierPatches[i]->Reset();
				break;
			}
		}
	}
}

void BezierSurface::ReplaceNode(SimplePoint* point, SimplePoint* replacement)
{
	for (int i = 0; i < m_bezierPatches.size(); i++)
	{
		vector<SimplePoint*> patchNodes = m_bezierPatches[i]->GetNodes();
		for (int j = 0; j < patchNodes.size(); j++)
		{
			if (patchNodes[j]->m_id == point->m_id)
			{
				m_bezierPatches[i]->ReplaceNode(j, replacement);
				break;
			}
		}
	}
}

void BezierSurface::AddPatch(BezierPatch* patch)
{
	m_bezierPatches.push_back(patch);
	vector<SimplePoint*> patchNodes = patch->GetNodes();
	for (int i = 0; i < patchNodes.size(); i++)
	{
		m_nodes.insert(pair<int, SimplePoint*>(patchNodes[i]->m_id, patchNodes[i]));
	}
}

void BezierSurface::TranslateSurfacePoints()
{
	isCyllindrical = false;
	int i = 0;
	int j = 0;
	double X = (double)m_sizeX / (m_cols * 3);
	double Y = (double)m_sizeY / (m_rows * 3);
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
	{
		(*it).second->Translate(XMFLOAT4(i * X - (m_sizeX / 2.0f ), j * Y - (m_sizeY / 2.0f), 0, 1));
		if (j < m_cols * 3)
			j++;
		else
		{
			j = 0;
			i++;
		}
	}
	for (int i = 0; i < m_bezierPatches.size(); i++)
	{
		m_bezierPatches[i]->Reset();
	}
}

XMFLOAT4 BezierSurface::Q(float u, float v)
{
	//FOR GREGORY TO ALLWAYS WORK
	if (m_bezierPatches.size() == 1)
		return m_bezierPatches[0]->Q(u, v);

	int n, m;
	//m = /*m_cols;*/m_bezierPatches.size();

	//float step_v = 1.0f / m;
	//int index_v = v / step_v;
	//if (v == 1.0f)
	//	index_v--;
	////if (index_v * step_v == v) //czyli podzieli³o siê bez reszty
	////	index_v--;
	//float valueScalled_v = (v - index_v * step_v) / step_v;
	//return m_bezierPatches[index_v]->Q(u, valueScalled_v);


	m = m_cols;

	float step_v = 1.0f / m;
	int index_v = v / step_v;
	if (index_v != 0 && index_v * step_v == v) //czyli podzieli³o siê bez reszty
		index_v--;
	float valueScalled_v = (v - index_v * step_v) / step_v;


	n = m_rows;
	float step_u = 1.0f / n;
	int index_u = u / step_u;
	if (index_u != 0 && index_u * step_u == u)
		index_u--;
	float valueScalled_u = (u - index_u * step_u) / step_u;

	int index = index_u * m_rows + index_v;


	return m_bezierPatches[index]->Q(valueScalled_u, valueScalled_v);


}

void BezierSurface::TranslateCyllinderPoints()
{
	isCyllindrical = true;
	double r = m_sizeX;
	double a = (double)1 / (m_cols * 3);
	double Y = (double)m_sizeY / (m_rows * 3);
	int i = 0;
	int j = 0;
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
	{
		double x = r * cosf((double)j * a * XM_2PI);
		(*it).second->Translate(
			XMFLOAT4(
			x,
			i * Y - m_sizeY / 2.0f,
			r * sinf((double)j * a * XM_2PI),
			1));
		if (j < m_cols * 3 - 1)
			j++;
		else
		{
			j = 0;
			i++;
		}
	}
	for (int i = 0; i < m_bezierPatches.size(); i++)
	{
		m_bezierPatches[i]->Reset();
	}

}


void BezierSurface::SetDimensions(int rows, int cols, double surfaceWidth, double surfaceHeigth)
{
	m_rows = rows;
	m_cols = cols;
	m_sizeX = surfaceWidth;
	m_sizeY = surfaceHeigth;
}

vector<SimplePoint*> BezierSurface::GetNodes()
{
	vector<SimplePoint*> nodes;
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
		nodes.push_back(it->second);
	return nodes;
}

void BezierSurface::Initialize()
{
	
}

void BezierSurface::Draw()
{
	//todo
}
void BezierSurface::setTriangleTopology()
{

}
void BezierSurface::setLineTopology()
{

}