#include "MFCMain.h"
#include "resource.h"
#include "ObjectManipulator.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_WINDOW_OBJECTDIALOG, &MFCMain::MenuWindowObject)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_COMMAND(ID_BUTTON_TRANSLATE, &MFCMain::ToolBarTranslate)
	ON_COMMAND(ID_BUTTON_ROTATE, &MFCMain::ToolBarRotate)
	ON_COMMAND(ID_BUTTON_SCALE, &MFCMain::ToolBarScale)
	ON_COMMAND(ID_BUTTON_WIREFRAME, &MFCMain::ToolBarWireframeObjects)
	ON_COMMAND(ID_BUTTON_WIREFRAME_LANDSCAPE, &MFCMain::ToolBarWireframeLandscape)
	ON_COMMAND(ID_BUTTON_FOCUS, &MFCMain::ToolBarFocus)
	ON_COMMAND(ID_BUTTON_NEW_OBJECT, &MFCMain::ToolBarNewObject)
	ON_COMMAND(ID_BUTTON_DEL_OBJECT, &MFCMain::ToolBarDelObject)
	ON_COMMAND(ID_BUTTON_COPY, &MFCMain::ToolBarCopy)
	ON_COMMAND(ID_BUTTON_PASTE, &MFCMain::ToolBarPaste)
	ON_COMMAND(ID_BUTTON_SETTINGS, &MFCMain::ToolBarSettings)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Corgcraft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	m_ToolObjectDialog.Create(IDD_DIALOG_OBJECT);
	m_ToolObjectDialog.SetGameRef(m_ToolSystem.GetGame());
	m_ToolObjectDialog.SetWindowPos(m_pMainWnd, 1024, 100, m_ToolObjectDialog.GetRect().Width(), m_ToolObjectDialog.GetRect().Height(), NULL); // position dialogue next to main window
	m_ToolObjectDialog.ShowWindow(SW_SHOW);
	m_ToolObjectDialog.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);

	m_ToolSettingsDialog.Create(IDD_DIALOG_SETTINGS);
	m_ToolSettingsDialog.SetGameRef(m_ToolSystem.GetGame());

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString;
			//statusString = L"Selected Object: " + std::to_wstring(ID); // not needed anymore, object highlighting and object dialog box indicate this
			
			switch (m_ToolSystem.GetGame()->GetManipulationMode())
			{
			case ManipulationMode::TRANSLATE:
				statusString += L"Control Mode: TRANSLATE";
				break;
			case ManipulationMode::ROTATE:
				statusString += L"Control Mode: ROTATE";
				break;
			case ManipulationMode::SCALE:
				statusString += L"Control Mode: SCALE";
				break;
			}
			
			m_ToolSystem.Tick(&msg);
			m_ToolObjectDialog.Update();

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
			
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::MenuWindowObject()
{
	m_ToolObjectDialog.ShowWindow(SW_SHOW);
	m_ToolObjectDialog.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}

void MFCMain::ToolBarTranslate()
{
	m_ToolSystem.GetGame()->SetManipulationMode(ManipulationMode::TRANSLATE);
}

void MFCMain::ToolBarRotate()
{
	m_ToolSystem.GetGame()->SetManipulationMode(ManipulationMode::ROTATE);
}

void MFCMain::ToolBarScale()
{
	m_ToolSystem.GetGame()->SetManipulationMode(ManipulationMode::SCALE);
}

void MFCMain::ToolBarWireframeObjects()
{
	m_ToolSystem.GetGame()->ToggleWireframeObjects();
}

void MFCMain::ToolBarWireframeLandscape()
{
	m_ToolSystem.GetGame()->ToggleWireframeTerrain();
}

void MFCMain::ToolBarFocus()
{
	m_ToolSystem.GetGame()->FocusObject(m_ToolSystem.m_selectedObject);
}

void MFCMain::ToolBarNewObject()
{
	m_ToolSystem.onActionNewObject();
}

void MFCMain::ToolBarDelObject()
{
	m_ToolSystem.onActionDelObject();
}

void MFCMain::ToolBarCopy()
{
	m_ToolSystem.onActionCopy();
}

void MFCMain::ToolBarPaste()
{
	m_ToolSystem.onActionPaste();
}

void MFCMain::ToolBarSettings()
{
	m_ToolSettingsDialog.ShowWindow(SW_SHOW);
}


MFCMain::MFCMain()
{
	
}


MFCMain::~MFCMain()
{
}
