#include "stdafx.h"
#include "CustomCEdit.h"

// validate input - ignore anything not a number, '.', or '-'
// only accept one '.' aand '-'
// only accept '-' at the start

IMPLEMENT_DYNAMIC(CustomCEdit, CEdit)

BEGIN_MESSAGE_MAP(CustomCEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

CustomCEdit::CustomCEdit()
{

}

CustomCEdit::~CustomCEdit()
{
}

void CustomCEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString CurString; // current string in the edit box
	GetWindowText(CurString);

	int nStart;
	int nEnd;
	CEdit::GetSel(nStart, nEnd);

	if (nChar >= '0' && nChar <= '9' || nChar == '\b')
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == '.')
	{
		if (CurString.Find('.') == -1)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if (nChar == '-')
	{
		if (CurString.Find('-') == -1 && nStart == 0)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}	
	else if (nChar == '\r')
	{
		updateFlag = true;
	}
}

