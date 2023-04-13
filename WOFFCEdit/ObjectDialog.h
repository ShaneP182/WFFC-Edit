#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "resource.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include "CustomCEdit.h"
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
	void UpdateObject();
	void UpdateObjectFromEditBoxes();
	void ClearObject();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void CheckboxVisibility();
	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;
	int previousSelection;
	bool bSelectionChanged;
	int m_PictureSize;

	DECLARE_MESSAGE_MAP();

public:

	CStatic m_ObjectIDText;
	CustomCEdit m_PosX, m_PosY, m_PosZ;
	CustomCEdit m_RotX, m_RotY, m_RotZ;
	CustomCEdit m_ScaleX, m_ScaleY, m_ScaleZ;
	CComboBox m_ModelPath, m_TexturePath;
	CStatic m_ModelPicture, m_TexturePicture;
	Game* m_gameRef;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();

	std::vector<CEdit*> m_EditBoxes;
	CustomCEdit* m_FocusedEditBox;
	CustomCEdit* m_PrevFocusedEditBox;

	HBITMAP m_NoPreview;
	std::vector<HBITMAP> m_TexturePreviews;
	std::vector<HBITMAP> m_ModelPreviews;
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);