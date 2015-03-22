#pragma once

#include "applicationBase.h"
#include "camera.h"
#include "inputclass.h"
#include "service.h"
#include "modelclass.h"
#include "modelsManager.h"
#include <vector>
#include <xnamath.h>

using namespace std;

class SceneHelper
{
	public:
		SceneHelper();
		virtual ~SceneHelper();

		void Initialize(Service& service);
		static void* operator new(size_t size);
		static void operator delete(void* ptr);

		//for checking input - shall be new class 
		void CheckInput();

		//for modelmanager class
		vector<ModelClass*>& GetModels();
		void CreateModels();
		void InitializeModels();
		void DrawModels();


	private:
		InputClass*  m_InputClass;
		ModelsManager m_modelsManager;

		void translateModels(vector<ModelClass*>& models, XMFLOAT3 offset);

		void scaleModels(vector<ModelClass*>& models, float scale);

		void rotateModels(vector<ModelClass*>& models, float rotation, ActiveAxis axis);
};

