////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "guiUpdater.h"


GUIUpdater::GUIUpdater()
{
}


GUIUpdater::GUIUpdater(Settings* settings)
{
	m_settings = settings;
}

GUIUpdater::GUIUpdater(const GUIUpdater& other)
{
}


GUIUpdater::~GUIUpdater()
{
}

void GUIUpdater::AddModel(XMFLOAT4 position)
{
	m_settings->insertItemExternally(TEXT("Point"));
}

void GUIUpdater::SetCursorPosition(XMFLOAT4 position)
{
	m_settings->setCursorPosition(position);
}
