////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////

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

private:
	bool m_keys[256];
	ActiveFeature m_activeFeature;
	ActiveAxis m_activeAxis;
	ActiveRadius m_activeRadius;
};

#endif