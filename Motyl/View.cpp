///////////////////////////////////////////////////////////////////////////////
// View.cpp
// ========
// View component
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-06-07
// UPDATED: 2007-10-02
///////////////////////////////////////////////////////////////////////////////


#include "View.h"
#include "resource.h"
#include <CommCtrl.h>
#include <sstream>
using std::wstringstream;
using std::ends;
using namespace Win;

const int MAX_STRING = 256;

using namespace std;
///////////////////////////////////////////////////////////////////////////////
// ctor/dtor
///////////////////////////////////////////////////////////////////////////////
View::View() :tvItemDragged(false), tvDraggedItem(0), tvDraggedImageList(0), tvMenu(0)
{
	int a = 10;
	//for (int i = 0; i < 100; i++)
	//{
	//	//treeItems[i] = NULL;
	//	allItems[i] = TreeItem();
	//	allItems[i].handle = NULL;
	//}
}

View::~View()
{
    ::ImageList_Destroy(imageList);
}

void View::SetEngineNotifier(EngineNotifier* engineNotifier)
{
	m_engineNotifier = engineNotifier;
}


///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void View::init(HWND handle)
{
	treeView.set(handle, IDC_TREE1);
    buttonExit.set(handle, IDC_EXIT);

    // create ImageList
    const int IMAGE_WIDTH = 16;
    const int IMAGE_HEIGHT = 16;
    const int IMAGE_COUNT = 3;
    const int GROW_SIZE = 4;                    // the number of images when ImageList needs to grow for new images

    // create ImageList control first
    imageList = ::ImageList_Create(IMAGE_WIDTH, IMAGE_HEIGHT, ILC_COLORDDB | ILC_MASK, IMAGE_COUNT, GROW_SIZE);

    // load icons and add them to ImageList
    // PARAMS: hInstance, filename, type, width, height, flag
    HICON icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDERCLOSED), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);
    icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDEROPEN), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);
    icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPDEFAULT), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);

    // make sure all icons are in the ImageList
    if(::ImageList_GetImageCount(imageList) == IMAGE_COUNT)
    {
        treeView.setImageList(imageList);
    }

    // add root item
    HTREEITEM parentItem = *treeView.insertItem(L"Parent", TVI_ROOT, TVI_LAST, 2, 2);
	TreeItem newTreeItem = TreeItem();
	newTreeItem.handle = parentItem;
	newTreeItem.id = counter;
	newTreeItem.type = ItemType::ItemRoot;
	newTreeItem.origin = Origin::Genuine;
	allItems[counter].push_back(newTreeItem);
	counter++;
}

int View::findIdOfGenuine(const wchar_t* str)
{
	for (int i = 0; i < 3000; i++)
	{
		for (int j = 0; j < allItems[i].size(); j++)
			if (allItems[i][j].text.compare(str) == 0)
				return allItems[i][j].id;
	}
}


int View::counter = 0;

