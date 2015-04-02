#pragma once

#include <string>
#include <Windows.h>
#include "window_service.h"
#include "Windowsx.h"

namespace gk2
{
	class Window
	{
	public:
		Window(HINSTANCE hInstance, int width, int height);
		Window(HINSTANCE hInstance, int width, int height, const std::wstring& title, bool shouldRegister);
		virtual ~Window();

		virtual void Show(int cmdShow);
		SIZE getClientSize() const;
		inline HWND getHandle() const { return m_hWnd; }
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
		WindowService* GetWindowService();
		HWND m_dialog;
	protected:
		virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static bool IsWindowClassRegistered(HINSTANCE hInstance);
		static void RegisterWindowClass(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		static std::wstring m_windowClassName;
		static int m_defaultWindowWidth;
		static int m_defaultWindowHeight;

		Window(const gk2::Window& other) { /* Do not use!*/ }

		void CreateWindowHandle(int width, int height, const std::wstring& title, bool shouldRegister);

		HWND m_hWnd;

		HINSTANCE m_hInstance;
		bool isLeftButtonPressed = false;
		//InputClass* input;
		WindowService m_WindowService;

		///////////////////
	};
	/////////////
	// GLOBALS //
	/////////////
	static Window* ApplicationHandle = 0;

}