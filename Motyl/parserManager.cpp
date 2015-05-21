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
	FILE* file = fopen(pathToLoadFile.c_str(), "r");
	char z;
	string tmp;
	while (fscanf(file, "%c", &z) != EOF)
	{
		if (z == ',')
			z = '.';
		tmp += z;
	}
	fclose(file);
	file = fopen(pathToLoadFile.c_str(), "w");
	for (int i = 0; i < tmp.size(); i++)
		fprintf(file, "%c", tmp[i]);
	fclose(file);
	m_loader.LoadScene(pathToLoadFile);
}

void ParserManger::SaveScene(string pathToSaveFile)
{
	m_saver.SaveScene(pathToSaveFile);
	//FILE* file = fopen(pathToSaveFile.c_str(), "r");
	//char z;
	//string tmp;
	//while (fscanf(file, "%c", &z) != EOF)
	//{
	//	if (z == '.')
	//		z = ',';
	//	tmp += z;
	//}
	//fclose(file);
	//file = fopen(pathToSaveFile.c_str(), "w");
	//for (int i = 0; i < tmp.size(); i++)
	//	fprintf(file, "%c", tmp[i]);
	//fclose(file);
}