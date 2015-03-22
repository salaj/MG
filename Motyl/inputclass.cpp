////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


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