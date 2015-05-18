#include "parserManager.h"

ParserManger::ParserManger()
{

}

void ParserManger::Initialize(SettingsHelper* settingsHelper)
{
	m_loader.Initialize(settingsHelper);
	m_saver.Initialize(settingsHelper, modelsManager);
}

void ParserManger::LoadScene(string pathToLoadFile)
{
	m_loader.LoadScene(pathToLoadFile);
}

void ParserManger::SaveScene(string pathToSaveFile)
{
	m_saver.SaveScene(pathToSaveFile);
}