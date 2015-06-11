#pragma once

#include <windows.h>
#include <string>


enum ItemType{
	ItemPoint, 
	ItemCurve,
	ItemC2Curve,
	ItemC2Interpolated,
	ItemTorus,
	ItemRoot,
	ItemPatch,
	ItemBSplinePatch,
	ItemBezierSurface,
	ItemBSplineSurface,
	ItemGregorySurface,
	ItemGregoryPatch
};

enum Origin{
	Genuine,
	Copy
};

struct TreeItem
{
    std::wstring text;
    HTREEITEM handle;
    HTREEITEM parentHandle;
    HTREEITEM previousHandle;
    int imageIndex;
    int selectedImageIndex;
	int id;
	ItemType type;
	Origin origin;

    // init all members
    TreeItem::TreeItem() : handle(0), parentHandle(0), previousHandle(0), imageIndex(0), selectedImageIndex(0), id(-1) {}
};
