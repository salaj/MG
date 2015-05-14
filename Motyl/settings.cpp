
#include "settings.h"
#include "exceptions.h"
#include "resource.h"
#include <basetsd.h>



using namespace std;
using namespace gk2;

wstring Settings::m_windowClassName = L"DirectX 11 Window";
int Settings::m_defaultWindowWidth = 1280;
int Settings::m_defaultWindowHeight = 720;

#define RADIO_STEREOSCOPY 200
#define ADD_POINT_BUTTON 201
#define DROPDOWN_TYPE 207
#define X_POS 202
#define Y_POS 203
#define Z_POS 204

#define X_POS_SCREEN 212
#define Y_POS_SCREEN 213
#define APPLY_BUTTON 205
#define RADIO_BEZIERPOLYGON 206
#define COMBO_BASE 210
#define ID_TRACKBARVERTICAL 211
#define ID_TRACKBARHORIZONTAL 214
#define PATCHES_WIDTH 215
#define PATCHES_HEIGHT 216
#define SURFACE_WIDTH 217
#define SURFACE_HEIGHT 218
#define COMBO_SURFACE 219

//InputClass* input;

void Settings::SetEngineNotifier(EngineNotifier* engineNotifier)
{
	m_settingsHelper.m_controller.SetEngineNotifier(engineNotifier);
}

void Settings::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW c;
	ZeroMemory(&c, sizeof(WNDCLASSEXW));

	c.cbSize = sizeof(WNDCLASSEXW);
	c.style = CS_HREDRAW | CS_VREDRAW;
	c.lpfnWndProc = WndProc;
	c.hInstance = hInstance;
	c.hCursor = LoadCursor(NULL, IDC_ARROW);
	c.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	c.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MENU1); //NULL;
	c.lpszClassName = m_windowClassName.c_str();
	c.cbWndExtra = sizeof(LONG_PTR);


	if (!RegisterClassExW(&c))
		THROW_WINAPI;
	//if (!RegisterClassW(&d))
	//	THROW_WINAPI;

}
bool Settings::IsWindowClassRegistered(HINSTANCE hInstance)
{
	WNDCLASSEXW c;
	return GetClassInfoExW(hInstance, m_windowClassName.c_str(), &c) == TRUE;
}

Settings::Settings(HINSTANCE hInstance, int width, int height)
	: m_hInstance(hInstance)
{
	CreateWindowHandle(width, height, m_windowClassName, true);
}

Settings::Settings(HINSTANCE hInstance, int width, int height, const std::wstring& title, InputClass* input, bool shouldRegister = false)
	: m_hInstance(hInstance), m_input(input)
{
	CreateWindowHandle(width, height, title, shouldRegister);
}