InsertionParams* View::insertItem(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
{

	bool copyPoint = false;
	int id_Genuine;
	if (type == ItemType::ItemPoint && 
		(allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemPatch ||
		 allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemBSplinePatch||
		 allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemGregoryPatch))
	{
		copyPoint = true;
		id_Genuine = findIdOfGenuine(str);
	}
	//HTREEITEM selected = treeView.getSelected();
	HTREEITEM item = *(treeView.insertItem(str, parent, TVI_LAST, 0, 1));

	TreeItem newTreeItem = TreeItem();
	newTreeItem.handle = item;
	if (!copyPoint)
		newTreeItem.id = counter;
	else
		newTreeItem.id = id_Genuine;
	newTreeItem.type = type;
	newTreeItem.text = str;
	//now we want to check if we created point for curve or patch
	if (type == ItemType::ItemPoint && (
		allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemCurve ||
		allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemC2Curve ||
		allItems[FindIdByViewItem(parent)][0].type == ItemType::ItemC2Interpolated
		))
	{
		//it means newTreeItem is just a copy
		newTreeItem.origin = Origin::Copy;
		//then we have to create also genuine point independently
		addOriginPointItem(str);
	}
	else if (copyPoint)
	{
		//it means newTreeItem is just a copy
		newTreeItem.origin = Origin::Copy;
	}
	else
	{
		newTreeItem.origin = Origin::Genuine;
	}

	if (!copyPoint)
	{
		allItems[counter].push_back(newTreeItem);
		counter++;
	}
	else
		allItems[id_Genuine].push_back(newTreeItem);

	ModelClass* model = nullptr;
	switch (type)
	{
	case ItemPoint:
		if (!copyPoint)
			model = m_engineNotifier->OnSimplePointAdded();
		onItemChanged(item);
		break;
	case ItemCurve:
		model = m_engineNotifier->OnBezierCurveAdded();
		break;
	case ItemC2Curve:
		model = m_engineNotifier->OnBezierC2CurveAdded();
		break;
	case ItemC2Interpolated:
		model = m_engineNotifier->OnC2InterpolatedAdded();
		break;
	case ItemTorus:
		model = m_engineNotifier->OnTorusAdded();
		break;
	case ItemPatch:
		model = m_engineNotifier->OnBezierPatchAdded();
		break;
	case ItemGregoryPatch:
		model = m_engineNotifier->OnGregoryPatchAdded();
		break;
	case ItemBezierSurface:
		model = m_engineNotifier->OnBezierSurfaceAdded();
		break;
	case ItemBSplinePatch:
		model = m_engineNotifier->OnBSplinePatchAdded();
		break;
	case ItemBSplineSurface:
		model = m_engineNotifier->OnBSplineSurfaceAdded();
		break;
	case ItemGregorySurface:
		model = m_engineNotifier->OnGregorySurfaceAdded();
		break;
	case ItemIntersectionCurve:
		model = m_engineNotifier->OnIntersectionCurveAdded();
		break;
	default:
		break;
	}
	return new InsertionParams(model, item);
	//return item;
}

void View::addOriginPointItem(const wchar_t* str)
{
	//we append genuine point to root
	HTREEITEM item = *(treeView.insertItem(str, TVI_ROOT, TVI_LAST, 0, 1));
	TreeItem newTreeItem = TreeItem();
	newTreeItem.handle = item;
	newTreeItem.id = counter;
	newTreeItem.origin = Origin::Genuine;
	//we are ALWAYS creating ITEMPOINT !!!!
	newTreeItem.type = ItemType::ItemPoint;
	//it may happen there is already genuine point on the list
	for (int i = 0; i < allItems[counter].size(); i++)
	{
		if (allItems[counter][i].type == Origin::Genuine)
			//in that case discard addition
			return;
	}
	allItems[counter].push_back(newTreeItem);
}

void View::onItemChanged(HTREEITEM& item)
{
	int parentId;
	vector<int> selectedItems;
	TreeItem parentTreeItem;
	HTREEITEM parentHTreeItem;
	parentHTreeItem = treeView.getParent(item);
	if (parentHTreeItem == NULL)
		return;
	parentId = FindIdByViewItem(parentHTreeItem);
	selectedItems = vector<int>();
	parentTreeItem = allItems[parentId][0];
	if (parentTreeItem.type == ItemType::ItemCurve)
	{
		addRecursivelyChildItems(selectedItems, parentHTreeItem);
		m_engineNotifier->SetC0CurvePoints(parentId, selectedItems);
	}
	else if (parentTreeItem.type == ItemType::ItemC2Curve)
	{
		addRecursivelyChildItems(selectedItems, parentHTreeItem);
		m_engineNotifier->SetC2CurvePoints(parentId, selectedItems);
	}
	else if (parentTreeItem.type == ItemType::ItemC2Interpolated)
	{
		addRecursivelyChildItems(selectedItems, parentHTreeItem);
		m_engineNotifier->SetC2InterpolatedCurvePoints(parentId, selectedItems);
	}
}

void View::ReconstructSurface(HTREEITEM surface)
{
	int surfaceId = FindIdByViewItem(surface);
	HTREEITEM patch = treeView.getChild(surface);

	while (patch != NULL)
	{
		vector<int> ids;
		int patchId = FindIdByViewItem(patch);
		addRecursivelyChildItems(ids, patch);
		if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemBezierSurface)
		{
			m_engineNotifier->SetPatchPoints(surfaceId, patchId, ids);
		}
		else if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemBSplineSurface)
		{
			m_engineNotifier->SetBSplinePatchPoints(surfaceId, patchId, ids);
		}
		else if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemGregorySurface)
		{
			m_engineNotifier->SetGregoryPatchPoints(surfaceId, patchId, ids);
		}
		patch = treeView.getNext(patch);
	}
	if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemBezierSurface)
	{
		m_engineNotifier->SetDimensionsForBezierSurface(surfaceId, m_rows, m_cols, m_surfaceWidth, m_surfaceHeight);
		m_engineNotifier->TranslateBezierSurfacePoints(surfaceId, isSurfacePlane);
	}
	else if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemBSplineSurface)
	{
		m_engineNotifier->SetDimensionsForBSplineSurface(surfaceId, m_rows, m_cols, m_surfaceWidth, m_surfaceHeight);
		m_engineNotifier->TranslateBSplineSurfacePoints(surfaceId, isSurfacePlane);
	}
	else if (GetTreeItemByHTREEITEM(surface).type == ItemType::ItemGregorySurface)
	{
		m_engineNotifier->TranslateGregoryPoints(surfaceId);
	}
}

