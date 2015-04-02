#pragma once

#include "applicationBase.h"
#include "scene_service.h"
#include "camera.h"
#include "inputclass.h"
#include "sceneHelper.h"
#include <xnamath.h>
#include "shader_torus.h"
#include "shader_elipsoid.h"
#include "shader_simple_point.h"
#include "shader_cursor.h"
#include "shader_bezier_curve.h"
#include "torus.h"
#include "elipsoid.h"

namespace gk2
{
	class Scene : public gk2::ApplicationBase
	{
	public:
		Scene(HINSTANCE hInstance, InputClass* inputClas, GUIUpdater* m_GUIUpdater);
		virtual ~Scene();

		//Niektóre struktury xnamath.h wymagaj¹ (np. XMMATRIX czy XMVECTOR) wymagaj¹, aby adresy zmiennych
		//tych typów by³y wyrównane do 16 bajtów. W przypadku dynamicznej alokacji operator new zapewnia tylko
		//wyrównanie do 8 bajtów. Dlatego nale¿y przeci¹¿yæ operator new dla klas, które zawieraj¹
		//pola jednego z tych typów, aby zapewniæ odpowiednie wyrówanie po³o¿enia obiektu w pamiêci.
		static void* operator new(size_t size);
		static void operator delete(void* ptr);


		//for injection
		SceneService* GetSceneService();

	protected:
		virtual bool LoadContent();
		virtual void UnloadContent();

		virtual void Update(float dt);
		virtual void Render();
	private:


		//Various D3D constants
		static const unsigned int VB_STRIDE;
		static const unsigned int VB_OFFSET;
		static const unsigned int BS_MASK;

		//Table of colors for dodecahedron's faces
		static const XMFLOAT4 COLORS[12];

		//Projection matrix
		XMMATRIX m_projMtx;
	
		//Elipsoid afinity matrix
		XMMATRIX m_elipsoidModel = XMMatrixIdentity();


		//Camera helper
		Camera m_camera;

		//void DrawButterfly();

		//Depth stencil state used to fill the stencil buffer
		std::shared_ptr<ID3D11DepthStencilState> m_dssWrite;
		//Depth stencil state used to perform stencil test when drawing mirrored scene
		std::shared_ptr<ID3D11DepthStencilState> m_dssTest;
		//Rasterizer state used to define front faces as counter-clockwise, used when drawing mirrored scene
		std::shared_ptr<ID3D11RasterizerState> m_rsCounterClockwise;
		//Blend state used to draw dodecahedron faced with alpha blending.
		std::shared_ptr<ID3D11BlendState> m_bsAlpha;

		//Shader's constant buffer containing Local -> World matrix
		std::shared_ptr<ID3D11Buffer> m_cbWorld;
		//Shader's constant buffer containing World -> Camera matrix
		std::shared_ptr<ID3D11Buffer> m_cbView;
		//Shader's constant buffer containing projection matrix
		std::shared_ptr<ID3D11Buffer> m_cbProj;
		//Shader's constant buffer containing light's positions
		std::shared_ptr<ID3D11Buffer> m_cbLightPos;
		//Shader's constant buffer containting lighting description
		std::shared_ptr<ID3D11Buffer> m_cbLightColors;
		//Shader's constant buffer containting surface color
		std::shared_ptr<ID3D11Buffer> m_cbSurfaceColor;





		// Return the position of point on the Moebius strip for parameters t and s
		static XMFLOAT3 MoebiusStripPos(float t, float s);
		// Return the t-derivative of point on the Moebius strip for parameters t and s
		static XMVECTOR MoebiusStripDt(float t, float s);
		// Return the s-derivative of point on the Moebius strip for parameters t and s
		static XMVECTOR MoebiusStripDs(float t, float s);

		//Initializes all render states
		void InitializeRenderStates();
		//Initializes camera
		void InitializeCamera();

		//Updates camera-related constant buffers
		void UpdateCamera(XMMATRIX &viewMtx);
		void UpdateCamera();

		/////////////////////

		TorusShader* m_shader_torus;
		ElipsoidShader* m_shader_elipsoid;
		SimplePointShader* m_shader_simple_point;
		CursorShader* m_shader_cursor;
		BezierCurveShader* m_shader_bezier_curve;

		InputClass* m_input_class;
		GUIUpdater* m_GUIUpdater;

		//Torus* m_Torus;
		//Torus* m_Torus2;
		//Elipsoid* m_Elipsoid;

		SceneHelper m_sceneHelper;
		SceneService m_SceneService;
	};
}
