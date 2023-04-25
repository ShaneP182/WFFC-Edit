
#include "ObjectDialog.h"
#include "atlstr.h"
#include <atlimage.h>

IMPLEMENT_DYNAMIC(ObjectDialog, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectDialog, CDialogEx)
	ON_COMMAND(IDOK, &ObjectDialog::End)
	ON_COMMAND(IDC_BUTTON_APPLY, &ObjectDialog::UpdateObjectFromEditBoxes)
	ON_COMMAND(IDC_CHECK_VISIBILITY, &ObjectDialog::CheckboxVisibility)
	ON_COMMAND(IDC_CHECK_SNAP, &ObjectDialog::CheckboxSnapToGround)
END_MESSAGE_MAP()

ObjectDialog::ObjectDialog(CWnd* pParent, std::vector<SceneObject>* SceneGraph) : CDialogEx(IDD_DIALOG_OBJECT, pParent)
{
	m_sceneGraph = SceneGraph;
	previousSelection = 1;
	
	
	bSelectionChanged = true;

	
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
	UpdateObject();
}

CRect ObjectDialog::GetRect()
{
	CRect DialogWindowRect;
	GetWindowRect(DialogWindowRect);
	return DialogWindowRect;
}

void ObjectDialog::Update()
{
	//for (CustomCEdit* editBox : m_EditBoxes)
	for (int i = 0; i < m_EditBoxes.size(); i++)
	{
		if (m_EditBoxes[i]->updateFlag)
		{
			UpdateObjectFromEditBoxes();
			m_EditBoxes[i]->updateFlag = false;
		}
	}

	if (*m_currentSelection != previousSelection)
	{
		if (*m_currentSelection == -1)
		{
			ClearObject();
		}
		else
		{
			UpdateObject();
		}

		bSelectionChanged = true;
		previousSelection = *m_currentSelection;
	}
	else
	{
		bSelectionChanged = false;
		if (m_gameRef->GetManipulator()->isManipulating)
		{
			UpdateObject();
		}
	}

	int texSelection = m_TexturePath.GetCurSel();
	int modSelection = m_ModelPath.GetCurSel();

	if (*m_currentSelection != -1)
	{
		if (m_sceneGraph->at(*m_currentSelection).snapToGround)
		{
			m_PosY.SetReadOnly(TRUE);
			// do same for rotx and rotz if changing rotation based on direction vector.
		}
		else
		{
			m_PosY.SetReadOnly(FALSE);
		}
	}
	else
	{
		m_PosY.SetReadOnly(FALSE);
	}
	
	if (texSelection == -1)
	{
		m_TexturePicture.SetBitmap(m_NoPreview);
	}
	else
	{
		CString path;
		m_TexturePath.GetLBText(texSelection, path);

		if (m_TexturePreviews[path])
		{
			m_TexturePicture.SetBitmap(m_TexturePreviews[path]);
		}
		else
		{
			m_TexturePicture.SetBitmap(m_NoPreview);
		}
		
	}
	
	if (modSelection == -1)
	{
		m_ModelPicture.SetBitmap(m_NoPreview);
	}
	else
	{
		CString path;
		m_ModelPath.GetLBText(modSelection, path);

		if (m_ModelPreviews[path])
		{
			m_ModelPicture.SetBitmap(m_ModelPreviews[path]);
		}
		else
		{
			m_ModelPicture.SetBitmap(m_NoPreview);
		}
	}
}



void ObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
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
	DDX_Control(pDX, IDC_STATIC_MODEL, m_ModelPicture);
	DDX_Control(pDX, IDC_STATIC_TEXTURE, m_TexturePicture);
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

void ObjectDialog::CheckboxSnapToGround()
{
	if (*m_currentSelection != -1)
	{
		if (IsDlgButtonChecked(IDC_CHECK_SNAP) == BST_CHECKED)
		{
			m_sceneGraph->at(*m_currentSelection).snapToGround = true;
		}
		else
		{
			m_sceneGraph->at(*m_currentSelection).snapToGround = false;
		}

		if (m_gameRef)
		{
			m_gameRef->BuildDisplayList(m_sceneGraph);
		}
	}
}

void ObjectDialog::UpdateObject()
{
	SceneObject* Object = &m_sceneGraph->at(*m_currentSelection);

	std::wstring IDString = std::to_wstring(Object->ID);
	m_ObjectIDText.SetWindowTextW(IDString.c_str());



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
	
	if (Object->snapToGround)
	{
		CheckDlgButton(IDC_CHECK_SNAP, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_SNAP, BST_UNCHECKED);
	}

	std::wstring modelPath = std::wstring(Object->model_path.begin(), Object->model_path.end());
	std::wstring texturePath = std::wstring(Object->tex_diffuse_path.begin(), Object->tex_diffuse_path.end());

	int modelIndex = m_ModelPath.FindString(0, modelPath.c_str());
	int textureIndex = m_TexturePath.FindString(0, texturePath.c_str());

	m_ModelPath.SetCurSel(modelIndex);
	m_TexturePath.SetCurSel(textureIndex);
	
	//m_gameRef->BuildDisplayList(m_sceneGraph); // dont build display list, change the object position

}