void View::OnSelectedChanged()
{
	/*vector<int> selectedItems = vector<int>();*/
	vector<int> selectedItems;
	if (isMultiSelectActive)
		selectedItems = previouslySelectedItems;
	else
		selectedItems = vector<int>();
	HTREEITEM selected = treeView.getSelected();
	int selectedId = FindIdByViewItem(selected);
	//there are following scenarios:
	//first when bezier curve is selected then we need to have posibility of adding
	//new points to curve so we do not want to select all its points - we return

	if (allItems[selectedId][0].type == ItemType::ItemCurve || 
		allItems[selectedId][0].type == ItemType::ItemC2Curve || 
		allItems[selectedId][0].type == ItemType::ItemC2Interpolated)
		return;
	//if we select any patch type we want to select all its child points
	if (allItems[selectedId][0].type == ItemType::ItemPatch ||
		allItems[selectedId][0].type == ItemType::ItemBSplinePatch ||
		allItems[selectedId][0].type == ItemType::ItemGregoryPatch)
	{
		map<int, int> ids = map<int, int>();
		HTREEITEM child = treeView.getChild(selected);
		while (child != NULL)
		{
			int childId = FindIdByViewItem(child);
			if (ids.find(childId) == ids.end())
				ids.insert(pair<int, int>(childId, 1));
			child = treeView.getNext(child);
		}
		for (map<int, int> ::iterator it = ids.begin(); it != ids.end(); it++)
			selectedItems.push_back(it->first);
		m_engineNotifier->SetActiveModels(selectedItems);
	} 
	//if we select any surface type we want to select all its child points recursively,
	//including all patch points, but not patch types themselves
	else if (allItems[selectedId][0].type == ItemType::ItemBezierSurface ||
		allItems[selectedId][0].type == ItemType::ItemBSplineSurface||
		allItems[selectedId][0].type == ItemType::ItemGregorySurface)
	{
		map<int, int> ids = map<int, int>();
		HTREEITEM patch = treeView.getChild(selected);
		while (patch != NULL)
		{
			HTREEITEM child = treeView.getChild(patch);
			while (child != NULL)
			{
				int childId = FindIdByViewItem(child);
				if (ids.find(childId) == ids.end())
					ids.insert(pair<int, int>(childId, 1));
				child = treeView.getNext(child);
			}
			patch = treeView.getNext(patch);
		}
		for (map<int, int> ::iterator it = ids.begin(); it != ids.end(); it++)
			selectedItems.push_back(it->first);
		m_engineNotifier->SetActiveModels(selectedItems);
	}
	else if (allItems[selectedId][0].type == ItemType::ItemPoint)
	{
		//second when other model is selected we want to notify engine about multiple selection
		selectedItems.push_back(selectedId);
		HTREEITEM child = treeView.getChild(selected);
		while (child != NULL)
		{
			int childId = FindIdByViewItem(child);
			selectedItems.push_back(childId);
			child = treeView.getNext(child);
		}
		m_engineNotifier->SetActiveModels(selectedItems);
	}
	else
	{
		//third handles selection of all other models
		selectedItems.push_back(selectedId);
		m_engineNotifier->SetActiveModels(selectedItems);
	}
	previouslySelectedItems = selectedItems;
}



