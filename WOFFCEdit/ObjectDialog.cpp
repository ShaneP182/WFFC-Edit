#include "ObjectDialog.h"
#include "stdafx.h"

IMPLEMENT_DYNAMIC(ObjectDialog, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectDialog, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialog::End)
	ON_COMMAND(IDC_CHECK_VISIBILITY, &ObjectDialog::CheckboxVisibility)
END_MESSAGE_MAP()

ObjectDialog::ObjectDialog(CWnd* pParent, std::vector<SceneObject>* SceneGraph) : CDialogEx(IDD_DIALOG_OBJECT, pParent)
{
	m_sceneGraph = SceneGraph;
}

ObjectDialog::ObjectDialog(CWnd* pParent) : CDialogEx(IDD_DIALOG_OBJECT, pParent)
{
}

ObjectDialog::~ObjectDialog()
{
}

void ObjectDialog::SetObjectData(std::vector<SceneObject>* SceneGraph, int* Selection)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = Selection;
}

CRect ObjectDialog::GetRect()
{
	CRect DialogWindowRect;
	GetWindowRect(DialogWindowRect);
	return DialogWindowRect;
}

void ObjectDialog::Update()
{
	if (*m_currentSelection == -1)
	{
		m_ObjectIDText.SetWindowTextW(L"NONE");

		m_PosX.SetWindowTextW(L"");
		m_PosY.SetWindowTextW(L"");
		m_PosZ.SetWindowTextW(L"");

		m_RotX.SetWindowTextW(L"");
		m_RotY.SetWindowTextW(L"");
		m_RotZ.SetWindowTextW(L"");

		m_ScaleX.SetWindowTextW(L"");
		m_ScaleY.SetWindowTextW(L"");
		m_ScaleZ.SetWindowTextW(L"");

		CheckDlgButton(IDC_CHECK_VISIBILITY, BST_UNCHECKED);
	}
	else
	{
		SceneObject* Object = &m_sceneGraph->at(*m_currentSelection);

		std::wstring IDString = std::to_wstring(Object->ID);
		m_ObjectIDText.SetWindowTextW(IDString.c_str());

		
		//REMEMBER TO DO INPUT VALIDATION SO THEY DON'T ENTER STRINGS FOR POSITIONS

		std::wstring PosX, PosY, PosZ;
		PosX = std::to_wstring(Object->posX);
		PosY = std::to_wstring(Object->posY);
		PosZ = std::to_wstring(Object->posZ);

		m_PosX.SetWindowTextW(PosX.c_str());
		m_PosY.SetWindowTextW(PosY.c_str());
		m_PosZ.SetWindowTextW(PosZ.c_str());

		std::wstring RotX, RotY, RotZ;
		RotX = std::to_wstring(Object->rotX);
		RotY = std::to_wstring(Object->rotY);
		RotZ = std::to_wstring(Object->rotZ);

		m_RotX.SetWindowTextW(RotX.c_str());
		m_RotY.SetWindowTextW(RotY.c_str());
		m_RotZ.SetWindowTextW(RotZ.c_str());

		std::wstring ScaleX, ScaleY, ScaleZ;
		ScaleX = std::to_wstring(Object->scaX);
		ScaleY = std::to_wstring(Object->scaY);
		ScaleZ = std::to_wstring(Object->scaZ);

		m_ScaleX.SetWindowTextW(ScaleX.c_str());
		m_ScaleY.SetWindowTextW(ScaleY.c_str());
		m_ScaleZ.SetWindowTextW(ScaleZ.c_str());

		if (Object->editor_render)
		{
			CheckDlgButton(IDC_CHECK_VISIBILITY, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_VISIBILITY, BST_UNCHECKED);
		}

	}
}

void ObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// do ddx control for thingies?
	DDX_Control(pDX, IDC_STATIC_SELECTED, m_ObjectIDText);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_PosX);
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_PosY);
	DDX_Control(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDX_Control(pDX, IDC_EDIT_ROT_X, m_RotX);
	DDX_Control(pDX, IDC_EDIT_ROT_Y, m_RotY);
	DDX_Control(pDX, IDC_EDIT_ROT_Z, m_RotZ);
	DDX_Control(pDX, IDC_EDIT_SCALE_X, m_ScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALE_Y, m_ScaleY);
	DDX_Control(pDX, IDC_EDIT_SCALE_Z, m_ScaleZ);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_ModelPath);
	DDX_Control(pDX, IDC_COMBO_TEXTURE, m_TexturePath);
}

void ObjectDialog::End()
{
	DestroyWindow();
}

void ObjectDialog::CheckboxVisibility()
{
	if (*m_currentSelection != -1)
	{
		if (IsDlgButtonChecked(IDC_CHECK_VISIBILITY) == BST_CHECKED)
		{
			m_sceneGraph->at(*m_currentSelection).editor_render = true;
		}
		else
		{
			m_sceneGraph->at(*m_currentSelection).editor_render = false;
		}

		//build display list
		//mention for higher object counts, rather than rebuilding the whole list just update single objects
		if (m_gameRef)
		{
			m_gameRef->BuildDisplayList(m_sceneGraph);
		}
	}
}

BOOL ObjectDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void ObjectDialog::PostNcDestroy()
{
}
