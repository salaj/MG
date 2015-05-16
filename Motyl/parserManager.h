#include <iostream>
#include "loader.h"

using namespace std;

class ParserManger{
public:
	ParserManger();
	void LoadScene(string pathToLoadFile);
private:
	Loader m_loader;
};