///////////////////////////////////////////////////////////////////////////////
// add a child item
///////////////////////////////////////////////////////////////////////////////
void View::addChildItem()
{
    static int count = 0;
    wstringstream wss;

    wss << L"Child " << ++count << ends;

	treeView.insertItem((wchar_t*)wss.str().c_str(), treeView.getRoot(), TVI_LAST, 0, 1);
}



///////////////////////////////////////////////////////////////////////////////
// delete an item
///////////////////////////////////////////////////////////////////////////////
void View::deleteChildItem()
{
    HTREEITEM hItem;
    hItem = treeView.getChild(treeView.getRoot());
    treeView.deleteItem(hItem);
}

TreeItem View::GetTreeItemByHTREEITEM(HTREEITEM item)
{
	int itemId = FindIdByViewItem(item);
	for (int i = 0; i < allItems[itemId].size(); i++)
	{
		if (allItems[itemId][i].handle == item)
			return allItems[itemId][i];
	}
}

int View::GetIndexOfTreeItemInVector(TreeItem treeItem)
{
	for (int i = 0; i < allItems[treeItem.id].size(); i++)
	{
		if (treeItem.handle == allItems[treeItem.id][i].handle)
		{
			return i;
		}
	}
}


void View::removeItem(HTREEITEM item)
{
	TreeItem treeItem = GetTreeItemByHTREEITEM(item);
	int treeItemId = treeItem.id;
	if (treeItem.type == ItemType::ItemPoint)
	{
		if (treeItem.origin == Origin::Copy)
		{
			//remove single copy
			HTREEITEM toRemove = item;
			HTREEITEM parentHTreeItem = treeView.getParent(toRemove);
			treeView.deleteItem(toRemove);
			int parentId = FindIdByViewItem(parentHTreeItem);
			vector<int>selectedItems = vector<int>();
			TreeItem parentTreeItem = allItems[parentId][0];
			if (parentTreeItem.type == ItemType::ItemCurve)
			{
				addRecursivelyChildItems(selectedItems, parentHTreeItem);
				m_engineNotifier->SetC0CurvePoints(parentId, selectedItems);
			}
			else if (parentTreeItem.type == ItemType::ItemC2Curve)
			{
				addRecursivelyChildItems(selectedItems, parentHTreeItem);
				m_engineNotifier->SetC2CurvePoints(parentId, selectedItems);
			}
			else if (parentTreeItem.type == ItemType::ItemC2Interpolated)
			{
				addRecursivelyChildItems(selectedItems, parentHTreeItem);
				m_engineNotifier->SetC2InterpolatedCurvePoints(parentId, selectedItems);
			}
			int indexToRemove = GetIndexOfTreeItemInVector(treeItem);
			allItems[treeItemId].erase(allItems[treeItemId].begin() + indexToRemove);
		}
		else if (treeItem.origin == Origin::Genuine)
		{
			//remove all copies together with genuine
			m_engineNotifier->RemoveModel(treeItem.id);
			while (allItems[treeItemId].size() > 0)
			{
				TreeItem item = allItems[treeItemId][0];
				HTREEITEM toRemove = allItems[treeItemId][0].handle;
				HTREEITEM parentHTreeItem = treeView.getParent(toRemove);
				treeView.deleteItem(toRemove);
				if (parentHTreeItem == NULL)
				{
					allItems[treeItemId].erase(allItems[treeItemId].begin());
					continue;
				}
				int parentId = FindIdByViewItem(parentHTreeItem);
				vector<int>selectedItems = vector<int>();
				TreeItem parentTreeItem = allItems[parentId][0];
				if (parentTreeItem.type == ItemType::ItemCurve)
				{
					addRecursivelyChildItems(selectedItems, parentHTreeItem);
					m_engineNotifier->SetC0CurvePoints(parentId, selectedItems);
				}
				else if(parentTreeItem.type == ItemType::ItemC2Curve)
				{
					addRecursivelyChildItems(selectedItems, parentHTreeItem);
					m_engineNotifier->SetC2CurvePoints(parentId, selectedItems);
				}
				else if (parentTreeItem.type == ItemType::ItemC2Interpolated)
				{
					addRecursivelyChildItems(selectedItems, parentHTreeItem);
					m_engineNotifier->SetC2InterpolatedCurvePoints(parentId, selectedItems);
				}
				allItems[treeItemId].erase(allItems[treeItemId].begin());
				//////////
			}
		}
	}
	else if (treeItem.type == ItemType::ItemCurve || 
		treeItem.type == ItemType::ItemC2Curve || 
		treeItem.type == ItemType::ItemBezierSurface ||
		treeItem.type == ItemType::ItemBSplineSurface||
		treeItem.type == ItemType::ItemGregorySurface)
	{
		vector<HTREEITEM>selectedItems;
		addRecursivelyChildTreeItems(selectedItems, item);
		for (int i = 0; i < selectedItems.size(); i++)
		{
			removeItem(selectedItems[i]);
		}
		m_engineNotifier->RemoveModel(treeItem.id);
		treeView.deleteItem(item);
		int indexToRemove = GetIndexOfTreeItemInVector(treeItem);
		allItems[treeItemId].erase(allItems[treeItemId].begin() + indexToRemove);
	}
	else if (treeItem.type == ItemType::ItemPatch || treeItem.type == ItemType::ItemTorus || treeItem.type == ItemType::ItemBSplinePatch || treeItem.type == ItemType::ItemGregoryPatch || treeItem.type == ItemType::ItemIntersectionCurve)
	{
		
		vector<HTREEITEM>selectedItems;
		vector<wstring>texts;
		addRecursivelyChildTreeItems(selectedItems, item);
		for (int i = 0; i < selectedItems.size(); i++)
		{
			texts.push_back(GetTreeItemByHTREEITEM(selectedItems[i]).text);
			removeItem(selectedItems[i]);
		}
		DestroyGenuinePoints(texts);
		m_engineNotifier->RemoveModel(treeItem.id);
		treeView.deleteItem(item);
		int indexToRemove = GetIndexOfTreeItemInVector(treeItem);
		allItems[treeItemId].erase(allItems[treeItemId].begin() + indexToRemove);
	}
}


