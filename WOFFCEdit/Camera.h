#include "pch.h"
#include "InputCommands.h"
#include "StepTimer.h"
#include <vector>

#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	void Update(DX::StepTimer const& timer, InputCommands* input);
	DirectX::SimpleMath::Vector3 GetPosition() { return m_camPosition; };
	DirectX::SimpleMath::Vector3 GetLookAt() { return m_camLookAt; };

private:
	// Camera properties
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	DirectX::SimpleMath::Vector3		m_camUp;
	float m_camRotRate;
	float m_camMoveSpeed;
	bool mouseMode;
	int lastMouseX;
	int lastMouseY;
};

