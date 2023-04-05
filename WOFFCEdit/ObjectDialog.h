#pragma once
#include <afxdialogex.h>
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include <vector>

class Game; //forward declaration, all my homies hate circular dependencies

class ObjectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialog)

public:
	ObjectDialog(CWnd* pParent, std::vector<SceneObject>* SceneGraph);
	ObjectDialog(CWnd* pParent = NULL);
	virtual ~ObjectDialog();
	void SetObjectData(std::vector<SceneObject>* SceneGraph, int* Selection);
	void SetGameRef(Game* game) { m_gameRef = game; };
	CRect GetRect();
	void Update();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void CheckboxVisibility();
	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;

	DECLARE_MESSAGE_MAP();

public:

	CStatic m_ObjectIDText;
	CEdit m_PosX, m_PosY, m_PosZ;
	CEdit m_RotX, m_RotY, m_RotZ;
	CEdit m_ScaleX, m_ScaleY, m_ScaleZ;
	CComboBox m_ModelPath, m_TexturePath;
	Game* m_gameRef;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();

};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);