void View::DestroyGenuinePoints(vector<wstring>&selectedItems)
{
	HTREEITEM hRoot = TVI_ROOT;
	HTREEITEM child = treeView.getChild(hRoot);
	while (child != NULL)
	{
		TreeItem treeItem = GetTreeItemByHTREEITEM(child);
		HTREEITEM toRemove = child;
		child = treeView.getNext(child);
		if (treeItem.type == ItemType::ItemPoint)
		{
			for (int i = 0; i < selectedItems.size(); i++)
			{
				if (selectedItems[i].compare(treeItem.text) == 0 && treeItem.origin == Origin::Genuine)
				{
					removeItem(toRemove);
					break;
				}
			}
		}

	}
}



///////////////////////////////////////////////////////////////////////////////
// remember the handle of editbox control when an item of TreeView start editing
// NMTVDISPINFO consists of NMHDR and TVITEM.
///////////////////////////////////////////////////////////////////////////////
void View::tvBeginLabelEdit(NMTVDISPINFO* tvDispInfo)
{
    if(tvDispInfo->hdr.hwndFrom == treeView.getHandle())
    {
        // limit the length of text
        // NOTE: It does not need to handle TVN_BEGINLABELEDIT to update the edited text.
        HWND editBoxHandle = treeView.getEditControl();
        ::SendMessage(editBoxHandle, EM_SETLIMITTEXT, (WPARAM)(MAX_STRING-1), 0);
    }
}



///////////////////////////////////////////////////////////////////////////////
// update the text of editbox control
// NMTVDISPINFO consists of NMHDR and TVITEM.
///////////////////////////////////////////////////////////////////////////////
void View::tvEndLabelEdit(NMTVDISPINFO* tvDispInfo)
{
    if(tvDispInfo->hdr.hwndFrom == treeView.getHandle())
    {
        // NMTVDISPINFO contains TVITEM struct to identify which item is edited
        // and the new edited string in TVITEM.pszText.
        // We only need call TVM_SETITEM here.
        treeView.setItem(&tvDispInfo->item);
    }
}



///////////////////////////////////////////////////////////////////////////////
// handle key down notification for TreeView control
// NMTVKEYDOWN consists of NMHDR, WORD(key) and UINT(flags).
///////////////////////////////////////////////////////////////////////////////
void View::tvKeyDown(NMTVKEYDOWN* tvKeyDown)
{
    if(tvKeyDown->hdr.hwndFrom == treeView.getHandle())
    {
    }

}



