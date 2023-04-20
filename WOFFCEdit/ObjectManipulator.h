#pragma once
#include <vector>
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "SceneObject.h"
#include "pch.h"
#include "InputCommands.h"
#include "StepTimer.h"
#include "Camera.h"

enum class ManipulationMode {
	TRANSLATE,
	ROTATE,
	SCALE
};

struct Triangle
{
	DirectX::SimpleMath::Vector3 vertex0;
	DirectX::SimpleMath::Vector3 vertex1;
	DirectX::SimpleMath::Vector3 vertex2;
};

class ObjectManipulator
{
public:
	ObjectManipulator();
	~ObjectManipulator();

	void SetMode(ManipulationMode mode) { manipulationMode = mode; };
	ManipulationMode GetMode() { return manipulationMode; };
	void SetObject(DisplayObject* obj) { object = obj; };
	DisplayObject* GetObject() { return object; };
	void Update(DX::StepTimer const& timer, InputCommands* input, Camera* camera);
	bool GetActive() { return isManipulating; };
	void SetSceneGraph(std::vector<SceneObject>* sceneGraph, int* sel) { m_sceneGraph = sceneGraph; m_currentSelection = sel; };
	void SetObjectPosition(float x, float y, float z);
	void SetObjectRotation(float x, float y, float z);
	void SetObjectScale(float x, float y, float z);
	void SnapToGround(DisplayChunk* terrain);
	bool RayIntersectsTriangle(DirectX::SimpleMath::Vector3 rayOrigin, DirectX::SimpleMath::Vector3 rayVector, Triangle* inTriangle, DirectX::SimpleMath::Vector3& outIntersectionPoint);
	void CreateTriangles(DisplayChunk* terrain);

	bool isManipulating;

	
	//void SetObjects(std::vector<DisplayObject> objs) { objects = &objs; };
private:
	ManipulationMode manipulationMode;
	DisplayObject* object;
	//std::vector<DisplayObject*> objects;
	std::vector<Triangle> triangles;

	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;

	int clickX;
	int clickY;

	float movementRate;
	float rotationRate;
	float scaleRate;
};

