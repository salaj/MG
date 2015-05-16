#include "parserManager.h"

ParserManger::ParserManger()
{

}

void ParserManger::LoadScene(string pathToLoadFile)
{
	m_loader.LoadScene(pathToLoadFile);
}