///////////////////////////////////////////////////////////////////////////////
// create drag image with TVM_CREATEDRAGIMAGE
///////////////////////////////////////////////////////////////////////////////
void View::tvBeginDrag(NMTREEVIEW* tv)
{
    if(tv->hdr.hwndFrom == treeView.getHandle())
    {
        // create dragging image using TVM_CREATEDRAGIMAGE
        // you have to delete it after drop operation
        this->tvDraggedItem = tv->itemNew.hItem;
        this->tvDraggedImageList = treeView.createDragImage(tvDraggedItem);

        //@@ NOT NEEDED
        // get bounding rectangle
        // NOTE: RECT should contains HTREEITEM of the dragged item that the rectangle is being retrieved for.
        //RECT rect;
        //*(HTREEITEM*)&rect = tv->itemNew.hItem;
        //treeView.getItemRect(&rect);

        // start dragging operation
        // PARAMS: HIMAGELIST, imageIndex, xHotspot, yHotspot
        ::ImageList_BeginDrag(tvDraggedImageList, 0, 0, 0);
        ::ImageList_DragEnter(treeView.getHandle(), tv->ptDrag.x, tv->ptDrag.y);

        // redirect mouse input to the parent window
        ::SetCapture(::GetParent(treeView.getHandle()));
        //::ShowCursor(false);

        tvItemDragged = true;
    }
}



///////////////////////////////////////////////////////////////////////////////
// dragging TreeView item
///////////////////////////////////////////////////////////////////////////////
void View::tvDragItem(HWND parentHandle, int x, int y)
{
    // convert the dialog coords to control coords
    POINT point;
    point.x = (SHORT)x;
    point.y = (SHORT)y;
    ::ClientToScreen(parentHandle, &point);
    ::ScreenToClient(treeView.getHandle(), &point);

    // drag the item to the current the cursor position
    ::ImageList_DragMove(point.x, point.y);

    // hide the dragged image, so the background can be refreshed
    ::ImageList_DragShowNolock(false);

    // find out if the pointer is on a item
    // If so, highlight the item as a drop target.
    TVHITTESTINFO hitTestInfo;
    hitTestInfo.pt.x = point.x;
    hitTestInfo.pt.y = point.y;
    HTREEITEM targetItem = treeView.hitTest(&hitTestInfo);
    if(targetItem)
    {
        treeView.selectItem(targetItem, TVGN_DROPHILITE);
    }

    // show the dragged image
    ::ImageList_DragShowNolock(true);
}

int View::FindIdByViewItem(HTREEITEM toFind)
{
	for (int i = 0; i < 3000; i++)
	{
		for (int j = 0; j < allItems[i].size(); j++)
			if (allItems[i][j].handle == toFind)
			return allItems[i][j].id;
			//return i;
	}
}

void View :: CopyItem(HTREEITEM source, HTREEITEM target)
{
	this->moveTreeViewItem(&treeView, source, target);

	//notify about changes
	int parentId = FindIdByViewItem(target);
	vector<int> selectedItems = vector<int>();
	TreeItem parent = allItems[parentId][0];
	if (parent.type == ItemType::ItemCurve)
	{
		addRecursivelyChildItems(selectedItems, target);
		m_engineNotifier->SetC0CurvePoints(parentId, selectedItems);
	}
	else if (parent.type == ItemType::ItemC2Curve)
	{
		addRecursivelyChildItems(selectedItems, target);
		m_engineNotifier->SetC2CurvePoints(parentId, selectedItems);
	}
	else if (parent.type == ItemType::ItemC2Interpolated)
	{
		addRecursivelyChildItems(selectedItems, target);
		m_engineNotifier->SetC2InterpolatedCurvePoints(parentId, selectedItems);
	}
}


