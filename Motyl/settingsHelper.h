#pragma once

#include<string>
#include"ControllerMain.h"


namespace gk2
{
	class SettingsHelper
	{
	public:
		SettingsHelper();

		static const std::wstring SimplePointName;
		static const std::wstring TorusName;
		static const std::wstring BezierEdgeName;

		static int SimplePointCounter;
		static int TorusCounter;
		static int BezierCurveCounter;


		void AddNewModelToTreeView(wchar_t*);
		HTREEITEM insertItemInternally(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		//HTREEITEM insertItemExternally(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		void removeItem(int id);

		Win::ControllerMain m_controller;
	private:

	};
}

