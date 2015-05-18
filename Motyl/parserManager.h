#include <iostream>
#include "loader.h"
#include "saver.h"

using namespace std;

class ParserManger{
public:
	ParserManger();
	void Initialize(SettingsHelper* settingsHelper);
	void LoadScene(string pathToLoadFile);
	void SaveScene(string pathToSaveFile);

	ModelsManager* modelsManager;

private:
	Loader m_loader;
	Saver m_saver;
};