///////////////////////////////////////////////////////////////////////////////
// drop the item whem mouse button is up
///////////////////////////////////////////////////////////////////////////////
void View::tvDropItem()
{
    // move the item to the target
    HTREEITEM targetItem = treeView.getDropHilight();
    this->moveTreeViewItem(&treeView, tvDraggedItem, targetItem);

    // finish drag operation
    ::ImageList_EndDrag();
	//FIXME
    //::ImageList_Destroy(tvDraggedImageList);
    ::ReleaseCapture();
    ::ShowCursor(true);

    // needed to avoid the target item is always highlighted
    treeView.selectItem(0, TVGN_DROPHILITE);

    // clear global variables
    tvDraggedItem = 0;
    tvDraggedImageList = 0;
    tvItemDragged = false;


	//notify about changes
	int parentId = FindIdByViewItem(targetItem);
	vector<int> selectedItems = vector<int>();
	TreeItem parent = allItems[parentId][0];
	if (parent.type == ItemType::ItemCurve)
	{
		addRecursivelyChildItems(selectedItems, targetItem);
		m_engineNotifier->SetC0CurvePoints(parentId, selectedItems);
	}
	else if (parent.type == ItemType::ItemC2Curve)
	{
		addRecursivelyChildItems(selectedItems, targetItem);
		m_engineNotifier->SetC2CurvePoints(parentId, selectedItems);
	}
	else if (parent.type == ItemType::ItemC2Interpolated)
	{
		addRecursivelyChildItems(selectedItems, targetItem);
		m_engineNotifier->SetC2InterpolatedCurvePoints(parentId, selectedItems);
	}
}


void View::addRecursivelyChildItems(vector<int>& selectedItems, HTREEITEM parent)
{
	int parentId = FindIdByViewItem(parent);
	int selectedId = parentId;
	HTREEITEM child = treeView.getChild(parent);
	while (child != NULL)
	{
		int childId = FindIdByViewItem(child);
		selectedItems.push_back(childId);
		child = treeView.getNext(child);
		if (child != NULL)
			addRecursivelyChildItems(selectedItems, child);
	}
}

void View::addRecursivelyChildTreeItems(vector<HTREEITEM>& selectedItems, HTREEITEM parent)
{
	int parentId = FindIdByViewItem(parent);
	int selectedId = parentId;
	HTREEITEM child = treeView.getChild(parent);
	while (child != NULL)
	{
		int childId = FindIdByViewItem(child);
		selectedItems.push_back(child);
		child = treeView.getNext(child);
		//many level
		//if (child != NULL)
		//	addRecursivelyChildTreeItems(selectedItems, child);
	}
}



///////////////////////////////////////////////////////////////////////////////
// helper for drag and drop a TreeView item to target item
///////////////////////////////////////////////////////////////////////////////
void View::moveTreeViewItem(TreeView* tv, HTREEITEM draggedItem, HTREEITEM targetItem)
{
    if(draggedItem == 0)
        return;

    // skip if targetItem is same as draggedItem
    if(draggedItem == targetItem)
        return;

    // skip if targetItem is same as parent
    if(tv->getParent(draggedItem) == targetItem)
        return;

    // root item cannot move
    if(draggedItem == tv->getRoot())
        return;

    // store the dragged item and its children into a list
    this->buildItemList(tv, draggedItem);

    // cannot move to its own child item
    for(size_t i = 0; i < items.size(); ++i)
    {
        if(items[i].handle == targetItem)
            return;
    }

    TreeItem treeItem;
    HTREEITEM oldItem = 0;
    HTREEITEM newItem = 0;

    // set the parent of the dragged item
    items[0].parentHandle = targetItem;

    // loop
    for(size_t i = 0; i < items.size(); ++i)
    {
        oldItem = items[i].handle;
        newItem = *tv->insertItem(items[i].text.c_str(), items[i].parentHandle,
                                 items[i].previousHandle, items[i].imageIndex,
                                 items[i].selectedImageIndex);

		//////////FIXME///////
		TreeItem oldTreeItem = GetTreeItemByHTREEITEM(oldItem);
		//tutaj musi byæ lista handle
		TreeItem newTreeItem = TreeItem();
		newTreeItem.handle = newItem;
		newTreeItem.id = oldTreeItem.id;
		newTreeItem.type = oldTreeItem.type;
		//by drag&drop we create only copies
		newTreeItem.origin = Origin::Copy;
		allItems[oldTreeItem.id].push_back(newTreeItem);

        // update parent and previous handle in the list
        for(size_t j = i+1; j < items.size(); ++j)
        {
            if(items[j].parentHandle == oldItem)
                items[j].parentHandle = newItem;
            if(items[j].previousHandle == oldItem)
                items[j].previousHandle = newItem;
        }
    }

    // we have now the copy of dragged item
    // delete the dragged item in the previous position

	////////////FIXME/////////
    //tv->deleteItem(draggedItem);

}

