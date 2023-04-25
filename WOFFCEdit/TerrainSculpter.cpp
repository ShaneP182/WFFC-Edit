#include "TerrainSculpter.h"

TerrainSculpter::TerrainSculpter()
{
	m_sculptMode = SculptMode::RAISE;
	m_radius = 5.0f;
	m_magnitude = 5.0f;
	m_canSculpt = true;
	m_editHeightMap = false;
}

TerrainSculpter::~TerrainSculpter()
{
}

void TerrainSculpter::Sculpt(DisplayChunk* terrain, DirectX::SimpleMath::Vector3 spherePos, DX::StepTimer const& timer)
{
	//FIX SCULPTING WHEN PRESSING TOOLBAR

	if (GetParent(GetActiveWindow()) == 0 && m_inputCommands->pickerY > 16 && m_canSculpt)  // if clicking the main window below the toolbar while can sculpt
	{
		for (int i = 0; i < TERRAINRESOLUTION - 1; i++)
		{
			for (int j = 0; j < TERRAINRESOLUTION - 1; j++)
			{
				// ignore height of positions. works smoother imo.
				DirectX::SimpleMath::Vector3 pos1, pos2;
				pos1 = spherePos;
				pos1.y = 0;

				pos2 = terrain->m_terrainGeometry[i][j].position;
				pos2.y = 0;


				float distance = DirectX::SimpleMath::Vector3::Distance(pos1, pos2);
				if (distance < m_radius)
				{
					int index = (TERRAINRESOLUTION * i) + j;
					float magnitude = MapFloat(distance, 0, m_radius, m_magnitude, m_magnitude / 2) * timer.GetElapsedSeconds(); // amount to extrude by. magnitude of extrusion is half at the edge compared to the centre

					switch (m_sculptMode)
					{
					case SculptMode::RAISE:
						if (m_editHeightMap)
						{
							terrain->GenerateHeightmap(index, magnitude);
						}
						else
						{
							terrain->m_terrainGeometry[i][j].position.y += magnitude;
						}
						break;
					case SculptMode::LOWER:
						if (m_editHeightMap)
						{
							terrain->GenerateHeightmap(index, -magnitude);
						}
						else
						{
							terrain->m_terrainGeometry[i][j].position.y -= magnitude;
						}
						break;
					case SculptMode::FLATTEN:
						if (m_editHeightMap)
						{
							terrain->FlattenHeightmap(index);
						}
						else
						{
							terrain->m_terrainGeometry[i][j].position.y = 0;
						}
						break;
					}

				}
			}
		}
	}
	
}

float TerrainSculpter::MapFloat(float f, float in1, float in2, float out1, float out2)
{
	return out1 + (f - in1) * (out2 - out1) / (in2 - in1);
}
