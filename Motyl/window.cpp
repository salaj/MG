#include "window.h"
#include "exceptions.h"
#include <basetsd.h>

using namespace std;
using namespace gk2;

wstring Window::m_windowClassName = L"DirectX 11 Window";
int Window::m_defaultWindowWidth = 1280;
int Window::m_defaultWindowHeight = 720;

InputClass* input;

void Window::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW c;
	ZeroMemory(&c, sizeof(WNDCLASSEXW));

	c.cbSize = sizeof(WNDCLASSEXW);
	c.style = CS_HREDRAW | CS_VREDRAW;
	c.lpfnWndProc = WndProc;
	c.hInstance = hInstance;
	c.hCursor = LoadCursor(NULL, IDC_ARROW);
	c.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	c.lpszMenuName = NULL;
	c.lpszClassName = m_windowClassName.c_str();
	c.cbWndExtra = sizeof(LONG_PTR);
	if (!RegisterClassExW(&c))
		THROW_WINAPI;
}
bool Window::IsWindowClassRegistered(HINSTANCE hInstance)
{
	WNDCLASSEXW c;
	return GetClassInfoExW(hInstance, m_windowClassName.c_str(), &c) == TRUE;
}

Window::Window(HINSTANCE hInstance, int width, int height)
	: m_hInstance(hInstance)
{
	CreateWindowHandle(width, height, m_windowClassName);
}

Window::Window(HINSTANCE hInstance, int width, int height, const std::wstring& title)
	: m_hInstance(hInstance)
{
	CreateWindowHandle(width, height, title);
}

void Window::CreateWindowHandle(int width, int height, const wstring& title)
{
	if (!IsWindowClassRegistered(m_hInstance))
		RegisterWindowClass(m_hInstance);
	RECT rect = { 0, 0, width, height};
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (!AdjustWindowRect(&rect, style, FALSE))
		THROW_WINAPI;
	m_hWnd = CreateWindowW(m_windowClassName.c_str(), title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, this);
	if (!m_hWnd)
		THROW_WINAPI;
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}



#define IDM_VIEW_TRANSLATION 1
#define IDM_VIEW_ROTATION 2
#define IDM_VIEW_SCALE 3

#define IDM_AXIS_X 4
#define IDM_AXIS_Y 5
#define IDM_AXIS_Z 6

#define IDM_BIG_CIRCLE 7
#define IDM_SMALL_CIRCLE 8

HWND ghSb;
HMENU ghMenuActions;
HMENU ghMenuAxis;
HMENU ghMenuTorrusRadius;

void AddMenus(HWND hwnd)
{
	HMENU hMenubar;

	hMenubar = CreateMenu();
	ghMenuActions = CreateMenu();
	ghMenuAxis = CreateMenu();
	ghMenuTorrusRadius = CreateMenu();

	AppendMenuW(ghMenuActions, MF_STRING, IDM_VIEW_TRANSLATION, L"Translation");
	AppendMenuW(ghMenuActions, MF_STRING, IDM_VIEW_ROTATION, L"Rotation");
	AppendMenuW(ghMenuActions, MF_STRING, IDM_VIEW_SCALE, L"Scale");


	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenuActions, L"&Actions");


	AppendMenuW(ghMenuAxis, MF_STRING, IDM_AXIS_X, L"X");
	AppendMenuW(ghMenuAxis, MF_STRING, IDM_AXIS_Y, L"Y");
	AppendMenuW(ghMenuAxis, MF_STRING, IDM_AXIS_Z, L"Z");


	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenuAxis, L"&Axis");


	AppendMenuW(ghMenuTorrusRadius, MF_STRING, IDM_BIG_CIRCLE, L"Big circle");
	AppendMenuW(ghMenuTorrusRadius, MF_STRING, IDM_SMALL_CIRCLE, L"Small circle");


	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenuTorrusRadius, L"&Elipsoid radius");



	//AppendMenuW(ghMenuTorrusRadius, MF_STRING, IDM_BIG_CIRCLE, L"Big circle");
	//AppendMenuW(ghMenuTorrusRadius, MF_STRING, IDM_SMALL_CIRCLE, L"Small circle");


	//AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenuTorrusRadius, L"&Light intensity");

	SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

