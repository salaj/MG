#include "gregoryPatch.h"


GregoryPatch::GregoryPatch()
{
}


GregoryPatch::~GregoryPatch()
{
}

GregoryPatch::GregoryPatch(const GregoryPatch&)
{
}

GregoryPatch::GregoryPatch(Service& service) : ModelClass(service)
{
	m_Type = ModelType::GregoryPatchType;
	ModelClass::Initialize();
}


void* GregoryPatch::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void GregoryPatch::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void GregoryPatch::Initialize()
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
vector<SimplePoint*> GregoryPatch::GetNodes()
{
	return m_nodes;
}

void GregoryPatch::SetNodes(vector<ModelClass*>& nodes)
{
	p0 = dynamic_cast<SimplePoint*>(nodes[0]);
	e0plus = dynamic_cast<SimplePoint*>(nodes[1]);
	e1minus = dynamic_cast<SimplePoint*>(nodes[2]);
	p1 = dynamic_cast<SimplePoint*>(nodes[3]);

	e0minus = dynamic_cast<SimplePoint*>(nodes[4]);
	f0minus = dynamic_cast<SimplePoint*>(nodes[5]);
	f0plus = dynamic_cast<SimplePoint*>(nodes[6]);
	f1minus = dynamic_cast<SimplePoint*>(nodes[7]);
	f1plus = dynamic_cast<SimplePoint*>(nodes[8]);
	e1plus = dynamic_cast<SimplePoint*>(nodes[9]);

	e3plus = dynamic_cast<SimplePoint*>(nodes[10]);
	f3plus = dynamic_cast<SimplePoint*>(nodes[11]);
	f3minus = dynamic_cast<SimplePoint*>(nodes[12]);
	f2plus = dynamic_cast<SimplePoint*>(nodes[13]);
	f2minus = dynamic_cast<SimplePoint*>(nodes[14]);
	e2minus = dynamic_cast<SimplePoint*>(nodes[15]);

	p3 = dynamic_cast<SimplePoint*>(nodes[16]);
	e3minus = dynamic_cast<SimplePoint*>(nodes[17]);
	e2plus = dynamic_cast<SimplePoint*>(nodes[18]);
	p2 = dynamic_cast<SimplePoint*>(nodes[19]);

	Reset();
}

XMMATRIX GregoryPatch::createBernteinBaseVector(float val, bool transpose)
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

void GregoryPatch::createDeBoorPointMatrix()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			matrixOfX.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().x;
			matrixOfY.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().y;
			matrixOfZ.m[i][j] = m_nodes[i * 4 + j]->GetPosition3().z;
		}
}

void calculate(float t, float b[])
{
	float u = 1.0f - t;
	float b30 = u * u * u;
	float b31 = 3 * t * u * u;
	float b32 = 3 * t * t * u;
	float b33 = t * t * t;

	b[0] = b30;
	b[1] = b31;
	b[2] = b32;
	b[3] = b33;
}


