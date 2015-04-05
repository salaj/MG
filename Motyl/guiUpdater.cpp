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
	m_settings->insertItemExternally();
}

void GUIUpdater::setCursorPositionWorld(XMFLOAT4 position)
{
	m_settings->setCursorPositionWorld(position);
}

void GUIUpdater::setCursorPositionScreen(XMFLOAT4 position)
{
	m_settings->setCursorPositionScreen(position);
}