InputClass* Window::GetInputClass()
{
	return input;
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	UINT state;
	switch (msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(m_hWnd, &paintStruct);
		EndPaint(m_hWnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:

		switch (LOWORD(wParam)) {
		case IDM_VIEW_TRANSLATION:

			state = GetMenuState(ghMenuActions, IDM_VIEW_TRANSLATION, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuActions, IDM_VIEW_TRANSLATION, MF_UNCHECKED);
			}
			else {
				input->SetActiveFeature(Translation);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuActions, IDM_VIEW_TRANSLATION, MF_CHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_ROTATION, MF_UNCHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_SCALE, MF_UNCHECKED);
			}

			break;

		case IDM_VIEW_ROTATION:

			state = GetMenuState(ghMenuActions, IDM_VIEW_ROTATION, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuActions, IDM_VIEW_ROTATION, MF_UNCHECKED);
			}
			else {
				input->SetActiveFeature(Rotation);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuActions, IDM_VIEW_TRANSLATION, MF_UNCHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_ROTATION, MF_CHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_SCALE, MF_UNCHECKED);
			}

			break;

		case IDM_VIEW_SCALE:

			state = GetMenuState(ghMenuActions, IDM_VIEW_SCALE, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuActions, IDM_VIEW_SCALE, MF_UNCHECKED);
			}
			else {
				input->SetActiveFeature(Scale);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuActions, IDM_VIEW_TRANSLATION, MF_UNCHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_ROTATION, MF_UNCHECKED);
				CheckMenuItem(ghMenuActions, IDM_VIEW_SCALE, MF_CHECKED);
			}

			break;



		case IDM_AXIS_X:

			state = GetMenuState(ghMenuAxis, IDM_AXIS_X, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_X, MF_UNCHECKED);
			}
			else {
				input->SetActiveAxis(AxisX);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_X, MF_CHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Y, MF_UNCHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Z, MF_UNCHECKED);
			}

			break;

		case IDM_AXIS_Y:

			state = GetMenuState(ghMenuAxis, IDM_AXIS_Y, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Y, MF_UNCHECKED);
			}
			else {
				input->SetActiveAxis(AxisY);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_X, MF_UNCHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Y, MF_CHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Z, MF_UNCHECKED);
			}

			break;

		case IDM_AXIS_Z:

			state = GetMenuState(ghMenuAxis, IDM_AXIS_Z, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Z, MF_UNCHECKED);
			}
			else {
				input->SetActiveAxis(AxisZ);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_X, MF_UNCHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Y, MF_UNCHECKED);
				CheckMenuItem(ghMenuAxis, IDM_AXIS_Z, MF_CHECKED);
			}

			break;

		case IDM_BIG_CIRCLE:

			state = GetMenuState(ghMenuTorrusRadius, IDM_BIG_CIRCLE, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuTorrusRadius, IDM_BIG_CIRCLE, MF_UNCHECKED);
			}
			else {
				input->SetActiveRadius(BigRadius);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuTorrusRadius, IDM_BIG_CIRCLE, MF_CHECKED);
				CheckMenuItem(ghMenuTorrusRadius, IDM_SMALL_CIRCLE, MF_UNCHECKED);
			}

			break;

		case IDM_SMALL_CIRCLE:

			state = GetMenuState(ghMenuTorrusRadius, IDM_SMALL_CIRCLE, MF_BYCOMMAND);

			if (state == SW_SHOWNA) {
				ShowWindow(ghSb, SW_HIDE);
				CheckMenuItem(ghMenuTorrusRadius, IDM_SMALL_CIRCLE, MF_UNCHECKED);
			}
			else {
				input->SetActiveRadius(SmallRadius);
				ShowWindow(ghSb, SW_SHOWNA);
				CheckMenuItem(ghMenuTorrusRadius, IDM_SMALL_CIRCLE, MF_CHECKED);
				CheckMenuItem(ghMenuTorrusRadius, IDM_BIG_CIRCLE, MF_UNCHECKED);
			}

			break;

		}
		break;

	case WM_CREATE:
		input = new InputClass;
		// Initialize the input object.
		input->Initialize();

		AddMenus(m_hWnd);

		//InitCommonControls();

		ghSb = CreateWindowExW(0, L"TickMenu", NULL,
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, m_hWnd,
			(HMENU)1, GetModuleHandle(NULL), NULL);

		break;
	default:
		//return DefWindowProc(m_hWnd, msg, wParam, lParam);
		return ApplicationHandle->MessageHandler(m_hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Window::Show(int cmdShow)
{
	ShowWindow(m_hWnd, cmdShow);
}

SIZE Window::getClientSize() const
{
	RECT r;
	GetClientRect(m_hWnd, &r);
	SIZE s = { r.right - r.left, r.bottom - r.top };
	return s;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Window* wnd = (Window*)pcs->lpCreateParams;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, PtrToUlong(wnd));
		wnd->m_hWnd = hWnd;
		return wnd->WndProc(msg, wParam, lParam);
	}
	else
	{
		Window *wnd = reinterpret_cast<Window*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)));
		return wnd ? wnd->WndProc(msg, wParam, lParam) : DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