void GregoryPatch::generateLines(list<VertexPos*>& vertices)
{
	int numberOfDots = 200;
	XMMATRIX matrix_X, matrix_Y, matrix_Z;
	for (int k = 0; k < horizontalSpaces; k++)
	{
		float u = (float)k / (float)(horizontalSpaces - 1.0f);
		calculate(u, b_u);
		for (int l = 0; l < numberOfDots; l++)
		{
			float v = (float)l / (float)(numberOfDots - 1);
			calculate(v, b_v);

			float f0 = (u * f0plus->GetPosition3().x + v * f0minus->GetPosition3().x) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			float f1 = ((1 - u) * f1minus->GetPosition3().x + v * f1plus->GetPosition3().x) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			float f2 = ((1 - u) * f2plus->GetPosition3().x + (1 - v) * f2minus->GetPosition3().x) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			float f3 = ((u)* f3minus->GetPosition3().x + (1 - v) * f3plus->GetPosition3().x) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;
			
			matrix_X(0, 0) = p0->GetPosition3().x;
			matrix_X(0, 1) = e0minus->GetPosition3().x;
			matrix_X(0, 2) = e3plus->GetPosition3().x;
			matrix_X(0, 3) = p3->GetPosition3().x;

			matrix_X(1, 0) = e0plus->GetPosition3().x;
			matrix_X(1, 1) = f0;
			matrix_X(1, 2) = f3;
			matrix_X(1, 3) = e3minus->GetPosition3().x;

			matrix_X(2, 0) = e1minus->GetPosition3().x;
			matrix_X(2, 1) = f1;
			matrix_X(2, 2) = f2;
			matrix_X(2, 3) = e2plus->GetPosition3().x;

			matrix_X(3, 0) = p1->GetPosition3().x;
			matrix_X(3, 1) = e1plus->GetPosition3().x;
			matrix_X(3, 2) = e2minus->GetPosition3().x;
			matrix_X(3, 3) = p2->GetPosition3().x;
			/////////////////////////////////
			f0 = (u * f0plus->GetPosition3().y + v * f0minus->GetPosition3().y) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			f1 = ((1 - u) * f1minus->GetPosition3().y + v * f1plus->GetPosition3().y) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			f2 = ((1 - u) * f2plus->GetPosition3().y + (1 - v) * f2minus->GetPosition3().y) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			f3 = ((u)* f3minus->GetPosition3().y + (1 - v) * f3plus->GetPosition3().y) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;

			matrix_Y(0, 0) = p0->GetPosition3().y;
			matrix_Y(0, 1) = e0minus->GetPosition3().y;
			matrix_Y(0, 2) = e3plus->GetPosition3().y;
			matrix_Y(0, 3) = p3->GetPosition3().y;

			matrix_Y(1, 0) = e0plus->GetPosition3().y;
			matrix_Y(1, 1) = f0;
			matrix_Y(1, 2) = f3;
			matrix_Y(1, 3) = e3minus->GetPosition3().y;

			matrix_Y(2, 0) = e1minus->GetPosition3().y;
			matrix_Y(2, 1) = f1;
			matrix_Y(2, 2) = f2;
			matrix_Y(2, 3) = e2plus->GetPosition3().y;

			matrix_Y(3, 0) = p1->GetPosition3().y;
			matrix_Y(3, 1) = e1plus->GetPosition3().y;
			matrix_Y(3, 2) = e2minus->GetPosition3().y;
			matrix_Y(3, 3) = p2->GetPosition3().y;
			/////////////////////////////////
			f0 = (u * f0plus->GetPosition3().z + v * f0minus->GetPosition3().z) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			f1 = ((1 - u) * f1minus->GetPosition3().z + v * f1plus->GetPosition3().z) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			f2 = ((1 - u) * f2plus->GetPosition3().z + (1 - v) * f2minus->GetPosition3().z) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			f3 = ((u)* f3minus->GetPosition3().z + (1 - v) * f3plus->GetPosition3().z) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;

			matrix_Z(0, 0) = p0->GetPosition3().z;
			matrix_Z(0, 1) = e0minus->GetPosition3().z;
			matrix_Z(0, 2) = e3plus->GetPosition3().z;
			matrix_Z(0, 3) = p3->GetPosition3().z;

			matrix_Z(1, 0) = e0plus->GetPosition3().z;
			matrix_Z(1, 1) = f0;
			matrix_Z(1, 2) = f3;
			matrix_Z(1, 3) = e3minus->GetPosition3().z;

			matrix_Z(2, 0) = e1minus->GetPosition3().z;
			matrix_Z(2, 1) = f1;
			matrix_Z(2, 2) = f2;
			matrix_Z(2, 3) = e2plus->GetPosition3().z;

			matrix_Z(3, 0) = p1->GetPosition3().z;
			matrix_Z(3, 1) = e1plus->GetPosition3().z;
			matrix_Z(3, 2) = e2minus->GetPosition3().z;
			matrix_Z(3, 3) = p2->GetPosition3().z;



				float resultX, resultY, resultZ;
				resultX = resultY = resultZ = 0;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						resultX += matrix_X(i, j) * b_u[i] * b_v[j];
						resultY += matrix_Y(i, j) * b_u[i] * b_v[j];
						resultZ += matrix_Z(i, j) * b_u[i] * b_v[j];
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



	for (int k = 0; k < numberOfDots; k++)
	{
		float u = (float)k / (float)(numberOfDots - 1);
		calculate(u, b_u);
		for (int l = 0; l < verticalSpaces; l++)
		{
			float v = (float)l / (float)(verticalSpaces - 1);
			calculate(v, b_v);

			float f0 = (u * f0plus->GetPosition3().x + v * f0minus->GetPosition3().x) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			float f1 = ((1 - u) * f1minus->GetPosition3().x + v * f1plus->GetPosition3().x) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			float f2 = ((1 - u) * f2plus->GetPosition3().x + (1 - v) * f2minus->GetPosition3().x) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			float f3 = ((u)* f3minus->GetPosition3().x + (1 - v) * f3plus->GetPosition3().x) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;

			matrix_X(0, 0) = p0->GetPosition3().x;
			matrix_X(0, 1) = e0minus->GetPosition3().x;
			matrix_X(0, 2) = e3plus->GetPosition3().x;
			matrix_X(0, 3) = p3->GetPosition3().x;

			matrix_X(1, 0) = e0plus->GetPosition3().x;
			matrix_X(1, 1) = f0;
			matrix_X(1, 2) = f3;
			matrix_X(1, 3) = e3minus->GetPosition3().x;

			matrix_X(2, 0) = e1minus->GetPosition3().x;
			matrix_X(2, 1) = f1;
			matrix_X(2, 2) = f2;
			matrix_X(2, 3) = e2plus->GetPosition3().x;

			matrix_X(3, 0) = p1->GetPosition3().x;
			matrix_X(3, 1) = e1plus->GetPosition3().x;
			matrix_X(3, 2) = e2minus->GetPosition3().x;
			matrix_X(3, 3) = p2->GetPosition3().x;
			/////////////////////////////////
			f0 = (u * f0plus->GetPosition3().y + v * f0minus->GetPosition3().y) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			f1 = ((1 - u) * f1minus->GetPosition3().y + v * f1plus->GetPosition3().y) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			f2 = ((1 - u) * f2plus->GetPosition3().y + (1 - v) * f2minus->GetPosition3().y) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			f3 = ((u)* f3minus->GetPosition3().y + (1 - v) * f3plus->GetPosition3().y) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;

			matrix_Y(0, 0) = p0->GetPosition3().y;
			matrix_Y(0, 1) = e0minus->GetPosition3().y;
			matrix_Y(0, 2) = e3plus->GetPosition3().y;
			matrix_Y(0, 3) = p3->GetPosition3().y;

			matrix_Y(1, 0) = e0plus->GetPosition3().y;
			matrix_Y(1, 1) = f0;
			matrix_Y(1, 2) = f3;
			matrix_Y(1, 3) = e3minus->GetPosition3().y;

			matrix_Y(2, 0) = e1minus->GetPosition3().y;
			matrix_Y(2, 1) = f1;
			matrix_Y(2, 2) = f2;
			matrix_Y(2, 3) = e2plus->GetPosition3().y;

			matrix_Y(3, 0) = p1->GetPosition3().y;
			matrix_Y(3, 1) = e1plus->GetPosition3().y;
			matrix_Y(3, 2) = e2minus->GetPosition3().y;
			matrix_Y(3, 3) = p2->GetPosition3().y;
			/////////////////////////////////
			f0 = (u * f0plus->GetPosition3().z + v * f0minus->GetPosition3().z) / (u + v);
			f0 = u + v == 0 ? 0 : f0;
			f1 = ((1 - u) * f1minus->GetPosition3().z + v * f1plus->GetPosition3().z) / (1 - u + v);
			f1 = 1 - u + v == 0 ? 0 : f1;
			f2 = ((1 - u) * f2plus->GetPosition3().z + (1 - v) * f2minus->GetPosition3().z) / (2 - u - v);
			f2 = 2 - u - v == 0 ? 0 : f2;
			f3 = ((u)* f3minus->GetPosition3().z + (1 - v) * f3plus->GetPosition3().z) / (1 + u - v);
			f3 = 1 + u - v == 0 ? 0 : f3;

			matrix_Z(0, 0) = p0->GetPosition3().z;
			matrix_Z(0, 1) = e0minus->GetPosition3().z;
			matrix_Z(0, 2) = e3plus->GetPosition3().z;
			matrix_Z(0, 3) = p3->GetPosition3().z;

			matrix_Z(1, 0) = e0plus->GetPosition3().z;
			matrix_Z(1, 1) = f0;
			matrix_Z(1, 2) = f3;
			matrix_Z(1, 3) = e3minus->GetPosition3().z;

			matrix_Z(2, 0) = e1minus->GetPosition3().z;
			matrix_Z(2, 1) = f1;
			matrix_Z(2, 2) = f2;
			matrix_Z(2, 3) = e2plus->GetPosition3().z;

			matrix_Z(3, 0) = p1->GetPosition3().z;
			matrix_Z(3, 1) = e1plus->GetPosition3().z;
			matrix_Z(3, 2) = e2minus->GetPosition3().z;
			matrix_Z(3, 3) = p2->GetPosition3().z;



			float resultX, resultY, resultZ;
			resultX = resultY = resultZ = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					resultX += matrix_X(i, j) * b_u[i] * b_v[j];
					resultY += matrix_Y(i, j) * b_u[i] * b_v[j];
					resultZ += matrix_Z(i, j) * b_u[i] * b_v[j];
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

void GregoryPatch::generateBezierNet(VertexPos* verticesContour)
{
	int internalNodesCounter = 0;

	verticesContour[internalNodesCounter++] = { p0->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e0plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e0plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e1minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e1minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { p1->GetPosition3() };

	verticesContour[internalNodesCounter++] = { e0minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f0minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f1plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e1plus->GetPosition3() };

	verticesContour[internalNodesCounter++] = { e3plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f3plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f2minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2minus->GetPosition3() };

	verticesContour[internalNodesCounter++] = { p3->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e3minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e3minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { p2->GetPosition3() };

	/////////////////
	verticesContour[internalNodesCounter++] = { p0->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e0minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e0minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e3plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e3plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { p3->GetPosition3() };

	verticesContour[internalNodesCounter++] = { e0plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f0plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f3minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e3minus->GetPosition3() };

	verticesContour[internalNodesCounter++] = { e1minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f1minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { f2plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2plus->GetPosition3() };

	verticesContour[internalNodesCounter++] = { p1->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e1plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e1plus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { e2minus->GetPosition3() };
	verticesContour[internalNodesCounter++] = { p2->GetPosition3() };

	//for (int i = 0; i < m_nodes.size(); i += 4)
	//{
	//	verticesContour[internalNodesCounter++] = { m_nodes[i]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 1]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 2]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 3]->GetPosition3() };
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 0]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 1]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 2]->GetPosition3() };
	//	verticesContour[internalNodesCounter++] = { m_nodes[i + 4 * 3]->GetPosition3() };
	//}
}


void GregoryPatch::Reset()
{
	//createDeBoorPointMatrix();
	list<VertexPos*> vertices = list<VertexPos*>();
	//we start from Second Point cause we want segments continuity C0
	int index = 0;
	int numberOfControlPoints = m_nodes.size() - 1;
	int segmentLength = 0;
	generateLines(vertices);

	m_vertexCountContour = 40;//(4 + 4 - 2) * 4 * 2; //48
	VertexPos* verticesContour;
	verticesContour = new VertexPos[m_vertexCountContour];

	generateBezierNet(verticesContour);

	m_vertexCount = vertices.size();
	VertexPos *arr = new VertexPos[m_vertexCount];
	int ind = 0;
	for (list<VertexPos*>::iterator it = vertices.begin(); it != vertices.end(); it++)
	{
		arr[ind++] = *(*it);
		delete *it;
	}
	m_vertexBuffer = m_device.CreateVertexBuffer(arr, m_vertexCount);
	m_vertexBufferContour = m_device.CreateVertexBuffer(verticesContour, m_vertexCountContour);
	delete[]arr;
	delete[]verticesContour;
	setPointTopology();
}

void GregoryPatch::setPointTopology()
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

void GregoryPatch::setTriangleTopology()
{

}
void GregoryPatch::setLineTopology()
{

}

void GregoryPatch::Draw()
{
	//Reset();
	GregoryPatchShader* shader = dynamic_cast<GregoryPatchShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(shader->getTopology());
	//if (m_input->isStereoscopyActive)
	//{
	//	m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
	//	m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
	//	m_context->DrawIndexed(m_indexCount, 0, 0);

	//	m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
	//	m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
	//	m_context->DrawIndexed(m_indexCount, 0, 0);

	//	if (m_input->isBezierPolygonActive)
	//	{
	//		m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
	//		b = m_vertexBufferContour.get();
	//		m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	//		m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
	//		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
	//		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
	//		m_context->DrawIndexed(m_indexCountContour, 0, 0);
	//		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
	//		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
	//		m_context->DrawIndexed(m_indexCountContour, 0, 0);
	//	}
	//}
	//else
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		if (m_input->isBezierPolygonActive)
		{
			m_context->IASetPrimitiveTopology(shader->m_NodesTopology);
			b = m_vertexBufferContour.get();
			m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
			m_context->IASetIndexBuffer(m_indexBufferContour.get(), DXGI_FORMAT_R16_UINT, 0);
			m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
			m_context->DrawIndexed(m_indexCountContour, 0, 0);
		}
	}
}