void Settings::CreateWindowHandle(int width, int height, const wstring& title, bool shouldRegister)
{
	if (shouldRegister && !IsWindowClassRegistered(m_hInstance))
		RegisterWindowClass(m_hInstance);
	RECT rect = { 0, 0, width, height };
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (!AdjustWindowRect(&rect, style, FALSE))
		THROW_WINAPI;
	m_hWnd = CreateWindowW(m_windowClassName.c_str(), title.c_str(), style, 800, 200,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, this);

	RECT Rect;
	GetWindowRect(m_hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(m_hWnd), (LPPOINT)&Rect, 2);

	//m_dialog = CreateWindowW(TEXT("MDICLIENT"), L"Settings",
	//	WS_OVERLAPPED | WS_VISIBLE,
	//	Rect.right, Rect.top, 200, rect.bottom - rect.top, m_hWnd, NULL, m_hInstance,
	//	this);

	//// Ensure that the common control DLL is loaded. 
	InitCommonControls();

	CreateWindow(TEXT("button"), TEXT("Stereoscopy active"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 20, 150, 35,
		m_hWnd, (HMENU)RADIO_STEREOSCOPY, m_hInstance, this);

	CheckDlgButton(m_hWnd, RADIO_STEREOSCOPY, BST_CHECKED);

	CreateWindow(TEXT("button"), TEXT("Polygon active"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 70, 150, 35,
		m_hWnd, (HMENU)RADIO_BEZIERPOLYGON, m_hInstance, this);

	CheckDlgButton(m_hWnd, RADIO_BEZIERPOLYGON, BST_CHECKED);


	//CreateWindow(TEXT("button"), TEXT("Add point"),
	//	WS_VISIBLE | WS_CHILD | BS_BUTTON,
	//	250, 20, 150, 35,
	//	m_hWnd, (HMENU), m_hInstance, this);

	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Cursor Position"),
		WS_CHILD | WS_VISIBLE,
		80, 70 + yLayoutOffset, 120, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		20, 100 + yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)X_POS,
		GetModuleHandle(NULL),
		NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		100, 100 + yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)Y_POS,
		GetModuleHandle(NULL),
		NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		180, 100 +yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)Z_POS,
		GetModuleHandle(NULL),
		NULL);


	CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Apply"),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		250, 100 + yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)APPLY_BUTTON,
		m_hInstance,
		NULL);

	UINT iMin = 4;     // minimum value in trackbar range 
	UINT iMax = 20;     // maximum value in trackbar range 
	UINT iSelMin = 4;  // minimum value in trackbar selection 
	//UINT iSelMax = 10;

	m_trackBarVertical = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		TEXT("Trackbar Control"),              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_AUTOTICKS |
		TBS_ENABLESELRANGE,              // style 
		190, 20,                          // position 
		150, 30,                         // size 
		m_hWnd,                         // parent window 
		(HMENU)ID_TRACKBARVERTICAL,                     // control identifier 
		m_hInstance,                         // instance 
		NULL                             // no WM_CREATE parameter 
		);

	SendMessage(m_trackBarVertical, TBM_SETRANGE,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions
	SendMessage(m_trackBarVertical, TBM_SETPAGESIZE,
		0, (LPARAM)4);                  // new page size 
	SendMessage(m_trackBarVertical, TBM_SETPOS,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)iSelMin);

	m_trackBarHorizontal= CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		TEXT("Trackbar Control"),              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_AUTOTICKS |
		TBS_ENABLESELRANGE,              // style 
		190, yLayoutOffset,                          // position 
		150, 30,                         // size 
		m_hWnd,                         // parent window 
		(HMENU)ID_TRACKBARHORIZONTAL,                     // control identifier 
		m_hInstance,                         // instance 
		NULL                             // no WM_CREATE parameter 
		);

	SendMessage(m_trackBarHorizontal, TBM_SETRANGE,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions
	SendMessage(m_trackBarHorizontal, TBM_SETPAGESIZE,
		0, (LPARAM)4);                  // new page size 
	SendMessage(m_trackBarHorizontal, TBM_SETPOS,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)iSelMin);



	//////////SECOND ROW/////////////
	CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		20, 140 + yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)X_POS_SCREEN,
		GetModuleHandle(NULL),
		NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		100, 140 + yLayoutOffset, 50, 20,
		m_hWnd,
		(HMENU)Y_POS_SCREEN,
		GetModuleHandle(NULL),
		NULL);

	////////////THIRD ROW//////////////
	 m_list = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, L"TreeView",
		WS_CHILD | WS_VISIBLE | WS_BORDER |
		TVS_HASBUTTONS | TVS_HASLINES |
		TVS_LINESATROOT | TVS_CHECKBOXES | TVS_EDITLABELS,
		20, 200 + yLayoutOffset, 250, 300, m_hWnd, (HMENU)IDC_TREE1, m_hInstance, NULL);

	 m_settingsHelper.m_controller.setHandle(m_hWnd);

	 m_settingsHelper.m_controller.create();

	////////////SECOND COLUMN///////////////

	 CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Add model"),
		 WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		 350, 20, 150, 35,
		 m_hWnd,
		 (HMENU)ADD_POINT_BUTTON,
		 m_hInstance,
		 NULL);


	hWndComboBox = CreateWindow(TEXT("combobox"), NULL,
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_TABSTOP,
		350, 100, 200, 100, m_hWnd, (HMENU)DROPDOWN_TYPE, m_hInstance, NULL);

	//Send combobox items individually
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::SimplePointName).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::TorusName).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BezierEdgeName).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BezierC2EdgeName).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BezierC2Interpolated).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BezierSurface).c_str());
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BSplineSurface).c_str());

	SendMessage(hWndComboBox, CB_SETCURSEL, 0, 0); //highlight/select the first option



	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Number of bezier patches"),
		WS_CHILD | WS_VISIBLE,
		350, 200, 180, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Width"),
		WS_CHILD | WS_VISIBLE,
		300, 240, 50, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	m_PatchesWidth = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		370, 240, 50, 20,
		m_hWnd,
		(HMENU)PATCHES_WIDTH,
		GetModuleHandle(NULL),
		NULL);

	int value = 3;
	TCHAR buf[32];
	swprintf(buf, TEXT("%d"), value);
	SetDlgItemText(m_hWnd, PATCHES_WIDTH, buf);

	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Height"),
		WS_CHILD | WS_VISIBLE,
		430, 240, 50, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	m_PatchesHeight = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		500, 240, 50, 20,
		m_hWnd,
		(HMENU)PATCHES_HEIGHT,
		GetModuleHandle(NULL),
		NULL);
	value = 1;
	TCHAR buffer[32];
	swprintf(buffer, TEXT("%d"), value);
	SetDlgItemText(m_hWnd, PATCHES_HEIGHT, buffer);



	//////////////NEXT/////////////////
	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Size of surface"),
		WS_CHILD | WS_VISIBLE,
		350, 280, 180, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Width"),
		WS_CHILD | WS_VISIBLE,
		300, 320, 50, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	m_SurfaceWidth = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		370, 320, 50, 20,
		m_hWnd,
		(HMENU)SURFACE_WIDTH,
		GetModuleHandle(NULL),
		NULL);

	value = 20;
	TCHAR bufWidth[32];
	swprintf(bufWidth, TEXT("%d"), value);
	SetDlgItemText(m_hWnd, SURFACE_WIDTH, bufWidth);

	CreateWindowEx(0,
		TEXT("STATIC"),
		TEXT("Height"),
		WS_CHILD | WS_VISIBLE,
		430, 320, 50, 20,
		m_hWnd,
		NULL,
		m_hInstance,
		NULL);

	m_SurfaceHeight = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		500, 320, 50, 20,
		m_hWnd,
		(HMENU)SURFACE_HEIGHT,
		GetModuleHandle(NULL),
		NULL);
	value = 40;
	TCHAR bufferHeight[32];
	swprintf(bufferHeight, TEXT("%d"), value);
	SetDlgItemText(m_hWnd, SURFACE_HEIGHT, bufferHeight);



	/////////BASE COMBOBOX//////////////
	m_baseComboBox = CreateWindow(TEXT("combobox"), NULL,
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_TABSTOP,
		350, 130, 200, 100, m_hWnd, (HMENU)COMBO_BASE, m_hInstance, NULL);

	//Send combobox items individually
	SendMessage(m_baseComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BernsteinBase).c_str());
	SendMessage(m_baseComboBox, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::BSplineBase).c_str());
	SendMessage(m_baseComboBox, CB_SETCURSEL, 0, 0); //highlight/select the first option

	//////SURFACE COMBOBOX//////////////

	m_surfaceCombo = CreateWindow(TEXT("combobox"), NULL,
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_TABSTOP,
		350, 160, 200, 100, m_hWnd, (HMENU)COMBO_SURFACE, m_hInstance, NULL);

	//Send combobox items individually
	SendMessage(m_surfaceCombo, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::PlaneSurface).c_str());
	SendMessage(m_surfaceCombo, CB_ADDSTRING, 0, (LPARAM)(SettingsHelper::CyllinderSurface).c_str());
	SendMessage(m_surfaceCombo, CB_SETCURSEL, 0, 0); //highlight/select the first option




	if (!m_hWnd)
		THROW_WINAPI;
}

