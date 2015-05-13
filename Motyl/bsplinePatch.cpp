#include "bsplinePatch.h"


BSplinePatch::BSplinePatch()
{
}


BSplinePatch::~BSplinePatch()
{
}

BSplinePatch::BSplinePatch(const BSplinePatch&)
{
}

BSplinePatch::BSplinePatch(Service& service) : ModelClass(service)
{
	m_Type = ModelType::BSplinePatchType;
	ModelClass::Initialize();
}


void* BSplinePatch::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BSplinePatch::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BSplinePatch::Initialize()
{
	verticalSpaces = horizontalSpaces = 4;
	////////////////FIXME////////
	if (m_nodes.size() < 4)
	{
		m_vertexCount = m_vertexCountContour = 1;
		VertexPos* vertices = new VertexPos[m_vertexCount];
		m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
		m_vertexBufferContour = m_device.CreateVertexBuffer(vertices, m_vertexCountContour);
		setPointTopology();
		delete[] vertices;
		return;
	}
}
vector<SimplePoint*> BSplinePatch::GetNodes()
{
	return m_nodes;
}

void BSplinePatch::SetNodes(vector<ModelClass*>& nodes)
{
	int numberOfNodes = nodes.size();
	vector<SimplePoint*> simplePoints = vector<SimplePoint*>();
	for (int i = 0; i < numberOfNodes; i++)
	{
		simplePoints.push_back(dynamic_cast<SimplePoint*>(nodes[i]));
	}
	m_nodes = simplePoints;
	Reset();
}

XMMATRIX BSplinePatch::createBernteinBaseVector(float val, bool transpose)
{
	XMVECTOR B;
	float t = val;
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	XMMATRIX matrix;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix.m[i][j] = 0.0f;
	if (transpose)
	{
		matrix.m[0][0] = uuu;
		matrix.m[1][0] = 3 * t * uu;
		matrix.m[2][0] = 3 * tt * u;
		matrix.m[3][0] = ttt;
	}
	else
	{
		matrix.m[0][0] = uuu;
		matrix.m[0][1] = 3 * t * uu;
		matrix.m[0][2] = 3 * tt * u;
		matrix.m[0][3] = ttt;
	}
	return matrix;
}

void BSplinePatch::createDeBoorPointMatrix()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			matrixOfX.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().x;
			matrixOfY.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().y;
			matrixOfZ.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().z;
		}
}

float BSplinePatch::calculateSingleDeBoor(int n, int i, double t, double* knots, int maxI)
{
	if (n == 0)
	{
		if (i < 0 || i >= maxI)
			return 0.0f;

		if (t >= knots[i] && t < knots[i + 1])
			return 1.0f;
		return 0.0f;
	}
	else
	{
		double left = (t - knots[i]) / (knots[i + n] - knots[i]);
		if (i + n >= maxI)
			left = 0;
		if (knots[i + n] - knots[i] == 0)
			left = 0;
		double leftRecursive = calculateSingleDeBoor(n - 1, i, t, knots, maxI);

		double right = (knots[i + n + 1] - t) / (knots[i + n + 1] - knots[i + 1]);
		if (i + n + 1 >= maxI)
			right = 0;
		if (knots[i + n + 1] - knots[i + 1] == 0)
			right = 0;

		double rightRecursive = calculateSingleDeBoor(n - 1, i + 1, t, knots, maxI);
		return left * leftRecursive + right * rightRecursive;
	}
}


