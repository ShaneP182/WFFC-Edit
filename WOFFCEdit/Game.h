//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SceneObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "ChunkObject.h"
#include "InputCommands.h"
#include <vector>
#include "Camera.h"
#include "ObjectManipulator.h"
#include "DirectXMath.h"
#include "TerrainSculpter.h"
#include <stack>

// A basic game implementation that creates a D3D11 device and
// provides a game loop.

enum class Action
{
	MODIFY,
	ADD,
	REMOVE
};

class Game : public DX::IDeviceNotify
{
public:

	Game();
	~Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);
	void SetGridState(bool state);

	// Basic game loop
	void Tick(InputCommands * Input);
	void Render();

	// Rendering helpers
	void Clear();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	//tool specific
	void BuildDisplayList(std::vector<SceneObject> * SceneGraph); //note vector passed by reference 
	void BuildDisplayChunk(ChunkObject *SceneChunk);
	void SaveDisplayChunk(ChunkObject *SceneChunk);	//saves geometry et al
	void ClearDisplayList();
	std::vector<DisplayObject>* GetDisplayList() { return &m_displayList; };
	void SetManipulationMode(ManipulationMode mode);
	ManipulationMode GetManipulationMode();
	void SetSelection(int* sel) { m_currentSelection = sel; };
	void SetManipulatorSceneGraph(std::vector<SceneObject>* sceneGraph, int* sel) { objectManipulator.SetSceneGraph(sceneGraph, sel); };
	//void SetSceneGraph(std::vector<SceneObject>* sceneGraph) { m_sceneGraph = sceneGraph; };
	ObjectManipulator* GetManipulator() { return &objectManipulator; };
	DirectX::SimpleMath::Vector3 LineTraceTerrain();
	bool GetSculptModeActive() { return m_sculptModeActive; };
	void SetSculptModeActive(bool active) { m_sculptModeActive = active; }
	SculptMode GetSculptMode() { return m_terrainSculpter.GetMode(); }
	void SetSculptMode(SculptMode mode) { m_terrainSculpter.SetMode(mode); };
	TerrainSculpter* GetSculpter() { return &m_terrainSculpter; };

	void ToggleWireframeObjects() { wireframeObjects = !wireframeObjects; };
	void ToggleWireframeTerrain() { wireframeTerrain = !wireframeTerrain; };
	void SetHighlight(bool highlight) { m_highlight = highlight; };
	bool GetHighlight() { return m_highlight; };
	int MousePicking(int curID);
	void FocusObject(int objID);
	float GetDeltaTime() { return m_timer.GetElapsedSeconds(); };
	void ScrollWheel(short delta);
	Camera* GetCamera() { return &camera; };
	float GetZoomSpeed() { return m_focusZoomSpeed; };
	void SetZoomSpeed(float s) { m_focusZoomSpeed = s; };
	DisplayChunk* GetDisplayChunk() { return &m_displayChunk; };
	void AddAction(Action action) { UndoStack.push(action); };
	void AddToModifyStack(SceneObject object);
	void AddToAddStack(SceneObject object);
	void AddToRemoveStack(SceneObject object);
	void ClearUndoRedo();

	std::stack<Action> GetUndoStack() { return UndoStack; };
	std::stack<Action> GetRedoStack() { return RedoStack; };
	void Undo();
	void Redo();
	SceneObject* GetObjectByID(int ID, int& returnIndex);
	void ApplyChanges(SceneObject* newObject, SceneObject oldObject);
	void DeleteSceneObject(int index);
	void AddSceneObject();

	int m_topID;
#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:

	std::stack<Action> UndoStack;  // clear stacks on save/load?
	std::stack<Action> RedoStack;
	std::stack<SceneObject> UndoModifyStack;
	std::stack<SceneObject> RedoModifyStack;
	std::stack<SceneObject> UndoAddStack;
	std::stack<SceneObject> UndoRemoveStack;
	std::stack<SceneObject> RedoRemoveStack;

	void Update(DX::StepTimer const& timer);

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);

	bool m_sculptModeActive;

	std::vector<SceneObject>* m_sceneGraph;
	bool m_ManipulatorUndoFlag;

	//tool specific
	std::vector<DisplayObject>			m_displayList;
	DisplayChunk						m_displayChunk;
	InputCommands						m_InputCommands;

	//camera
	Camera								camera;
	ObjectManipulator					objectManipulator;
	TerrainSculpter						m_terrainSculpter;
	bool wireframeObjects;
	bool wireframeTerrain;
	bool m_highlight;
	int* m_currentSelection;
	RECT m_ScreenDimensions;
	DirectX::SimpleMath::Vector3 m_spherePos;

	
	float m_focus;
	float m_focusMin;
	float m_focusMax;
	float m_focusZoomSpeed;

	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere;
	//control variables
	bool m_grid;							//grid rendering on / off
	// Device resources.
    std::shared_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // Input devices.
    std::unique_ptr<DirectX::GamePad>       m_gamePad;
    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    std::unique_ptr<DirectX::Mouse>         m_mouse;

    // DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states;
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
    std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
    std::unique_ptr<DirectX::Model>                                         m_model;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
    DirectX::SimpleMath::Matrix                                             m_view;
    DirectX::SimpleMath::Matrix                                             m_projection;


};

std::wstring StringToWCHART(std::string s);