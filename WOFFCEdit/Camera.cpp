#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	//functional
	m_camMoveSpeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	// Up vector is fixed regardless of orientation
	m_camUp.x = 0.0f;
	m_camUp.y = 1.0f;
	m_camUp.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;

	mouseMode = false;
}

Camera::~Camera()
{

}

void Camera::Update(DX::StepTimer const& timer, InputCommands* input)
{
	// Update to add mouse input. When mouse is clicked and held when hovered over the scene window, move to middle of screen then get difference in position every frame.
	// Also add locking to 180 degrees up/down.

	//camera motion is on a plane, so kill the y component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	//update to use mouse position rather than moving to centre of screen

	// optimize by only doing this when holding mouse, not every update
	RECT rect;
	GetWindowRect(GetActiveWindow(), &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	if (input->RMBDown && !mouseMode)
	{
		clickX = input->mouseX;
		clickY = input->mouseY;

		//lastMouseX = input->mouseX;
		//lastMouseY = input->mouseY;
		mouseMode = true;
	}
	else if(!input->RMBDown)
	{
		mouseMode = false;
		// Process rotation input.
		if (input->rotRight)
		{
			m_camOrientation.y += m_camRotRate;
		}
		if (input->rotLeft)
		{
			m_camOrientation.y -= m_camRotRate;
		}
		if (input->rotUp)
		{
			m_camOrientation.z += m_camRotRate;

		}
		if (input->rotDown)
		{
			m_camOrientation.z -= m_camRotRate;

		}
	}

	float distanceX;
	float distanceY;

	if (mouseMode)
	{
		distanceX = input->mouseX - clickX;
		distanceY = input->mouseY - clickY;

		m_camOrientation.y += distanceX * timer.GetElapsedSeconds() * m_camRotRate;
		m_camOrientation.z -= distanceY * timer.GetElapsedSeconds() * m_camRotRate;

		SetCursorPos(clickX, clickY);
	}


	
	//create look direction from Euler angles in m_camOrientation
	/*
	m_camLookDirection.x = sin((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.Normalize();
	*/

	// x = roll, y = yaw, z = pitch
	// lock pitch so can't look too far up or down
	if (m_camOrientation.z > 90) m_camOrientation.z = 90;
	else if (m_camOrientation.z < -90) m_camOrientation.z = -90;

	// Create look direction using parametric equation of a sphere.
	m_camLookDirection.x = cos((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.z) * 3.1415 / 180);
	m_camLookDirection.y = sin((m_camOrientation.z) * 3.1415 / 180);
	m_camLookDirection.z = sin((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.z) * 3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (input->forward)
	{
		m_camPosition += m_camLookDirection * m_camMoveSpeed;
	}
	if (input->back)
	{
		m_camPosition -= m_camLookDirection * m_camMoveSpeed;
	}
	if (input->right)
	{
		m_camPosition += m_camRight * m_camMoveSpeed;
	}
	if (input->left)
	{
		m_camPosition -= m_camRight * m_camMoveSpeed;
	}
	if (input->down)
	{
		m_camPosition -= m_camUp * m_camMoveSpeed;
	}
	if (input->up)
	{
		m_camPosition += m_camUp * m_camMoveSpeed;
	}


	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	
}
