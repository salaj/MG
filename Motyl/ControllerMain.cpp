///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2007-09-25
///////////////////////////////////////////////////////////////////////////////

#include "ControllerMain.h"
#include "resource.h"

using namespace Win;


bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);



int ControllerMain::close()
{
    // close all child windows first
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

    ::DestroyWindow(handle);                    // close itself
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// enumerate all child windows
///////////////////////////////////////////////////////////////////////////////
bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
    {
        ::SendMessage(handle, WM_CLOSE, 0, 0);  // close child windows
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE message
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::create()
{
    view.init(handle);

    return 0;
}




///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND notification
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::command(int id, int command, LPARAM message)
{
    switch(id)
    {
    case IDC_EXIT:
        if(command == BN_CLICKED)
            ::PostMessage(handle, WM_CLOSE, 0, 0);
        break;
    }

    return 0;
}




///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::notify(int id, LPARAM lParam)
{
    // first cast lParam to NMHDR* to know what the control is
    NMHDR* nmhdr = (NMHDR*)lParam;
	int w,a;

    switch(nmhdr->code)
    {
    // TreeView Notifications =========
    case TVN_BEGINDRAG:         // drag-and-drop operation has begun
        // cast again lParam to NMTREEVIEW*
        view.tvBeginDrag((NMTREEVIEW*)lParam);
        break;

    //case TVN_BEGINRDRAG:        // drag-and-drop operation using right mouse button has begun
    //    break;


    case TVN_BEGINLABELEDIT:    // label editing has begun
        // cast again lParam to NMTVDISPINFO*
        view.tvBeginLabelEdit((NMTVDISPINFO*)lParam);
        break;

    case TVN_ENDLABELEDIT:      // label editing has ended
        // cast again lParam to NMTVDISPINFO*
        view.tvEndLabelEdit((NMTVDISPINFO*)lParam);
        break;

    case TVN_DELETEITEM:        // an item has been deleted
        break;

    case TVN_GETDISPINFO:       // TreeView needs info(such as item text) to display an item
        break;

    case TVN_SETDISPINFO:       // parent window must update the item information
        break;

    case TVN_ITEMEXPANDED:      // list of items was expanded or collapsed
        break;

    case TVN_ITEMEXPANDING:     // list of items are about to be expanded or collapsed
        break;

    case TVN_KEYDOWN:           // a keyboard event has occurred
        // When the TreeView control is contained in a dialog box,
        // IsDialogMessage() processes the ESC and ENTER keystrokes and
        // does not pass them on to the edit control that is created by the TreeView control.
        // The result is that the keystrokes have no effect.
        // cast again lParam to NMTVKEYDOWN*
        //view.tvKeyDown((NMTVKEYDOWN*)lParam)
        break;

    case TVN_SELCHANGED:        // the item selection has changed: NM_TREEVIEW.action=(TVC_UNKNOWN, TVC_BYMOUSE, TVC_BYKEYBOARD)
		//m_selectedTreeViewItem = view.getIdOfSelected();
		view.OnSelectedChanged();
		break;

    case TVN_SELCHANGING:       // the item selection is about to change
        break;

    default:
        break;
    }

    // handle TreeView notifications
    return 1;
}

void ControllerMain::SetEngineNotifier(EngineNotifier* engineNotifier)
{
	view.SetEngineNotifier(engineNotifier);
}


HTREEITEM ControllerMain::insertItem(const wchar_t* str, ItemType type, HTREEITEM parent, HTREEITEM insertAfter, int imageIndex, int selectedImageIndex)
{
	if (type == ItemType::ItemPatch || (type == ItemType::ItemPoint && parent != TVI_ROOT))
		return view.insertItem(str, type, parent, TVI_LAST, 0, 1);
	else if (type == ItemType::ItemPoint)
		return view.insertItem(str, type, view.getSelected(), TVI_LAST, 0, 1);
	else
		return view.insertItem(str, type, TVI_ROOT, TVI_LAST, 0, 1);
}
void ControllerMain::ReconstructSurface(HTREEITEM surface)
{
	view.ReconstructSurface(surface);
}

int ControllerMain::getSelectetTreeViewItem()
{
	if (m_selectedTreeViewItem == m_previousSelectedTreeViewItem)
		return -1; // do not want to notify
	m_previousSelectedTreeViewItem = m_selectedTreeViewItem;
	return m_selectedTreeViewItem;
}

void ControllerMain::removeItem(int id)
{
	//view.removeItem(id);
}
///////////////////////////////////////////////////////////////////////////////
// handle mouse move
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::mouseMove(WPARAM state, int x, int y)
{
    // is TreeView item dragged ?
    if(view.isTreeViewItemDragged())
    {
        view.tvDragItem(handle, x, y);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_LBUTTONUP
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::lButtonUp(WPARAM, int x, int y)
{
    // is TreeView item dragged ?
    if(view.isTreeViewItemDragged())
    {
        view.tvDropItem();
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CONTEXTMENU when a user click the right mouse button
// The cursor position is in the screen coordinates.
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::contextMenu(HWND handle, int x, int y)
{
    view.showPopupMenu(handle, x, y);
    return 0;
}
