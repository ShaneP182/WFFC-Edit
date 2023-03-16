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
				isManipulating = true;
				clickX = input->mouseX;
				clickY = input->mouseY;
			}
		}
		else
		{
			isManipulating = false;
		}
	}
	else
	{
		object = NULL;
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
			break;
		case ManipulationMode::SCALE:
			object->m_scale -= DirectX::SimpleMath::Vector3(1) * distanceY * timer.GetElapsedSeconds() * scaleRate;
			break;
		}

		//m_camOrientation.y += distanceX * timer.GetElapsedSeconds() * m_camRotRate;
		//m_camOrientation.z -= distanceY * timer.GetElapsedSeconds() * m_camRotRate;

		SetCursorPos(clickX, clickY);
	
	}
}
