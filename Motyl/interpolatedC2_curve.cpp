#include "interpolatedC2_curve.h"

using namespace gk2;


InterpolatedC2Curve::InterpolatedC2Curve(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : BezierC2Curve(deviceContext, shader_base, device, camera, input)
{
}

InterpolatedC2Curve::InterpolatedC2Curve(Service& service) : BezierC2Curve(service)
{
	m_Type = ModelType::InterpolatedC2Type;
}

InterpolatedC2Curve::InterpolatedC2Curve(const BezierCurve& other) : BezierC2Curve(other)
{
}


InterpolatedC2Curve::~InterpolatedC2Curve()
{
}

InterpolatedC2Curve::InterpolatedC2Curve()
{
}

void* InterpolatedC2Curve::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void InterpolatedC2Curve::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void InterpolatedC2Curve::Draw()
{
	BezierC2Curve::Draw();
}

void InterpolatedC2Curve::SetNodes(vector<ModelClass*> nodes)
{
	m_interpolants = nodes;
	m_deBoor.clear();
	for (int i = 0; i < m_interpolants.size(); i++)
	{
		SimplePoint* origin = dynamic_cast<SimplePoint*>(m_interpolants[i]);
		SimplePoint* copy = new SimplePoint(*origin);
		m_deBoor.push_back(copy);
	}
	createBaseFunctionsMatrix();
	Reset();
}

void InterpolatedC2Curve::createBaseFunctionsMatrix()
{
	int n = 3;
	int m = m_deBoor.size();

	if (m <= n)
		return;

	int cols, rows;
	cols = rows = m;
	float** matrix = new float*[rows];

	double* T = new double[m + n + 1];
	T[0] = 0.0f;
	for (int i = 0; i <= m + 1; i++)
		T[i + 1] = (double)i;
	T[m + n] = (double)(m + 1);

	//double T[8] = { 0, 0, 1, 2, 3, 4, 5, 5 };

	double tao = 2.0f;
	double step = (m - n) / (double)(m - 1);

	double length = 0;
	for (int i = 1; i < m_interpolants.size(); i++)
	{
		XMFLOAT3 pos1 = m_interpolants[i - 1]->GetPosition3();
		XMFLOAT3 pos2 = m_interpolants[i]->GetPosition3();
		float singleSegment = (pos1.x - pos2.x) *  (pos1.x - pos2.x) +
			(pos1.y - pos2.y) *  (pos1.y - pos2.y) +
			(pos1.z - pos2.z) *  (pos1.z - pos2.z);
		length += singleSegment;
	}

	double* steps = new double[m];
	double singleLength = 0;
	steps[0] = tao;
	steps[m - 1] = tao + (m - n);
	double dif = steps[m - 1] - steps[0];
	for (int i = 1; i < m_interpolants.size(); i++)
	{
		XMFLOAT3 pos1 = m_interpolants[i - 1]->GetPosition3();
		XMFLOAT3 pos2 = m_interpolants[i]->GetPosition3();
		singleLength += (pos1.x - pos2.x) *  (pos1.x - pos2.x) +
			(pos1.y - pos2.y) *  (pos1.y - pos2.y) +
			(pos1.z - pos2.z) *  (pos1.z - pos2.z);
		steps[i] = steps[0] + dif * (singleLength / length);
	}


	for (int i = 0; i < rows; i++)
	{
		float* row = new float[cols];
		matrix[i] = row;
		for (int j = 0; j < cols; j++)
		{
			float deBoorValue = calculateSingleDeBoor(n, j, tao, T, m + n + 1);
			matrix[i][j] = deBoorValue;
		}
		//tao += step;
		tao = steps[i + 1];
	}

	for (int i = 0; i < rows - 1; i++)
	{
		float leadingValue = matrix[i][i];
		for (int j = 1; j <= 3; j++)
			reductSingleElementDown(i, i + j, matrix, rows, cols, n, leadingValue);
	}

	//traverse from the bottom
	for (int i = rows - 1; i >= 0; i--)
	{
		float leadingValue = matrix[i][i];
		dividePoint(i, leadingValue, matrix);

		//reduct above elements
		for (int j = 1; j <= 3; j++)
			reductSingleElementUp(i, i - j, matrix);
	}


	for (int i = 0; i < rows; i++)
		delete[] matrix[i];
	delete[] matrix;
}

void InterpolatedC2Curve::UpdateNode(SimplePoint* point)
{
	m_deBoor.clear();
	for (int i = 0; i < m_interpolants.size(); i++)
	{
		SimplePoint* origin = dynamic_cast<SimplePoint*>(m_interpolants[i]);
		SimplePoint* copy = new SimplePoint(*origin);
		m_deBoor.push_back(copy);
	}
	createBaseFunctionsMatrix();
	Reset();
}


void InterpolatedC2Curve::reductSingleElementDown(int i, int j, float** matrix, int rows, int cols, int n, float leadingValue)
{
	if (j < rows)
	{
		float secondBelowLeadingValue = matrix[j][i];
		float factor = secondBelowLeadingValue / leadingValue;
		if (factor != 0)
		{
			int min = min(i + n + 1, cols);
			for (int k = i; k < min; k++)
			{
				float reduction = matrix[i][k] * factor;
				matrix[j][k] -= reduction;
			}
			substractPoint(j, i, factor);
		}
	}
}

void InterpolatedC2Curve::reductSingleElementUp(int i, int j, float** matrix)
{
	if (j >= 0)
	{
		float aboveElement = matrix[j][i];
		if (aboveElement != 0)
		{
			substractPoint(j, i, aboveElement);
			matrix[j][i] = 0;
		}
	}
}

void InterpolatedC2Curve::dividePoint(int index, float dividor, float** matrix)
{
	if (dividor == 0)
		return;
	XMFLOAT3 pos = m_deBoor[index]->GetPosition3();
	pos.x /= dividor;
	pos.y /= dividor;
	pos.z /= dividor;
	m_deBoor[index]->SetPosition(pos.x, pos.y, pos.z);
	matrix[index][index] /= dividor; //so it is one
}


void InterpolatedC2Curve::substractPoint(int indexFirst, int indexSecond, float factor)
{
	XMFLOAT3 posFirst = m_deBoor[indexFirst]->GetPosition3();
	XMFLOAT3 posSecond = m_deBoor[indexSecond]->GetPosition3();
	posFirst.x -= factor * posSecond.x;
	posFirst.y -= factor * posSecond.y;
	posFirst.z -= factor * posSecond.z;

	m_deBoor[indexFirst]->SetPosition(posFirst.x, posFirst.y, posFirst.z);
}