void ObjectDialog::UpdateObjectFromEditBoxes()
{
	// update from value
	// update scene graph object
	// then update object through object manipulator
	if (*m_currentSelection != -1)
	{

		//NEED TO MAKE OWN CEDITS TO HANDLE INPUT VALIDATION. CURRENTLY DOES NOT ACCEPT NEGATIVES OR DECIMAL
		SceneObject* Object = &m_sceneGraph->at(*m_currentSelection);
		CString text;

		m_PosX.GetWindowText(text);
		Object->posX = _ttof(text);
		m_PosY.GetWindowText(text);
		Object->posY = _ttof(text);
		m_PosZ.GetWindowText(text);
		Object->posZ = _ttof(text);

		m_RotX.GetWindowText(text);
		Object->rotX = _ttof(text);
		m_RotY.GetWindowText(text);
		Object->rotY = _ttof(text);
		m_RotZ.GetWindowText(text);
		Object->rotZ = _ttof(text);

		m_ScaleX.GetWindowText(text);
		Object->scaX = _ttof(text);
		m_ScaleY.GetWindowText(text);
		Object->scaY = _ttof(text);
		m_ScaleZ.GetWindowText(text);
		Object->scaZ = _ttof(text);

		m_gameRef->GetManipulator()->SetObjectPosition(Object->posX, Object->posY, Object->posZ);
		m_gameRef->GetManipulator()->SetObjectRotation(Object->rotX, Object->rotY, Object->rotZ);
		m_gameRef->GetManipulator()->SetObjectScale(Object->scaX, Object->scaY, Object->scaZ);
		// if bug happens when switching objects, specify which object to transform and use previous selection

		bool rebuildDisplayList = false;
		std::wstring modelPath = std::wstring(Object->model_path.begin(), Object->model_path.end());
		std::wstring texturePath = std::wstring(Object->tex_diffuse_path.begin(), Object->tex_diffuse_path.end());
		
		CString boxPath;
		m_ModelPath.GetLBText(m_ModelPath.GetCurSel(), boxPath);
		// if texture/model changed, rebuild display list with new model/texture
		if (modelPath != boxPath.GetString())
		{
			rebuildDisplayList = true;
			Object->model_path = CW2A(boxPath.GetString());
		}

		m_TexturePath.GetLBText(m_TexturePath.GetCurSel(), boxPath);
		if (texturePath != boxPath.GetString())
		{
			rebuildDisplayList = true;
			Object->tex_diffuse_path = CW2A(boxPath.GetString());
		}

		if (rebuildDisplayList)
		{
			m_gameRef->BuildDisplayList(m_sceneGraph);
		}

		UpdateObject();
	}
}

void ObjectDialog::ClearObject()
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
	CheckDlgButton(IDC_CHECK_SNAP, BST_UNCHECKED);

	m_ModelPath.SetCurSel(-1);
	m_TexturePath.SetCurSel(-1);
}

BOOL ObjectDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PictureSize = 112;
	std::vector<CString> modelNames{ L"bedroll", L"campfire", L"corgi", L"doghouse", L"crate", L"placeholder", L"pug", L"thrall" };
	std::vector<CString> textureNames{ L"bedroll", L"campfire", L"corgi", L"doghouse", L"crate", L"placeholder", L"pug", L"thrall", L"blank"};

	for (int i = 0; i < modelNames.size(); i++)
	{
		CString path = L"database/data/" + modelNames[i] + L".cmo";
		CString previewPath = L"database/data/" + modelNames[i] + L"_preview.bmp";

		HBITMAP image = (HBITMAP)LoadImage(NULL, previewPath, IMAGE_BITMAP, m_PictureSize, m_PictureSize, LR_LOADFROMFILE);

		m_ModelPath.AddString(path);
		if (image)
		{
			m_ModelPreviews.emplace(path, image);
		}
	}

	m_ModelPath.SetCurSel(-1);

	for (int i = 0; i < textureNames.size(); i++)
	{
		CString path = L"database/data/" + textureNames[i] + L".dds";
		CString previewPath = L"database/data/" + textureNames[i] + L"_tex.bmp";

		HBITMAP image = (HBITMAP)LoadImage(NULL, previewPath, IMAGE_BITMAP, m_PictureSize, m_PictureSize, LR_LOADFROMFILE);

		m_TexturePath.AddString(path);
		if (image)
		{
			m_TexturePreviews.emplace(path, image);
		}
	}

	m_TexturePath.SetCurSel(-1);
	
	m_NoPreview = (HBITMAP)LoadImage(NULL, L"database/data/no_preview.bmp", IMAGE_BITMAP, m_PictureSize, m_PictureSize, LR_LOADFROMFILE);
	m_TexturePicture.SetBitmap(m_NoPreview);
	m_ModelPicture.SetBitmap(m_NoPreview);

	m_EditBoxes.push_back(&m_PosX);
	m_EditBoxes.push_back(&m_PosY);
	m_EditBoxes.push_back(&m_PosZ);
	m_EditBoxes.push_back(&m_RotX);
	m_EditBoxes.push_back(&m_RotY);
	m_EditBoxes.push_back(&m_RotZ);
	m_EditBoxes.push_back(&m_ScaleX);
	m_EditBoxes.push_back(&m_ScaleY);
	m_EditBoxes.push_back(&m_ScaleZ);

	return TRUE;
}

void ObjectDialog::PostNcDestroy()
{
}
