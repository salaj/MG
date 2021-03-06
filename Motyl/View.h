///////////////////////////////////////////////////////////////////////////////
// View.h
// ======
// View component
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-06-07
// UPDATED: 2007-06-08
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_VIEW_H
#define WIN_VIEW_H

#include <map>
#include <vector>
#include "Controls.h"

#include "scene_service.h"
#include "engine_Notifier.h"
#include "TreeItem.h"

namespace Win
{

	class InsertionParams
	{
	public:
		InsertionParams();
		InsertionParams(ModelClass* model, HTREEITEM item)
		{
			this->model = model;
			this->item = item;
		}
		ModelClass* model;
		HTREEITEM item;
	};

    class View
    {
    public:
        View();
        ~View();

        void init(HWND handle);
        void addChildItem();
        void deleteChildItem();
        void showPopupMenu(HWND handle, int x, int y);

        // handle TreeView Notifications
        void tvBeginLabelEdit(NMTVDISPINFO* tvDispInfo);
        void tvEndLabelEdit(NMTVDISPINFO* tvDispInfo);
        void tvKeyDown(NMTVKEYDOWN* tvKeyDown);
        void tvBeginDrag(NMTREEVIEW* tv);
        void tvDragItem(HWND parent, int x, int y);
        void tvDropItem();
		void CopyItem(HTREEITEM source, HTREEITEM target);
        bool isTreeViewItemDragged() const { return tvItemDragged; };
		HTREEITEM getSelected();
		InsertionParams* insertItem(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		int findIdOfGenuine(const wchar_t* str);
		//request to remove all copy items with given id
		void removeItem(HTREEITEM item);
		void OnSelectedChanged();
		void ReconstructSurface(HTREEITEM surface);

		void SetEngineNotifier(EngineNotifier*);
		int m_rows, m_cols;
		double m_surfaceWidth, m_surfaceHeight;
		bool isSurfacePlane = true;
		void SetMultiSelect(bool isActive);
    private:
        // member functions
        void moveTreeViewItem(TreeView* tv, HTREEITEM dragged, HTREEITEM target);
        void buildItemList(TreeView* tv, HTREEITEM from);
        void getItemInfo(TreeView* tv, HTREEITEM item, TreeItem* treeItem);
        void addChildren(TreeView* tv, HTREEITEM first);
		//it is used to create origin point - handled at root hierarchy
		//when point has been added directly to curve
		void addOriginPointItem(const wchar_t*);
		int FindIdByViewItem(HTREEITEM);
		TreeItem GetTreeItemByHTREEITEM(HTREEITEM);
		int GetIndexOfTreeItemInVector(TreeItem);
		void DestroyGenuinePoints(vector<wstring>&selectedItems);

        TreeView treeView;
        Button   buttonExit;
        HMENU tvMenu;                           // handle to popup menu
        HIMAGELIST imageList;                   // handle to imagelist for items

        // for drag-and-drop operation in TreeView
        HTREEITEM tvDraggedItem;                // handle to item being dragged
        HIMAGELIST tvDraggedImageList;          // handle to LmageList created for dragging
        bool tvItemDragged;
        std::vector <TreeItem> items;           // container for items to be moved

		//HTREEITEM getHandle();
		void addRecursivelyChildItems(vector<int>&, HTREEITEM);
		void addRecursivelyChildTreeItems(vector<HTREEITEM>&, HTREEITEM);
		void onItemChanged(HTREEITEM& item);
		
		static int counter;
		
		//HTREEITEM treeItems[100];
		vector<TreeItem> allItems[3000];
		EngineNotifier* m_engineNotifier;

		bool isMultiSelectActive = false;
		vector<int> previouslySelectedItems;

    };
}

#endif