Settings::~Settings()
{
	DestroyWindow(m_hWnd);
}

void Settings::setCursorPositionWorld(XMFLOAT4 position)
{
	float value = position.x;
	TCHAR buf[32];
	swprintf(buf, TEXT("%f"), value);
	SetDlgItemText(m_hWnd, X_POS, buf);
	value = position.y;
	swprintf(buf, TEXT("%f"), value);
	SetDlgItemText(m_hWnd, Y_POS, buf);
	value = position.z;
	swprintf(buf, TEXT("%f"), value);
	SetDlgItemText(m_hWnd, Z_POS, buf);
}

void Settings::setCursorPositionScreen(XMFLOAT4 position)
{
	float value = position.x;
	TCHAR buf[32];
	swprintf(buf, TEXT("%f"), value);
	SetDlgItemText(m_hWnd, X_POS_SCREEN, buf);
	value = position.y;
	swprintf(buf, TEXT("%f"), value);
	SetDlgItemText(m_hWnd, Y_POS_SCREEN, buf);
}


LRESULT CALLBACK Settings::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		//input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		//input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

InputClass* Settings::GetInputClass()
{
	//return input;
	return NULL;
}

SettingsHelper* Settings::GetSettingsHelper()
{
	return &m_settingsHelper;
}


void Settings::insertItemExternally()
{
	wchar_t buf[30];
	int selectedComboBoxItemIndex = SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0);
	SendMessage(hWndComboBox, CB_GETLBTEXT, selectedComboBoxItemIndex, (LPARAM)buf);
	HTREEITEM item = m_settingsHelper.AddNewModelToTreeView(buf, m_list);
	//// Set the selection of the tree-view to be the first image
	//// that has analyzed contour.
	//NMHDR nmhdr;
	//NMTREEVIEW nmtv;
	//TVITEM itemNew, itemOld;
	//nmhdr.code = TVN_SELCHANGED;
	////nmhdr.hwndFrom = GetDlgItem(hDlg, IDC_TREE_GRIDFIELD);
	////nmhdr.idFrom = IDC_TREE_GRIDFIELD;
	//nmtv.hdr = nmhdr;
	//nmtv.action = TVC_BYMOUSE;
	////itemNew.lParam = _all_contours[0].element;
	//nmtv.itemNew = itemNew;
	////itemOld.lParam = _all_contours[0].element;;
	//nmtv.itemOld = itemOld;
	//SendMessage(m_list, WM_NOTIFY, WPARAM(IDC_TREE_GRIDFIELD), LPARAM(&nmtv));
	//TreeView_SelectItem(m_hWnd, item);
}


