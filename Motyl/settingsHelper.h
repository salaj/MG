#pragma once

#include<string>
#include"ControllerMain.h"

using namespace Win;

namespace gk2
{
	class SettingsHelper
	{
	public:
		SettingsHelper();

		static const std::wstring SimplePointName;
		static const std::wstring TorusName;
		static const std::wstring BezierEdgeName;
		static const std::wstring BezierC2EdgeName;
		static const std::wstring BezierC2Interpolated;
		static const std::wstring BezierPatch;
		static const std::wstring BezierSurface;
		static const std::wstring BSplinePatch;
		static const std::wstring GregoryPatch;
		static const std::wstring BSplineSurface;
		static const std::wstring GregorySurface;


		static const std::wstring BernsteinBase;
		static const std::wstring BSplineBase;

		static const std::wstring PlaneSurface;
		static const std::wstring CyllinderSurface;

		static int SimplePointCounter;
		static int TorusCounter;
		static int BezierCurveCounter;
		static int BezierC2CurveCounter;
		static int C2InterpolatedCounter;
		static int BezierSurfaceCounter;
		static int BezierPatchCounter;
		static int BSplineSurfaceCounter;
		static int BSplinePatchCounter;
		static int GregoryPatchCounter;
		static int GregorySurfaceCounter;


		InsertionParams* AddNewModelToTreeView(wchar_t*, HWND handle = 0);
		InsertionParams* insertItemInternally(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		InsertionParams* insertItemFreely(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		//HTREEITEM insertItemExternally(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		void removeItem(int id);

		Win::ControllerMain m_controller;
		void CopyItem(HTREEITEM source, HTREEITEM target);
		bool isSurfacePlane = true;
	private:

	};
}

