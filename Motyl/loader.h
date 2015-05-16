
#include <iostream>
#include <fstream>
#include <string>
#include "modelclass.h"

using namespace std;

enum ParseType{
	ParsePoint,
	ParseCurve,
	ParseC2Curve,
	ParseC2Interpolated,
	ParseTorus,
	ParseRoot,
	ParsePatch,
	ParseBSplinePatch,
	ParseBezierSurface,
	ParseBSplineSurface
};

class Loader{
public:
	Loader();
	void LoadScene(string pathToLoadFile);
private:
	void parseModel(FILE* myReadfile, ParseType itemType);
	XMMATRIX& parseModelMatrix(FILE* myReadfile);
};