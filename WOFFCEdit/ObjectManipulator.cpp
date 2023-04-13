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

	if (isManipulating)
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
				object->m_position.y -= distanceY * timer.GetElapsedSeconds() * movementRate;
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
