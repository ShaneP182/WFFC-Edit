#pragma once
#include "DisplayChunk.h"
#include "StepTimer.h"
#include "InputCommands.h"

enum class SculptMode {
	RAISE,
	LOWER,
	FLATTEN
};

class TerrainSculpter
{
public:
	TerrainSculpter();
	~TerrainSculpter();

	void Sculpt(DisplayChunk* terrain, DirectX::SimpleMath::Vector3 spherePos, DX::StepTimer const& timer);
	float GetRadius() { return m_radius; };
	float GetMagnitude() { return m_magnitude; };
	void SetRadius(float r) { m_radius = r; };
	void SetMagnitude(float m) { m_magnitude = m; };
	SculptMode GetMode() { return m_sculptMode; };
	void SetMode(SculptMode mode) { m_sculptMode = mode; };
	void SetInput(InputCommands* input) { m_inputCommands = input; };
	static float MapFloat(float f, float in1, float in2, float out1, float out2);

	bool m_canSculpt;
	bool m_editHeightMap;
protected:
	SculptMode m_sculptMode;
	float m_radius;
	float m_magnitude;
	InputCommands* m_inputCommands;
};

