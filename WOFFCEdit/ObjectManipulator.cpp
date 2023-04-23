#pragma once
#include "ObjectManipulator.h"



ObjectManipulator::ObjectManipulator()
{
	object = NULL;
	manipulationMode = ManipulationMode::TRANSLATE;
	movementRate = 1.0f;
	rotationRate = 5.0f;
	scaleRate = 0.2f;
}

ObjectManipulator::~ObjectManipulator()
{
}

void ObjectManipulator::Update(DX::StepTimer const& timer, InputCommands* input, Camera* camera)
{
	if (input->LMBDown)
	{
		if (object)
		{
			if (!isManipulating)
			{
				if (GetParent(GetActiveWindow()) == 0) // if clicking the main window, manipulate
				{
					isManipulating = true;
					clickX = input->mouseX;
					clickY = input->mouseY;
				}
			}
		}
		else
		{
			isManipulating = false;
		}
	}
	else
	{
		//object = NULL;
		isManipulating = false;
	}

	if (isManipulating && object)
	{
		float distanceX;
		float distanceY;

		distanceX = input->mouseX - clickX;
		distanceY = input->mouseY - clickY;

		DirectX::SimpleMath::Vector3 forward = camera->GetForward();
		forward.y = 0; // remove yaw of forward, up-down movement is done separately

		DirectX::SimpleMath::Vector3 right = camera->GetRight();
		DirectX::SimpleMath::Vector3(1);

		switch (manipulationMode)
		{
		default:
		case ManipulationMode::TRANSLATE:
			if (input->shift)
			{
				if (!object->m_snap_to_ground)
				{
					object->m_position.y -= distanceY * timer.GetElapsedSeconds() * movementRate;
				}
			}
			else
			{
				object->m_position += right * distanceX * timer.GetElapsedSeconds() * movementRate;
				object->m_position -= forward * distanceY * timer.GetElapsedSeconds() * movementRate;
			}

			m_sceneGraph->at(*m_currentSelection).posX = object->m_position.x;
			m_sceneGraph->at(*m_currentSelection).posY = object->m_position.y;
			m_sceneGraph->at(*m_currentSelection).posZ = object->m_position.z;

			break;
		case ManipulationMode::ROTATE:
			if (input->shift)
			{
				object->m_orientation.x += distanceX * timer.GetElapsedSeconds() * rotationRate;
				object->m_orientation.z -= distanceY * timer.GetElapsedSeconds() * rotationRate;
			}
			else
			{
				object->m_orientation.y += distanceX * timer.GetElapsedSeconds() * rotationRate;
			}

			m_sceneGraph->at(*m_currentSelection).rotX = object->m_orientation.x;
			m_sceneGraph->at(*m_currentSelection).rotY = object->m_orientation.y;
			m_sceneGraph->at(*m_currentSelection).rotZ = object->m_orientation.z;
			break;
		case ManipulationMode::SCALE:
			object->m_scale -= DirectX::SimpleMath::Vector3(1) * distanceY * timer.GetElapsedSeconds() * scaleRate;

			m_sceneGraph->at(*m_currentSelection).scaX = object->m_scale.x;
			m_sceneGraph->at(*m_currentSelection).scaY = object->m_scale.y;
			m_sceneGraph->at(*m_currentSelection).scaZ = object->m_scale.z;
			// limit scaling so it can't go below 0 here
			break;
		}

		//m_camOrientation.y += distanceX * timer.GetElapsedSeconds() * m_camRotRate;
		//m_camOrientation.z -= distanceY * timer.GetElapsedSeconds() * m_camRotRate;

		SetCursorPos(clickX, clickY);
	}
}

void ObjectManipulator::SetObjectPosition(float x, float y, float z)
{
	DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(x, y, z);

	if (object)
	{
		object->m_position = pos;
	}
}

