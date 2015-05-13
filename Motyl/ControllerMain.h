#ifndef WIN_CONTROLLER_MAIN_H
#define WIN_CONTROLLER_MAIN_H

#include "Controller.h"
#include "View.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain() {};
        ~ControllerMain() {};

        int create();
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int contextMenu(HWND handle, int x, int y);
        int close();
        int notify(int id, LPARAM lParam);
        int mouseMove(WPARAM state, int x, int y);
        int lButtonUp(WPARAM state, int x, int y);

		HTREEITEM insertItem(const wchar_t* str, ItemType type = ItemType::ItemPoint, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		void removeItem(int id);
		int getSelectetTreeViewItem();
		void SetEngineNotifier(EngineNotifier*);
		void ReconstructSurface(HTREEITEM surface);
		Win::View  view;
    private:
        //Win::Model model;
		int m_selectedTreeViewItem;
		int m_previousSelectedTreeViewItem;
    };
}

#endif
