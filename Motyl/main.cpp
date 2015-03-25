#include "scene.h"
#include "window.h"
#include "settings.h"
#include "exceptions.h"
#include "guiUpdater.h"

#include <commctrl.h>
#include "DialogWindow.h"
#include "ControllerMain.h"
#include "resource.h"
#include "procedure.h"

using namespace std;
using namespace gk2;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	shared_ptr<ApplicationBase> app;
	shared_ptr<Window> w;
	shared_ptr<Settings> settings;
	shared_ptr<GUIUpdater> guiUpdater;
	int exitCode = 0;
	try
	{
		//INITCOMMONCONTROLSEX commonCtrls;
		//commonCtrls.dwSize = sizeof(commonCtrls);
		//commonCtrls.dwICC = ICC_TREEVIEW_CLASSES;
		//InitCommonControlsEx(&commonCtrls);

		w.reset(new Window(hInstance, 600, 600, L"Geometric modelling", true));
		InputClass* input = w->GetInputClass();
		settings.reset(new Settings(hInstance, 600, 600, L"Settings",input, false));
		guiUpdater.reset(new GUIUpdater(settings.get()));
		app.reset(new Scene(hInstance, input, guiUpdater.get()));

		//////
		//// register TreeView from comctl32.dll before creating windows

		//// create a child dialog box contains controls
		//Win::ControllerMain ctrl;
		//Win::DialogWindow dialog(hInstance, IDD_DIALOG1, 0, &ctrl);
		//dialog.create();
		//dialog.show();

		//////


		exitCode = app->Run(w.get(), settings.get(), cmdShow);
	}
	catch (Exception& e)
	{
		MessageBoxW(NULL, e.getMessage().c_str(), L"B³¹d", MB_OK);
		exitCode = e.getExitCode();
	}
	catch(...)
	{
		MessageBoxW(NULL, L"Nieznany B³¹d", L"B³¹d", MB_OK);
		exitCode = -1;
	}
	w.reset();
	app.reset();

	return exitCode;
}