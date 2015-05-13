#pragma once
#include <string>
#include <Windows.h>
#include "inputclass.h"
#include "settingsHelper.h"
#include "View.h"
#include "ControllerMain.h"
#include <xnamath.h>
#include <cstdlib>

namespace gk2
{
	class Settings
	{
	public:
		Settings(HINSTANCE hInstance, int width, int height);
		Settings(HINSTANCE hInstance, int width, int height,
			const std::wstring& title, InputClass* input,
			bool shouldRegister);
		virtual ~Settings();

		virtual void Show(int cmdShow);
		SIZE getClientSize() const;
		inline HWND getHandle() const { return m_hWnd; }
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
		InputClass* GetInputClass();
		HWND m_dialog, hWndComboBox, m_baseComboBox, m_surfaceCombo, m_trackBarVertical, m_trackBarHorizontal,
			m_PatchesWidth, m_PatchesHeight, m_SurfaceWidth, m_SurfaceHeight;

		//HTREEITEM insertItemInternally(const wchar_t* str, HTREEITEM parent = TVI_ROOT, HTREEITEM insertAfter = TVI_LAST, int imageIndex = 0, int selectedImageIndex = 0);
		void insertItemExternally();
		//void removeItem(int id);

		void setCursorPositionWorld(XMFLOAT4 position);
		void setCursorPositionScreen(XMFLOAT4 position);

		SettingsHelper* GetSettingsHelper();
		void SetEngineNotifier(EngineNotifier*);

		HWND m_hWnd, m_list;

	protected:
		virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static bool IsWindowClassRegistered(HINSTANCE hInstance);
		static void RegisterWindowClass(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		static std::wstring m_windowClassName;
		static int m_defaultWindowWidth;
		static int m_defaultWindowHeight;

		Settings(const gk2::Settings& other) { /* Do not use!*/ }

		void CreateWindowHandle(int width, int height, const std::wstring& title, bool shouldRegister);



		HINSTANCE m_hInstance;
		InputClass* m_input;		
		//Win::ControllerMain m_controller;

		SettingsHelper m_settingsHelper;

		int yLayoutOffset = 70;

		///////////////////
	};
	/////////////
	// GLOBALS //
	/////////////
	static Settings* ApplicationHandleSettings = 0;

}
