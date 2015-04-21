////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

using namespace gk2;
using namespace std;

InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

ActiveFeature InputClass::getActiveFeature()
{
	return m_activeFeature;
}

ActiveAxis InputClass::getActiveAxis()
{
	return m_activeAxis;
}

ActiveRadius InputClass::getActiveRadius()
{
	return m_activeRadius;
}

void InputClass::SetActiveFeature(ActiveFeature feature)
{
	m_activeFeature = feature;
}

void InputClass::SetActiveAxis(ActiveAxis axis)
{
	m_activeAxis = axis;
}

void InputClass::SetActiveRadius(ActiveRadius radius)
{
	m_activeRadius = radius;
}


void InputClass::Initialize()
{
	int i;
	m_activeFeature = None;
	m_activeAxis = AxisNone;

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

void InputClass::setStereorcopy(bool isActive)
{
	isStereoscopyActive = isActive;
}

void InputClass::setBezierPolygon(bool isActive)
{
	isBezierPolygonActive = isActive;
}

void InputClass::setBase(bool _isBernsteinBase)
{
	isBernsteinBaseSet = _isBernsteinBase;
}

void InputClass::AddModel(ModelType type)
{
	
}

void InputClass::SetMousePosition(float x, float y)
{
	m_MousePosition.x = x;
	m_MousePosition.y = y;
}

void InputClass::SetMousePressing(bool isPressed)
{
	m_isMouseButtonDown = isPressed;
}

bool InputClass::IsMouseButtonDown()
{
	return m_isMouseButtonDown;
}

POINT InputClass::GetMousePosition()
{
	return m_MousePosition;
}

void InputClass::SetSelectedModel(int id)
{
	m_selectedTreeViewItem = id;
	if (m_selectedTreeViewItem == m_previousSelectedTreeViewItem)
	{
		change = false;
		return;
	}
	m_previousSelectedTreeViewItem = m_selectedTreeViewItem;
	change = true;	//we want to notify
}

int InputClass:: GetSelectedModel()
{
	if (change)
	{
		change = false;
		return m_selectedTreeViewItem;
	}
	return -1;
}
