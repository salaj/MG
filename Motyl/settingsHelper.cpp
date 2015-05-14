#include "settingsHelper.h"
using namespace std;
using namespace gk2;


const std::wstring SettingsHelper::SimplePointName = L"Simple point ";
const std::wstring SettingsHelper::TorusName = L"Torus ";
const std::wstring SettingsHelper::BezierEdgeName = L"C0 curve ";
const std::wstring SettingsHelper::BezierC2EdgeName = L"C2 curve ";
const std::wstring SettingsHelper::BezierC2Interpolated = L"C2 interpolated ";
const std::wstring SettingsHelper::BezierPatch = L"Bezier patch ";
const std::wstring SettingsHelper::BezierSurface = L"Bezier surface ";
const std::wstring SettingsHelper::BSplinePatch = L"BSpline patch ";
const std::wstring SettingsHelper::BSplineSurface = L"BSpline surface ";

const std::wstring SettingsHelper::BernsteinBase = L"Bernstein base ";
const std::wstring SettingsHelper::BSplineBase = L"BSpline base ";

const std::wstring SettingsHelper::PlaneSurface = L"Plane surface ";
const std::wstring SettingsHelper::CyllinderSurface = L"Cyllinder surface ";

int SettingsHelper::SimplePointCounter = 0;
int SettingsHelper::TorusCounter = 0;
int SettingsHelper::BezierCurveCounter = 0;
int SettingsHelper::BezierC2CurveCounter = 0;
int SettingsHelper::C2InterpolatedCounter = 0;
int SettingsHelper::BezierSurfaceCounter = 0;
int SettingsHelper::BezierPatchCounter = 0;
int SettingsHelper::BSplineSurfaceCounter = 0;
int SettingsHelper::BSplinePatchCounter = 0;

SettingsHelper::SettingsHelper()
{
}


