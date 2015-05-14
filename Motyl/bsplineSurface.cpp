#include "bsplineSurface.h"


BSplineSurface::BSplineSurface()
{
	m_Type = ModelType::BSplineSurfaceType;
	ModelClass::Initialize();
}


BSplineSurface::~BSplineSurface()
{
}

BSplineSurface::BSplineSurface(const BSplineSurface&)
{
}

void* BSplineSurface::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BSplineSurface::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BSplineSurface::UpdateNode(SimplePoint* point)
{
	for (int i = 0; i < m_bsplinePatches.size(); i++)
	{
		vector<SimplePoint*> patchNodes = m_bsplinePatches[i]->GetNodes();
		for (int j = 0; j < patchNodes.size(); j++)
		{
			if (patchNodes[j]->m_id == point->m_id)
			{
				m_bsplinePatches[i]->Reset();
				break;
			}
		}
	}
}

void BSplineSurface::AddPatch(BSplinePatch* patch)
{
	m_bsplinePatches.push_back(patch);
	vector<SimplePoint*> patchNodes = patch->GetNodes();
	for (int i = 0; i < patchNodes.size(); i++)
	{
		m_nodes.insert(pair<int, SimplePoint*>(patchNodes[i]->m_id, patchNodes[i]));
	}
}

void BSplineSurface::TranslateSurfacePoints()
{
	int i = 0;
	int j = 0;
	double X = (double)m_sizeX / (m_cols + 2);
	double Y = (double)m_sizeY / (m_rows + 2);
	for (map<int, SimplePoint*> ::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
	{
		(*it).second->Translate(XMFLOAT4(j * X - (m_sizeX / 2.0f ), i * Y - (m_sizeY / 2.0f), 0, 1));
		if (j < m_cols + 2)
			j++;
		else
		{
			j = 0;
			i++;
		}
	}
	for (int i = 0; i < m_bsplinePatches.size(); i++)
	{
		m_bsplinePatches[i]->Reset();
	}
}

void BSplineSurface::TranslateCyllinderPoints()
{
	double r = m_sizeX;
	double a = (double)1 / (m_cols);
	double Y = (double)m_sizeY / (m_rows + 2);
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
		if (j < m_cols - 1)
			j++;
		else
		{
			j = 0;
			i++;
		}
	}
	for (int i = 0; i < m_bsplinePatches.size(); i++)
	{
		m_bsplinePatches[i]->Reset();
	}

}


void BSplineSurface::SetDimensions(int rows, int cols, int surfaceWidth, int surfaceHeigth)
{
	m_rows = rows;
	m_cols = cols;
	m_sizeX = surfaceWidth;
	m_sizeY = surfaceHeigth;
}

void BSplineSurface::Initialize()
{
	
}

void BSplineSurface::Draw()
{
	//todo
}
void BSplineSurface::setTriangleTopology()
{

}
void BSplineSurface::setLineTopology()
{

}