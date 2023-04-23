#pragma once
#include <afxwin.h>
class CustomCEdit : public CEdit
{
	DECLARE_DYNAMIC(CustomCEdit)

public:
	CustomCEdit();
	virtual ~CustomCEdit();

	bool updateFlag;

protected:
	DECLARE_MESSAGE_MAP();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

