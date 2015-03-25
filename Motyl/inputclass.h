
#pragma once
#include "input.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////

enum ModelType{
	TorusType,
	ElipsoidType,
	SimplePointType,
	CursorType,
	Undecided
};

enum ActiveFeature{
	Translation,
	Rotation,
	Scale,
	None
};

enum ActiveAxis{
	AxisX,
	AxisY,
	AxisZ,
	AxisNone
};

enum ActiveRadius{
	BigRadius,
	SmallRadius
};

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	void SetActiveFeature(ActiveFeature);
	void SetActiveAxis(ActiveAxis);
	void SetActiveRadius(ActiveRadius);
	ActiveFeature getActiveFeature();
	ActiveAxis getActiveAxis();
	ActiveRadius getActiveRadius();

	bool IsKeyDown(unsigned int);

	void setStereorcopy(bool isActive);

	bool isStereoscopyActive;
	void AddModel(ModelType);
	void SetMousePosition(float x, float y);
	void SetMousePressing(bool isPressed);
	bool IsMouseButtonDown();
	POINT GetMousePosition();

private:
	POINT m_MousePosition;
	bool m_isMouseButtonDown = false;
	bool m_keys[256];
	ActiveFeature m_activeFeature;
	ActiveAxis m_activeAxis;
	ActiveRadius m_activeRadius;
};
