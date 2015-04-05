
#pragma once
#include "settings.h"
#include "xnamath.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: GUIUpdater
////////////////////////////////////////////////////////////////////////////////

//enum ModelType{
//	TorusType,
//	ElipsoidType,
//	SimplePointType
//};
using namespace gk2;

class GUIUpdater
{
public:
	GUIUpdater();
	GUIUpdater(const GUIUpdater&);
	GUIUpdater(Settings* settings);
	~GUIUpdater();
	void Initialize();
	void AddModel(XMFLOAT4 position);
	void setCursorPositionWorld(XMFLOAT4 position);
	void setCursorPositionScreen(XMFLOAT4 position);

private:
	Settings* m_settings;
};
