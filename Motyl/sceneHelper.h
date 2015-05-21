#pragma once

#include "applicationBase.h"
#include "camera.h"
#include "inputclass.h"
#include "service.h"
#include "modelclass.h"
#include "modelsManager.h"
#include "guiUpdater.h"
#include <vector>
#include <xnamath.h>
#include <time.h>

using namespace std;

class SceneHelper
{
	public:
		SceneHelper();
		virtual ~SceneHelper();

		void Initialize(Service& service, GUIUpdater*);
		static void* operator new(size_t size);
		static void operator delete(void* ptr);

		//for injection
		ModelsManager* GetModelsManager();

		//for checking input - shall be new class 
		void CheckInput();
		void CheckMouse();
		//void CheckSelectedByTreeView();
		void IsBaseChanged();
		void RefreshSpaces();

		//for modelmanager class
		map<int, ModelClass*>& GetModels();
		void CreateModels();
		void InitializeModels();
		void DrawModels();
		void AddModel(ModelType type);

		void findClosestWithMouse(XMVECTOR orig, XMVECTOR dir);

	private:
		InputClass*  m_InputClass;
		GUIUpdater* m_GUIUpdater;
		ModelsManager m_modelsManager;
		ModelClass* m_activeVirtualModel = nullptr;

		POINT m_previousPoint;

		bool m_CanAdd = true;
		bool m_CanSelect = true;
		CurveBase m_base = CurveBase::Bezier;

		void translateModels(vector<ModelClass*>& models, XMFLOAT4 offset, bool forceRefresh);
		void translatePostActions(vector<ModelClass*>& models);
		void scaleModels(vector<ModelClass*>& models, float scale);
		void rotateModels(vector<ModelClass*>& models, float rotation, ActiveAxis axis);
		void redrawCurves();

		//ModelClass* selected;

		void selectNewAndDeselectOldModel(ModelClass* model);
		void deselectCurrentModel();

		void findClosestModelWithCursor();
		void findClosestModelWithMouse(POINT mousePosition);
		ModelClass* findMirroredModel(map<int, ModelClass*>& models, ModelClass* relativeTo);
		ModelClass* mirroredModel = nullptr;

		double refreshRate = 0.1f;
		double refreshRateLong = 1.0f;
		double deltaTime;
		double lastTime;
		bool shallBeRefreshed = false;
		bool shallBeRefreshedLong = false;
		void refreshTime();
		void toggleRotation(bool isRotationUp);
		void toggleTranslation(bool isTranslationUp);
		bool isTranslationPending = false;
		bool isTranslationUp;
		bool isRotationPending = false;
		bool isRotationUp;
};

