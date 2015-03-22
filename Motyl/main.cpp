#include "scene.h"
#include "window.h"
#include "settings.h"
#include "exceptions.h"

using namespace std;
using namespace gk2;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	shared_ptr<ApplicationBase> app;
	shared_ptr<Window> w;
	shared_ptr<Settings> settings;
	int exitCode = 0;
	try
	{
		w.reset(new Window(hInstance, 600, 600, L"Geometric modelling", true));
		InputClass* input = w->GetInputClass();
		settings.reset(new Settings(hInstance, 600, 600, L"Settings",input, false));
		app.reset(new Scene(hInstance, input));
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