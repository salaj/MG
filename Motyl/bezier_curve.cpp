#include "bezier_curve.h"
#include "shader_simple_point.h"

using namespace gk2;

int BezierCurve::CurveCounter = 0;

BezierCurve::BezierCurve(std::shared_ptr<ID3D11DeviceContext> deviceContext,
	ShaderBase* shader_base,
	gk2::DeviceHelper device,
	gk2::Camera camera,
	InputClass* input) : ModelClass(deviceContext, shader_base, device, camera, input)
{
	ModelClass::Initialize();
}

BezierCurve::BezierCurve(Service& service) : ModelClass(service)
{
	m_Type = ModelType::SimplePointType;
	ModelClass::Initialize();
}

BezierCurve::BezierCurve(const BezierCurve& other) : ModelClass(other)
{
}


BezierCurve::~BezierCurve()
{
}

BezierCurve::BezierCurve()
{
}

void* BezierCurve::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}


void BezierCurve::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

void BezierCurve::SetNodes(vector<SimplePoint*> nodes)
{
	m_nodes = nodes;
}

void BezierCurve::InsertNodeAfter(SimplePoint* node, SimplePoint* after)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->m_id == after->m_id)
			m_nodes.insert(m_nodes.begin() + i + 1, node);
	}
}

void BezierCurve::RemoveNode(SimplePoint* node)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->m_id == node->m_id)
			m_nodes.erase(m_nodes.begin() + i);
	}
}

void BezierCurve::InsertNodeAt(SimplePoint* node, int position)
{
	m_nodes.insert(m_nodes.begin() + position, node);
}

void BezierCurve::RemoveNodeAt(int position)
{
	m_nodes.erase(m_nodes.begin() + position);
}


double BezierCurve::bezier_length()
{
	double t;
	int i;
	int steps = 10;
	int estimation = 2;
	int screenWidth = 300;
	XMFLOAT3 dot;
	XMFLOAT3 previous_dot;
	double length = 0.0;
	for (i = 0; i <= steps; i++) {
		t = (double)i / (double)steps;
		dot = calculateInPowerBase(t);
		if (i > 0) {
			double x_diff = dot.x - previous_dot.x;
			double y_diff = dot.y - previous_dot.y;
			length += sqrt(x_diff * x_diff + y_diff * y_diff);
		}
		previous_dot = dot;
	}
	return length * screenWidth * estimation;
}

XMFLOAT3 BezierCurve::calculateInPowerBase(float t)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	XMVECTOR result = uuu * A; //first term in equation
	result += 3 * uu * t * B;  //second term in equation
	result += 3 * u * tt * C; //third term in equation
	result += ttt * D;

	return XMFLOAT3(
		XMVectorGetX(result),
		XMVectorGetY(result),
		XMVectorGetZ(result)
		);
}

void BezierCurve::Reset()
{
	Initialize();
}

int BezierCurve::CheckIfNodesHaveChanged(vector<ModelClass*>& models)
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		for (int j = 0; j < models.size(); j++)
		{
			if (models[j]->m_id == m_nodes[i]->m_id)
				return models[j]->m_id;
		}
	}
	return -1;
}


void BezierCurve::Initialize()
{
	A = XMLoadFloat4(&m_nodes[0]->GetPosition());
	B = XMLoadFloat4(&m_nodes[1]->GetPosition());
	C = XMLoadFloat4(&m_nodes[2]->GetPosition());
	D = XMLoadFloat4(&m_nodes[3]->GetPosition());

	// Set the number of vertices in the vertex array.
	m_vertexCount = bezier_length();// 20;//m_nodes.size();

	VertexPosNormal* vertices = new VertexPosNormal[m_vertexCount];

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i] = {
			calculateInPowerBase((float)i / (float)(m_vertexCount-1)),
			XMFLOAT3(0.0f, 0.0f, 1.0f) 
		};

	}

	m_vertexBuffer = m_device.CreateVertexBuffer(vertices, m_vertexCount);
	delete vertices;

	//setTriangleTopology();
	//setLineTopology();
	setPointTopology();
}

void BezierCurve::setPointTopology()
{
	m_indexCount = m_vertexCount;
	unsigned short* indices = new unsigned short[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
	delete indices;
}

void BezierCurve::setLineTopology()
{

	// Set the number of indices in the index array.
	m_indexCount = 38;

	unsigned short* indices = new unsigned short[m_indexCount];
	indices[0] = 0;
	for (int i = 1; i <= 18; i++)
	{
		indices[2 * i] = i;
		indices[2 * i + 1] = i;
	}
	indices[37] = 19;
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
	delete indices;
}

void BezierCurve::setTriangleTopology()
{
	// Set the number of indices in the index array.
	m_indexCount = 36;

	unsigned short indices[] =
	{
		0, 1, 2, 0, 2, 3,		//Front face
		4, 5, 6, 4, 6, 7,		//Left face
		8, 9, 10, 8, 10, 11,	//Botton face
		12, 13, 14, 12, 14, 15,	//Back face
		16, 17, 18, 16, 18, 19,	//Right face
		20, 21, 22, 20, 22, 23	//Top face
	};
	m_indexBuffer = m_device.CreateIndexBuffer(indices, m_indexCount);
}

void BezierCurve::setStereoscopy(bool isStereoscopic)
{
	m_isStereoscopic = isStereoscopic;
}


void BezierCurve::Draw()
{
	BezierCurveShader* shader = dynamic_cast<BezierCurveShader*>(m_shader_base);
	m_context->UpdateSubresource(shader->GetCBWorldMatrix().get(), 0, 0, &m_modelMatrix, 0, 0);

	shader->SetContent();

	ID3D11Buffer* b = m_vertexBuffer.get();
	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);

	if (m_input->isStereoscopyActive)
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixLeft(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);

		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
	else
	{
		m_context->UpdateSubresource(shader->GetCBProjMatrix().get(), 0, 0, &createStereoscopicProjMatrixRight(), 0, 0);
		m_context->UpdateSubresource(shader->GetCBColor().get(), 0, 0, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
		m_context->DrawIndexed(m_indexCount, 0, 0);
	}

}