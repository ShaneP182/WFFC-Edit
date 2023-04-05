#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "ObjectDialog.h"


class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	ObjectDialog m_ToolObjectDialog;

	int m_width;		
	int m_height;

	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuWindowObject();
	afx_msg	void ToolBarButton1();
	afx_msg void ToolBarTranslate();
	afx_msg void ToolBarRotate();
	afx_msg void ToolBarScale();
	afx_msg void ToolBarWireframeObjects();
	afx_msg void ToolBarWireframeLandscape();
	afx_msg void ToolBarFocus();


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