HTREEITEM View::getSelected()
{
	return treeView.getSelected();
}

void View::SetMultiSelect(bool isActive)
{
	isMultiSelectActive = isActive;
	if (!isMultiSelectActive)
	{
		vector<int> selectedItems = vector<int>();
		m_engineNotifier->SetActiveModels(selectedItems);
	}
}



///////////////////////////////////////////////////////////////////////////////
// helper to get TVITEM data into TreeItem
///////////////////////////////////////////////////////////////////////////////
void View::getItemInfo(TreeView* tv, HTREEITEM item, TreeItem* treeItem)
{
    if(!tv || !treeItem)
        return;

    // string storage
    wchar_t str[MAX_STRING];

    // get TVITEM struct of item
    TVITEM tvItem;
    tvItem.hItem = item;
    tvItem.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT | TVIF_PARAM; // all attributes
    tvItem.pszText = str;
    tvItem.cchTextMax = MAX_STRING - 1;
    tv->getItem(&tvItem);

    // store info to TreeItem
    treeItem->text = str;
    treeItem->handle = item;
    treeItem->parentHandle = tv->getParent(item);
    treeItem->previousHandle = tv->getPrevious(item);
    treeItem->imageIndex = tvItem.iImage;
    treeItem->selectedImageIndex = tvItem.iSelectedImage;
}



///////////////////////////////////////////////////////////////////////////////
// store the item to be moved and its children to a container
///////////////////////////////////////////////////////////////////////////////
void View::buildItemList(TreeView* tv, HTREEITEM fromItem)
{
    // clear the container
    items.clear();

    if(!tv)
        return;

    TreeItem treeItem;

    // first, add the dragged item
    this->getItemInfo(tv, fromItem, &treeItem);
    items.push_back(treeItem);

    // add children if it has
    HTREEITEM firstChildItem = tv->getChild(fromItem);
    this->addChildren(tv, firstChildItem);
}



///////////////////////////////////////////////////////////////////////////////
// recursive method to add children items
///////////////////////////////////////////////////////////////////////////////
void View::addChildren(TreeView* tv, HTREEITEM firstChild)
{
    if(!tv)
        return;

    // check if no children
    if(!firstChild)
        return;

    TreeItem treeItem;
    HTREEITEM item = firstChild;    // start from the first child item

    do
    {
        this->getItemInfo(tv, item, &treeItem);
        items.push_back(treeItem);

        // add sub-children first before sibling
        this->addChildren(tv, tv->getChild(item));

        // next sibling
        item = tv->getNext(item);
    }
    while(item != 0);
}



///////////////////////////////////////////////////////////////////////////////
// show pop-up menu
// The cursor position is in screen coordinates. It needs to convert to client
// coords for TreeView hittest.
///////////////////////////////////////////////////////////////////////////////
void View::showPopupMenu(HWND handle, int x, int y)
{
    if(handle == treeView.getHandle())
    {
        // convert the cursor position to client coords
        POINT point;
        point.x = (SHORT)x;
        point.y = (SHORT)y;
        ::ScreenToClient(treeView.getHandle(), &point);

        // find out the item under the cursor
        TVHITTESTINFO hitTestInfo;
        hitTestInfo.pt.x = point.x;
        hitTestInfo.pt.y = point.y;
        HTREEITEM item = treeView.hitTest(&hitTestInfo);

        // load popup menu
		HMENU menu = ::LoadMenu(::GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU2));
        HMENU subMenu = ::GetSubMenu(menu, 0);
        int command = ::TrackPopupMenu(subMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, x, y, 0, treeView.getParentHandle(), 0);
		if (command == ID_POPUP_DODAJ)
        {
            static int count = 5;
            wstringstream wss;
            wss << L"Child " << count++ << ends;

            treeView.insertItem((wchar_t*)wss.str().c_str(), item, TVI_LAST, 0, 1);
        }
		else if (command == ID_POPUP_USU40017)
        {
            // delete if it is not root
			if (item != treeView.getRoot())
				removeItem(item);
                //treeView.deleteItem(item);
        }
		else if (command == ID_POPUP_ROZWI40018)
        {
            treeView.expand(item, TVE_TOGGLE);
        }

        ::DestroyMenu(menu);
    }
}