void ObjectManipulator::SetObjectRotation(float x, float y, float z)
{
	DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3(x, y, z);

	if (object)
	{
		object->m_orientation = rot;
	}
}

void ObjectManipulator::SetObjectScale(float x, float y, float z)
{
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(x, y, z);

	if (object)
	{
		object->m_scale = scale;
	}
}

void ObjectManipulator::CreateTriangles(DisplayChunk* terrain)
{
	triangles.clear();
	// in report, use diagram for demonstrating quad labelled with i and j showing triangles

	for (int i = 0; i < TERRAINRESOLUTION - 1; i++)
	{
		for (int j = 0; j < TERRAINRESOLUTION - 1; j++)
		{
			DirectX::SimpleMath::Vector3 vertex0 = terrain->m_terrainGeometry[i][j].position;
			DirectX::SimpleMath::Vector3 vertex1 = terrain->m_terrainGeometry[i + 1][j].position;
			DirectX::SimpleMath::Vector3 vertex2 = terrain->m_terrainGeometry[i][j + 1].position;
			DirectX::SimpleMath::Vector3 vertex3 = terrain->m_terrainGeometry[i + 1][j + 1].position;

			Triangle triangle0, triangle1;

			triangle0.vertex0 = vertex0;
			triangle0.vertex1 = vertex1;
			triangle0.vertex2 = vertex3;
			triangles.push_back(triangle0);

			triangle1.vertex0 = vertex0;
			triangle1.vertex1 = vertex2;
			triangle1.vertex2 = vertex3;
			triangles.push_back(triangle1);
		}
	}
}

void ObjectManipulator::SnapToGround(DisplayChunk* terrain)
{
	// set object's y position to height of terrain at its x and z co-ordinates
	if (object)
	{
		if (object->m_snap_to_ground) // optimise - only do when changing position, don't loop through every vertice every time
		{
			float traceLength = 10000;
			DirectX::SimpleMath::Vector3 rayOrigin = object->m_position;
			rayOrigin.y += traceLength / 2; // offset by half of trace length so ray traces up and down
			DirectX::SimpleMath::Vector3 rayVector = DirectX::SimpleMath::Vector3(0, -traceLength, 0);

			
			CreateTriangles(terrain); // change to only run this when terrain is updated

			for (Triangle triangle : triangles)
			{
				DirectX::SimpleMath::Vector3 intersectionPoint;

				if (RayIntersectsTriangle(rayOrigin, rayVector, &triangle, intersectionPoint))
				{
					object->m_position.y = intersectionPoint.y;
					m_sceneGraph->at(*m_currentSelection).posY = object->m_position.y;

					// could also be improved to update object rotation based on surface normal. Would require reworking how object rotation is handled.
					break;
				}

				// remember last triangle, work through adjacent triangles first?
			}
			
			
		}
	}
}

// Implementation of Möller–Trumbore intersection algorithm, source: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Adapted for use in DirectX
bool ObjectManipulator::RayIntersectsTriangle(DirectX::SimpleMath::Vector3 rayOrigin, DirectX::SimpleMath::Vector3 rayVector, Triangle* inTriangle, DirectX::SimpleMath::Vector3& outIntersectionPoint)
{
	const float EPSILON = 0.0000001;
	DirectX::SimpleMath::Vector3 vertex0 = inTriangle->vertex0;
	DirectX::SimpleMath::Vector3 vertex1 = inTriangle->vertex1;
	DirectX::SimpleMath::Vector3 vertex2 = inTriangle->vertex2;
	DirectX::SimpleMath::Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = rayVector.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.

	f = 1.0 / a;
	s = rayOrigin - vertex0;
	u = f * s.Dot(h);

	if (u < 0.0 || u > 1.0)
		return false;

	q = s.Cross(edge1);
	v = f * rayVector.Dot(q);

	if (v < 0.0 || u + v > 1.0)
		return false;

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * edge2.Dot(q);

	if (t > EPSILON) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}