LRESULT Settings::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	UINT state;
	Win::ControllerMain* ctrl = &m_settingsHelper.m_controller;
	int selected = m_settingsHelper.m_controller.getSelectetTreeViewItem();
	int selectedComboBoxItemIndex;
	int horizontalSpaces, verticalSpaces;
	wstring modelToAddName;
	wchar_t buf[30];
	TCHAR bufferWidth[32];
	TCHAR bufferHeight[32];
	TCHAR surfaceWidth[32];
	TCHAR surfaceHeight[32];
	int a;
	//swprintf(buf, TEXT("%f"), value);
	if (selected != -1)
	{
		m_input->SetSelectedModel(selected);
	}
	switch (msg)
	{
	case WM_COMMAND:
		ctrl->command(LOWORD(wParam), HIWORD(wParam), lParam);   // id, code, msg
		switch (LOWORD(wParam)) {
		case RADIO_STEREOSCOPY:
			if (IsDlgButtonChecked(m_hWnd, RADIO_STEREOSCOPY))
			{
				CheckDlgButton(m_hWnd, RADIO_STEREOSCOPY, BST_UNCHECKED);
				m_input->setStereorcopy(false);
			}
			else
			{
				CheckDlgButton(m_hWnd, RADIO_STEREOSCOPY, BST_CHECKED);
				m_input->setStereorcopy(true);
			}
			return false;

		case RADIO_BEZIERPOLYGON:
			if (IsDlgButtonChecked(m_hWnd, RADIO_BEZIERPOLYGON))
			{
				CheckDlgButton(m_hWnd, RADIO_BEZIERPOLYGON, BST_UNCHECKED);
				m_input->setBezierPolygon(false);
			}
			else
			{
				CheckDlgButton(m_hWnd, RADIO_BEZIERPOLYGON, BST_CHECKED);
				m_input->setBezierPolygon(true);
			}
			return false;

		case ADD_POINT_BUTTON:
			selectedComboBoxItemIndex = SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0);
			SendMessage(hWndComboBox, CB_GETLBTEXT, selectedComboBoxItemIndex, (LPARAM)buf);
			GetDlgItemText(m_hWnd, PATCHES_WIDTH, bufferWidth, 32);
			ctrl->view.m_cols = _wtoi(bufferWidth) <= 0 ? 3 : _wtoi(bufferWidth);
			GetDlgItemText(m_hWnd, PATCHES_HEIGHT, bufferHeight, 32);

			GetDlgItemText(m_hWnd,SURFACE_WIDTH, surfaceWidth, 32);
			ctrl->view.m_surfaceWidth = _wtoi(surfaceWidth) <= 20 ? 20 : _wtoi(surfaceWidth);
			GetDlgItemText(m_hWnd, SURFACE_HEIGHT, surfaceHeight, 32);
			ctrl->view.m_surfaceHeight = _wtoi(surfaceHeight) <= 20 ? 20 : _wtoi(surfaceHeight);

			ctrl->view.m_rows = _wtoi(bufferHeight) <= 0 ? 1 : _wtoi(bufferHeight);
			m_settingsHelper.AddNewModelToTreeView(buf, m_list);
			return false;
		case COMBO_BASE:
			selectedComboBoxItemIndex = SendMessage(m_baseComboBox, CB_GETCURSEL, 0, 0);
			SendMessage(m_baseComboBox, CB_GETLBTEXT, selectedComboBoxItemIndex, (LPARAM)buf);
			modelToAddName = wstring(buf);
			if (modelToAddName.compare(SettingsHelper::BernsteinBase) == 0)
				m_input->setBase(true);
			else if (modelToAddName.compare(SettingsHelper::BSplineBase) == 0)
				m_input->setBase(false);
			return false;

		case COMBO_SURFACE:
			selectedComboBoxItemIndex = SendMessage(m_surfaceCombo, CB_GETCURSEL, 0, 0);
			SendMessage(m_surfaceCombo, CB_GETLBTEXT, selectedComboBoxItemIndex, (LPARAM)buf);
			modelToAddName = wstring(buf);
			if (modelToAddName.compare(SettingsHelper::PlaneSurface) == 0){
				m_settingsHelper.isSurfacePlane = true;
				m_settingsHelper.m_controller.view.isSurfacePlane = true;
			}
			else if (modelToAddName.compare(SettingsHelper::CyllinderSurface) == 0){
				m_settingsHelper.isSurfacePlane = false;
				m_settingsHelper.m_controller.view.isSurfacePlane = false;
			}
			return false;
		}
		return false;

	case WM_TIMER:
		ctrl->timer(LOWORD(wParam), HIWORD(wParam));
		return false;

	case WM_PAINT:
		hDC = BeginPaint(m_hWnd, &paintStruct);
		EndPaint(m_hWnd, &paintStruct);
		ctrl->paint();
		::DefWindowProc(m_hWnd, msg, wParam, lParam);
		return false;

	case WM_DESTROY:
		ctrl->destroy();
		return false;

	case WM_CLOSE:
		ctrl->close();
		return false;

	case WM_HSCROLL:
		ctrl->hScroll(wParam, lParam);
		return false;

	case WM_VSCROLL:
		ctrl->vScroll(wParam, lParam);
		return false;

	case WM_NOTIFY:
		ctrl->command(LOWORD(wParam), HIWORD(wParam), lParam);   // id, code, msg
		switch (LOWORD(wParam)) {
		case ID_TRACKBARVERTICAL:
			verticalSpaces = SendMessage(m_trackBarVertical, TBM_GETPOS, 0, 0);
			m_input->SetVerticalSpaces(verticalSpaces);
			return false;
		case ID_TRACKBARHORIZONTAL:
			horizontalSpaces = SendMessage(m_trackBarHorizontal, TBM_GETPOS, 0, 0);
			m_input->SetHorizontalSpaces(horizontalSpaces);
			return false;
		}
		ctrl->notify((int)wParam, lParam);                      // controllerID, lParam
		return false;

	case WM_MOUSEMOVE:
		ctrl->mouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		//ctrl->mouseMove(wParam, (int)GET_X_LPARAM(lParam), (int)GET_Y_LPARAM(lParam));  // state, x, y
		return false;

	case WM_LBUTTONUP:
		ctrl->lButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));    // state, x, y
		return false;

	case WM_CONTEXTMENU:
		ctrl->contextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam));    // handle, x, y (from screen coords)
		return false;

	default:
		//return DefWindowProc(m_hWnd, msg, wParam, lParam);
		return ApplicationHandleSettings->MessageHandler(m_hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Settings::Show(int cmdShow)
{
	ShowWindow(m_hWnd, cmdShow);
}

SIZE Settings::getClientSize() const
{
	RECT r;
	GetClientRect(m_hWnd, &r);
	SIZE s = { r.right - r.left, r.bottom - r.top };
	return s;
}

LRESULT CALLBACK Settings::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Settings* wnd = (Settings*)pcs->lpCreateParams;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, PtrToUlong(wnd));
		wnd->m_hWnd = hWnd;
		return wnd->WndProc(msg, wParam, lParam);
	}
	else
	{
		Settings *wnd = reinterpret_cast<Settings*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)));
		return wnd ? wnd->WndProc(msg, wParam, lParam) : DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
