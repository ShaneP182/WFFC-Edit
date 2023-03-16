#pragma once
#include <vector>
#include "DisplayObject.h"
#include "pch.h"
#include "InputCommands.h"
#include "StepTimer.h"
#include "Camera.h"

enum class ManipulationMode {
	TRANSLATE,
	ROTATE,
	SCALE
};

class ObjectManipulator
{
public:
	ObjectManipulator();
	~ObjectManipulator();

	void SetMode(ManipulationMode mode) { manipulationMode = mode; };
	void SetObject(DisplayObject* obj) { object = obj; };
	void Update(DX::StepTimer const& timer, InputCommands* input, Camera* camera);
	bool GetActive() { return isManipulating; };
	//void SetObjects(std::vector<DisplayObject> objs) { objects = &objs; };
private:
	ManipulationMode manipulationMode;
	DisplayObject* object;
	//std::vector<DisplayObject*> objects;
	bool isManipulating;

	int clickX;
	int clickY;

	float movementRate;
	float rotationRate;
	float scaleRate;
};