HTREEITEM SettingsHelper::AddNewModelToTreeView(wchar_t* buf, HWND handle)
{
	wstring modelToAddName(buf);
	wchar_t digitBuf[16];
	HTREEITEM item;

	if (modelToAddName.compare(SimplePointName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", SimplePointCounter++);
		item = insertItemFreely((SimplePointName + wstring(digitBuf)).c_str(), ItemType::ItemPoint, nullptr);
	}
	else if (modelToAddName.compare(TorusName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", TorusCounter++);
		item = insertItemFreely((TorusName + wstring(digitBuf)).c_str(), ItemType::ItemTorus, nullptr);
	}
	else if (modelToAddName.compare(BezierEdgeName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierCurveCounter++);
		item = insertItemFreely((BezierEdgeName + wstring(digitBuf)).c_str(), ItemType::ItemCurve, nullptr);
	}
	else if (modelToAddName.compare(BezierC2EdgeName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierCurveCounter++);
		item = insertItemFreely((BezierC2EdgeName + wstring(digitBuf)).c_str(), ItemType::ItemC2Curve, nullptr);
	}
	else if (modelToAddName.compare(BezierC2Interpolated) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierCurveCounter++);
		item = insertItemFreely((BezierC2Interpolated + wstring(digitBuf)).c_str(), ItemType::ItemC2Interpolated, nullptr);
	}
	else if (modelToAddName.compare(BezierSurface) == 0)
	{
		if (isSurfacePlane)
		{
			swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierSurfaceCounter++);
			item = insertItemInternally((BezierSurface + wstring(digitBuf)).c_str(), ItemType::ItemBezierSurface);

			int numberOfPatches = m_controller.view.m_cols * m_controller.view.m_rows;
			wchar_t*** items = new wchar_t**[m_controller.view.m_rows * 3 + 1];

			for (int i = 0; i < m_controller.view.m_rows * 3 + 1; i++)
			{
				items[i] = new wchar_t*[m_controller.view.m_cols * 3 + 1];
				for (int j = 0; j < m_controller.view.m_cols * 3 + 1; j++)
				{
					items[i][j] = new wchar_t[16];
					swprintf(items[i][j], sizeof(wchar_t) * 16 / sizeof(*items[i][j]), L"%d", SimplePointCounter++);
					insertItemInternally((SimplePointName + wstring(items[i][j])).c_str(), ItemType::ItemPoint);
				}
			}
			for (int i = 0; i < m_controller.view.m_rows; i++)
			{
				for (int j = 0; j < m_controller.view.m_cols; j++)
				{
					wchar_t digitBuf[16];
					swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierPatchCounter++);
					HTREEITEM patch = insertItemInternally((BezierPatch + wstring(digitBuf)).c_str(), ItemType::ItemPatch, item);
					int row = i * 3;
					for (int n = 0; n < 4; n++)
					{
						int col = j * 3;
						for (int m = 0; m < 4; m++)
						{
							insertItemInternally((SimplePointName + wstring(items[row][col])).c_str(), ItemType::ItemPoint, patch);
							col++;
						}
						row++;
					}
				}
			}
		}
		else
		{
			//cyllinder
			swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierSurfaceCounter++);
			item = insertItemInternally((BezierSurface + wstring(digitBuf)).c_str(), ItemType::ItemBezierSurface);

			int numberOfPatches = m_controller.view.m_cols * m_controller.view.m_rows;
			wchar_t*** items = new wchar_t**[m_controller.view.m_rows * 3 + 1];

			for (int i = 0; i < m_controller.view.m_rows * 3 + 1; i++)
			{
				items[i] = new wchar_t*[m_controller.view.m_cols * 3];
				for (int j = 0; j < m_controller.view.m_cols * 3; j++)
				{
					items[i][j] = new wchar_t[16];
					swprintf(items[i][j], sizeof(wchar_t) * 16 / sizeof(*items[i][j]), L"%d", SimplePointCounter++);
					insertItemInternally((SimplePointName + wstring(items[i][j])).c_str(), ItemType::ItemPoint);
				}
			}
			for (int i = 0; i < m_controller.view.m_rows; i++)
			{
				for (int j = 0; j < m_controller.view.m_cols; j++)
				{
					wchar_t digitBuf[16];
					swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierPatchCounter++);
					HTREEITEM patch = insertItemInternally((BezierPatch + wstring(digitBuf)).c_str(), ItemType::ItemPatch, item);
					int row = i * 3;
					for (int n = 0; n < 4; n++)
					{
						int col = j * 3;
						for (int m = 0; m < 4; m++)
						{
							if (col == m_controller.view.m_cols * 3)
								insertItemInternally((SimplePointName + wstring(items[row][0])).c_str(), ItemType::ItemPoint, patch);
							else
								insertItemInternally((SimplePointName + wstring(items[row][col])).c_str(), ItemType::ItemPoint, patch);
							col++;
						}
						row++;
					}
				}
			}
		}
		m_controller.ReconstructSurface(item);
	}
	else if (modelToAddName.compare(BSplineSurface) == 0)
	{
		if (isSurfacePlane)
		{
			swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BSplineSurfaceCounter++);
			item = insertItemInternally((BSplineSurface + wstring(digitBuf)).c_str(), ItemType::ItemBSplineSurface);

			int numberOfPatches = m_controller.view.m_cols * m_controller.view.m_rows;
			wchar_t*** items = new wchar_t**[m_controller.view.m_rows + 3];

			for (int i = 0; i < m_controller.view.m_rows + 3; i++)
			{
				items[i] = new wchar_t*[m_controller.view.m_cols + 3];
				for (int j = 0; j < m_controller.view.m_cols + 3; j++)
				{
					items[i][j] = new wchar_t[16];
					swprintf(items[i][j], sizeof(wchar_t) * 16 / sizeof(*items[i][j]), L"%d", SimplePointCounter++);
					insertItemInternally((SimplePointName + wstring(items[i][j])).c_str(), ItemType::ItemPoint);
				}
			}
			for (int i = 0; i < m_controller.view.m_rows; i++)
			{
				for (int j = 0; j < m_controller.view.m_cols; j++)
				{
					wchar_t digitBuf[16];
					swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BSplinePatchCounter++);
					HTREEITEM patch = insertItemInternally((BSplinePatch + wstring(digitBuf)).c_str(), ItemType::ItemBSplinePatch, item);
					int row = i;
					for (int n = 0; n < 4; n++)
					{
						int col = j;
						for (int m = 0; m < 4; m++)
						{
							insertItemInternally((SimplePointName + wstring(items[row][col])).c_str(), ItemType::ItemPoint, patch);
							col++;
						}
						row++;
					}
				}
			}
		}
		else
		{
			//cyllinder
			swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierSurfaceCounter++);
			item = insertItemInternally((BSplineSurface + wstring(digitBuf)).c_str(), ItemType::ItemBSplineSurface);

			int numberOfPatches = m_controller.view.m_cols * m_controller.view.m_rows;
			wchar_t*** items = new wchar_t**[m_controller.view.m_rows + 3];

			for (int i = 0; i < m_controller.view.m_rows + 3; i++)
			{
				items[i] = new wchar_t*[m_controller.view.m_cols];
				for (int j = 0; j < m_controller.view.m_cols; j++)
				{
					items[i][j] = new wchar_t[16];
					swprintf(items[i][j], sizeof(wchar_t) * 16 / sizeof(*items[i][j]), L"%d", SimplePointCounter++);
					insertItemInternally((SimplePointName + wstring(items[i][j])).c_str(), ItemType::ItemPoint);
				}
			}
			for (int i = 0; i < m_controller.view.m_rows; i++)
			{
				for (int j = 0; j < m_controller.view.m_cols; j++)
				{
					wchar_t digitBuf[16];
					swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierPatchCounter++);
					HTREEITEM patch = insertItemInternally((BSplinePatch + wstring(digitBuf)).c_str(), ItemType::ItemBSplinePatch, item);
					int row = i;
					for (int n = 0; n < 4; n++)
					{
						int col = j;
						for (int m = 0; m < 4; m++)
						{
							if (col >= m_controller.view.m_cols)
								insertItemInternally((SimplePointName + wstring(items[row][col % m_controller.view.m_cols])).c_str(), ItemType::ItemPoint, patch);
							else
								insertItemInternally((SimplePointName + wstring(items[row][col])).c_str(), ItemType::ItemPoint, patch);
							col++;
						}
						row++;
					}
				}
			}
		}
		m_controller.ReconstructSurface(item);
	}
	
	return item;
}

HTREEITEM SettingsHelper::insertItemInternally(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
{
	return m_controller.insertItemHierarchically(str, type, parent, TVI_LAST, 0, 1);
}

HTREEITEM SettingsHelper::insertItemFreely(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
{
	return m_controller.insertItemFreely(str, type, parent, TVI_LAST, 0, 1);
}


void SettingsHelper::removeItem(int id)
{
	m_controller.removeItem(id);
}