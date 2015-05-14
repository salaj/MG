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
	int i = 0;
	int j = 0;
	double X = (double)m_sizeX / (m_cols * 3);
	double Y = (double)m_sizeY / (m_rows * 3);
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
	{
		(*it).second->Translate(XMFLOAT4(j * X - (m_sizeX / 2.0f ), i * Y - (m_sizeY / 2.0f), 0, 1));
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

void BezierSurface::TranslateCyllinderPoints()
{
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


void BezierSurface::SetDimensions(int rows, int cols, int surfaceWidth, int surfaceHeigth)
{
	m_rows = rows;
	m_cols = cols;
	m_sizeX = surfaceWidth;
	m_sizeY = surfaceHeigth;
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