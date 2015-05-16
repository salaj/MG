#include "loader.h"

Loader::Loader()
{

}

void Loader::LoadScene(string pathToLoadFile)
{
	FILE* myReadfile;
	 myReadfile = fopen(pathToLoadFile.c_str(), "r");
	string line;
	char last[255];
	while (fscanf(myReadfile, "%s\n", &line) != EOF)
	{
		if (strcmp(line.c_str(), "Torus")== 0)
		{
			parseModel(myReadfile, ParseType::ParseTorus);
		}
		strcpy(last, line.c_str());
	}
}

XMMATRIX& Loader::parseModelMatrix(FILE* myReadfile)
{
	fscanf(myReadfile, "TMtx=");
	XMMATRIX m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			float val;
			fscanf(myReadfile, "%f", &val);
			m(i, j) = val;
		}
	fscanf(myReadfile, "\n");
	return m;
}

void Loader::parseModel(FILE* myReadFile, ParseType parseType)
{
	int id;
	fscanf(myReadFile, "Id=%d\n", &id);
	string name;
	fscanf(myReadFile, "Name=%s\n", &name);
	float R;
	float r;
	if (parseType == ParseType::ParseTorus)
	{
		fscanf(myReadFile, "R=%f\n", &R);
		fscanf(myReadFile, "r=%f\n", &r);
	}
	float X;
	float Y;
	float Z;
	fscanf(myReadFile, "X=%f\n", &X);
	fscanf(myReadFile, "Y=%f\n", &Y);
	fscanf(myReadFile, "Z=%f\n", &Z);
	XMMATRIX modelMatrix = parseModelMatrix(myReadFile);
	string color;
	fscanf(myReadFile, "Color=%s\n", &color);
	fscanf(myReadFile, "\n");
	int a = 10;
}