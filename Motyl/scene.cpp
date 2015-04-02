#include "scene.h"
#include "utils.h"
#include "vertices.h"
#include "window.h"

using namespace std;
using namespace gk2;
//

const unsigned int Scene::BS_MASK = 0xffffffff;

const unsigned int Scene::VB_STRIDE = sizeof(VertexPosNormal);
const unsigned int Scene::VB_OFFSET = 0;

void* Scene::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void Scene::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}

Scene::Scene(HINSTANCE hInstance, InputClass* inputClass, GUIUpdater* guiUpdater)
	: ApplicationBase(hInstance),
	m_camera(0.01f, 100.0f),
	m_input_class(inputClass),
	m_GUIUpdater(guiUpdater)
{
}

Scene::~Scene()
{
	delete m_shader_torus;
	delete m_shader_elipsoid;
	delete m_shader_simple_point;
}

void Scene::InitializeRenderStates()
//Setup render states used in various stages of the scene rendering
{
	D3D11_DEPTH_STENCIL_DESC dssDesc = m_device.DefaultDepthStencilDesc();
	//Setup depth stencil state for writing
	dssDesc.DepthEnable = true;
	dssDesc.StencilEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //wy³¹czamy test bufora g³êbokoœci

	D3D11_BLEND_DESC bsDesc = m_device.DefaultBlendDesc();
	bsDesc.RenderTarget[0].BlendEnable = true;
	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR; 
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//(+)

	//Setup color blending
	m_bsAlpha = m_device.CreateBlendState(bsDesc);

	m_dssWrite = m_device.CreateDepthStencilState(dssDesc);

}

void Scene::InitializeCamera()
{
	SIZE s = getMainWindow()->getClientSize();
	float ar = static_cast<float>(s.cx) / s.cy;
	//m_projMtx = XMMatrixPerspectiveFovLH(XM_PIDIV4, ar, 0.01f, 100.0f);
	m_projMtx = XMMatrixOrthographicLH(s.cx, s.cy, 0.01f, 100.0f);
	m_context->UpdateSubresource(m_shader_torus->GetCBProjMatrix().get(), 0, 0, &m_projMtx, 0, 0);
	//m_camera.Zoom(5);
	UpdateCamera();
}

bool Scene::LoadContent()
{
	m_shader_torus = new TorusShader(m_context, m_device, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_shader_elipsoid = new ElipsoidShader(m_context, m_device, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_shader_bezier_curve = new BezierCurveShader(m_context, m_device, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_shader_simple_point = new SimplePointShader(m_context, m_device, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_shader_cursor = new CursorShader(m_context, m_device, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	Service service;
	service.Context = m_context;
	service.Camera = m_camera;
	service.Device = m_device;
	service.Mouse = m_mouse;
	service.InputClass = m_input_class;
	service.Shader = new ShaderBase*[5]
	{
		m_shader_torus,
		m_shader_elipsoid,
		m_shader_simple_point,
		m_shader_cursor,
		m_shader_bezier_curve
	};
	m_sceneHelper.Initialize(service, m_GUIUpdater);


	m_sceneHelper.CreateModels();

	//m_Torus = new Torus(m_context, m_shader_torus, m_device, m_camera);
	//m_Torus2 = new Torus(m_context, m_shader_torus, m_device, m_camera);
	//m_Elipsoid = new Elipsoid(m_context, m_shader_elipsoid, m_device, m_camera);

	m_shader_torus->LoadContent();
	m_shader_elipsoid->LoadContent();
	m_shader_simple_point->LoadContent();
	m_shader_cursor->LoadContent();
	m_shader_bezier_curve->LoadContent();

	InitializeRenderStates();
	InitializeCamera();

	//m_Torus -> Initialize();
	//m_Torus2 -> Initialize();
	//m_Elipsoid -> Initialize();
	//m_sceneHelper.InitializeModels();
	return true;
}

void Scene::UnloadContent()
{
	m_shader_torus->UnloadContent();
	m_shader_elipsoid->UnloadContent();

	m_dssWrite.reset();
	m_dssTest.reset();
	m_rsCounterClockwise.reset();
	m_bsAlpha.reset();


	m_cbWorld.reset();
	m_cbView.reset();
	m_cbProj.reset();
	m_cbLightPos.reset();
	m_cbLightColors.reset();
	m_cbSurfaceColor.reset();
}

void Scene::UpdateCamera()
{
	XMMATRIX viewMtx;
	m_camera.GetViewMatrix(viewMtx);
	m_context->UpdateSubresource(m_shader_torus->GetCBViewMatrix().get(), 0, 0, &viewMtx, 0, 0);
}

void Scene::UpdateCamera(XMMATRIX& viewMtx)
{
	m_context->UpdateSubresource(m_shader_torus->GetCBViewMatrix().get(), 0, 0, &viewMtx, 0, 0);
}


int counter = 0;

void Scene::Update(float dt)
{
	//static MouseState prevState;
	//MouseState currentState;
	//if (!m_mouse->GetState(currentState))
	//	return;
	//bool change = true;
	//if (prevState.isButtonDown(0))
	//{
	//	POINT d = currentState.getMousePositionChange();
	//	m_camera.Rotate(d.y / 300.f, d.x / 300.f);
	//}
	//else if (prevState.isButtonDown(1))
	//{
	//	POINT d = currentState.getMousePositionChange();
	//	m_camera.Zoom(d.y / 10.0f);
	//}
	//else
	//	change = false;
	//prevState = currentState;
	//if (change)
	//	UpdateCamera();

	m_sceneHelper.CheckMouse();
	m_sceneHelper.CheckInput();
	//m_sceneHelper.CheckSelectedByTreeView();
}

SceneService* Scene::GetSceneService()
{
	m_SceneService.ModelsManager = m_sceneHelper.GetModelsManager();
	return &m_SceneService;
}
//void Butterfly::DrawButterfly()
////Draw the butterfly
//{
//	m_context->UpdateSubresource(m_cbWorld.get(), 0, 0, &m_elipsoidModel, 0, 0);
//	ID3D11Buffer* b = m_vbWing.get();
//	m_context->IASetVertexBuffers(0, 1, &b, &VB_STRIDE, &VB_OFFSET);
//	m_context->Draw(m_vertexCount, 0);
//}

void Scene::Render()
{
	if (m_context == nullptr)
		return;
	//Clear buffers
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_backBuffer.get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	//m_context->UpdateSubresource(m_.get(), 0, 0, &m_elipsoidModel, 0, 0);

	//m_shader_torus->SetContent();

	m_context->OMSetDepthStencilState(m_dssWrite.get(), 0);
	m_context->OMSetBlendState(m_bsAlpha.get(), 0, BS_MASK);

	m_sceneHelper.DrawModels();


	m_context->RSSetState(NULL);
	m_context->OMSetDepthStencilState(NULL, 0);


	//rysowanie elipsoidy
	//m_shader_elipsoid->SetContent();
	//m_Elipsoid->Draw(true, false);

	m_swapChain->Present(0, 0);
}