void BSplinePatch::generateLines(list<VertexPos*>& vertices)
{
	double numberOfDots = 50;
	int n = 3;
	int m = m_nodes.size();
	double* T = new double[m + n + 1];
	T[0] = 0.0f;
	for (int i = 0; i <= m + 1; i++)
		T[i + 1] = (double)i;
	T[m + n] = (double)(m + 1);

	float values_u[4];
	float values_v[4];

	for (int l = 0; l < horizontalSpaces; l++)
	{
		float u = (float)l / (float)(horizontalSpaces - 1);
		u += 2;
		for (int i = 0; i < 4; i++)
		{
			values_u[i] = calculateSingleDeBoor(n, i, u, T, m + n + 1);
		}
		for (int k = 0; k < numberOfDots; k++)
		{
			float v = (float)k / (float)(numberOfDots - 1);
			v += 2;
			for (int j = 0; j < 4; j++)
			{
				values_v[j] = calculateSingleDeBoor(n, j, v, T, m + n + 1);
			}
			float resultX, resultY, resultZ;
			resultX = resultY = resultZ = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					resultX += matrixOfX.m[i][j] * values_u[i] * values_v[j];
					resultY += matrixOfY.m[i][j] * values_u[i] * values_v[j];
					resultZ += matrixOfZ.m[i][j] * values_u[i] * values_v[j];
				}
			}
			vertices.push_back(new VertexPos{
				XMFLOAT3(
				resultX,
				resultY,
				resultZ
				) });
		}
	}

	for (int l = 0; l < verticalSpaces; l++)
	{
		float v = (float)l / (float)(verticalSpaces - 1);
		v += 2;
		for (int j = 0; j < 4; j++)
		{
			values_v[j] = calculateSingleDeBoor(n, j, v, T, m + n + 1);
		}
		for (int k = 0; k < numberOfDots; k++)
		{
			float u = (float)k / (float)(numberOfDots - 1);
			u += 2;
			for (int i = 0; i < 4; i++)
			{
				values_u[i] = calculateSingleDeBoor(n, i, u, T, m + n + 1);
			}
			float resultX, resultY, resultZ;
			resultX = resultY = resultZ = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					resultX += matrixOfX.m[i][j] * values_u[i] * values_v[j];
					resultY += matrixOfY.m[i][j] * values_u[i] * values_v[j];
					resultZ += matrixOfZ.m[i][j] * values_u[i] * values_v[j];
				}
			}
			vertices.push_back(new VertexPos{
				XMFLOAT3(
				resultX,
				resultY,
				resultZ
				) });
		}
	}
}

void BSplinePatch::generateBezierNet(VertexPos* verticesContour)
{
	int internalNodesCounter = 0;
	for (int i = 0; i < m_nodes.size(); i += 4)
	{
		verticesContour[internalNodesCounter++] = { m_nodes[i]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 3]->GetPosition3() };
	}
	for (int i = 0; i < 4; i++)
	{
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 0]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
		verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 3]->GetPosition3() };
	}
}


void BSplinePatch::Reset()
{
	createDeBoorPointMatrix();
	list<VertexPos*> vertices = list<VertexPos*>();
	//we start from Second Point cause we want segments continuity C0
	int index = 0;
	int numberOfControlPoints = m_nodes.size() - 1;
	int segmentLength = 0;
	generateLines(vertices);

	m_vertexCountContour = (4 + 4 - 2) * 4 * 2; //48
	VertexPos* verticesContour;
	verticesContour = new VertexPos[m_vertexCountContour];
	generateBezierNet(verticesContour);

	m_vertexCount = vertices.size();
	VertexPos *arr = new VertexPos[m_vertexCount];
	int ind = 0;
	for (list<VertexPos*>::iterator it = vertices.begin(); it != vertices.end(); it++)
	{
		arr[ind++] = *(*it);
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(arr, m_vertexCount);
	m_vertexBufferContour = m_device.CreateVertexBuffer(verticesContour, m_vertexCountContour);
	delete arr;
	delete verticesContour;
	setPointTopology();
}

void BSplinePatch::setPointTopology()
{
	m_indexCount = m_vertexCount;
	unsigned short* indices = new unsigned short[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
	delete[] indices;
	m_indexCountContour = m_vertexCountContour;
	unsigned short* indicesContour = new unsigned short[m_indexCountContour];
	for (int i = 0; i < m_indexCountContour; i++)
	{
		indicesContour[i] = i;
	}
	m_indexBufferContour = m_device.CreateIndexBuffer(indicesContour, m_indexCountContour);
	delete[] indicesContour;
}

void BSplinePatch::setTriangleTopology()
{

}
void BSplinePatch::setLineTopology()
{

}

void BSplinePatch::Draw()
{
	BSplinePatchShader* shader = dynamic_cast<BSplinePatchShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(shader->getTopology());
	if (m_input->isStereoscopyActive)
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		if (m_input->isBezierPolygonActive)
		{
			m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
			b = m_vertexBufferContour.get();
			m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
			m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
			m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
			m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
			m_context->DrawIndexed(m_indexCountContour, 0, 0);
			m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
			m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
			m_context->DrawIndexed(m_indexCountContour, 0, 0);
		}
	}
	else
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		if (m_input->isBezierPolygonActive)
		{
			m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
			b = m_vertexBufferContour.get();
			m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
			m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
			m_context->DrawIndexed(m_indexCountContour, 0, 0);
		}
	}
}