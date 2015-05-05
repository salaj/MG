
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
	BezierType,
	BezierC2Type,
	InterpolatedC2Type,
	BezierPatchType,
	BezierSurfaceType,
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
	void setBezierPolygon(bool isActive);
	void setBase(bool isBezierBase);

	bool isStereoscopyActive = true;
	bool isBezierPolygonActive = true;
	bool isBernsteinBaseSet = true;

	void AddModel(ModelType);
	void SetMousePosition(float x, float y);
	void SetMousePressing(bool isPressed);
	bool IsMouseButtonDown();
	POINT GetMousePosition();
	int GetSelectedModel();
	int GetVerticalSpaces();
	int GetHorizontalSpaces();
	void SetSelectedModel(int id);
	void SetVerticalSpaces(int verticalSpaces);
	void SetHorizontalSpaces(int horizontalSpaces);

private:
	POINT m_MousePosition;
	bool m_isMouseButtonDown = false;
	bool m_keys[256];
	ActiveFeature m_activeFeature;
	ActiveAxis m_activeAxis;
	ActiveRadius m_activeRadius;

	int m_selectedTreeViewItem;
	int m_previousSelectedTreeViewItem;
	bool change = false;
	int m_verticalSpaces = 4;
	int m_horizontalSpaces = 4;
};
