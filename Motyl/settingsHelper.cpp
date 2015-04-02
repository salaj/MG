#include "settingsHelper.h"
using namespace std;
using namespace gk2;


const std::wstring SettingsHelper::SimplePointName = L"Simple point ";
const std::wstring SettingsHelper::TorusName = L"Torus ";
const std::wstring SettingsHelper::BezierEdgeName = L"Bezier curve ";

int SettingsHelper::SimplePointCounter = 0;
int SettingsHelper::TorusCounter = 0;
int SettingsHelper::BezierCurveCounter = 0;

SettingsHelper::SettingsHelper()
{
}


void SettingsHelper::AddNewModelToTreeView(wchar_t* buf)
{
	wstring modelToAddName(buf);
	wchar_t digitBuf[16];
	//std::wstring dB(digitBuf);
	//string modelToAddName(ws.begin(), ws.end());
	if (modelToAddName.compare(SimplePointName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", SimplePointCounter++);
		insertItemInternally((SimplePointName + wstring(digitBuf)).c_str(), ItemType::ItemPoint);
	}
	else if (modelToAddName.compare(TorusName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", TorusCounter++);
		insertItemInternally((TorusName + wstring(digitBuf)).c_str(), ItemType::ItemTorus);
	}
	else if (modelToAddName.compare(BezierEdgeName) == 0)
	{
		swprintf(digitBuf, sizeof(digitBuf) / sizeof(*digitBuf), L"%d", BezierCurveCounter++);
		insertItemInternally((BezierEdgeName + wstring(digitBuf)).c_str(), ItemType::ItemCurve);
	}

}

HTREEITEM SettingsHelper::insertItemInternally(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
{
	//m_input->AddModel(SimplePointType);
	return m_controller.insertItem(str, type, parent, TVI_LAST, 0, 1);
}
//
//HTREEITEM SettingsHelper::insertItemExternally(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
//{
//	return m_controller.insertItem(str, type, parent, TVI_LAST, 0, 1);
//}

void SettingsHelper::removeItem(int id)
{
	m_controller.removeItem(id);
}