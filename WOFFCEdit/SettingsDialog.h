#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "resource.h"
#include "ToolMain.h"

class Game;

class SettingsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd* pParent = NULL);
	virtual ~SettingsDialog();
	void SetGameRef(Game* game);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Apply();
	Game* m_gameRef;
	CSliderCtrl m_camMoveSpeed, m_camRotSpeed, m_focusZoomSpeed, m_focusLerpSpeed;
	CSliderCtrl m_objMoveSpeed, m_objRotSpeed, m_objScaleSpeed;
	CSliderCtrl m_sculptRadius, m_sculptMagnitude;
	
	float m_objMultiplier;
	float m_camMultiplier;
	float m_sculptMultiplier;

	DECLARE_MESSAGE_MAP();

public:
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
};

