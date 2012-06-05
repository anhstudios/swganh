/*************************************************************************
				Class Implementation : CUGEdit
**************************************************************************
	Source file : ugedit.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include <ctype.h>
#include "UGCtrl.h"

#include "ugxpthemes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGEdit::CUGEdit() : m_autoSize(TRUE)
{
}

CUGEdit::~CUGEdit()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGEdit, CEdit)
	//{{AFX_MSG_MAP(CUGEdit)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
OnSetFocus
	event handler is user to initially size the edit control, and to setup
	user's selection.
Params:
	pOldWnd		- pointer to window that lost focus.
Returns:
	<none>
*****************************************************/
void CUGEdit::OnSetFocus(CWnd* pOldWnd) 
{
	//setup the autosize ability
	m_tempAutoSize = m_autoSize;
	CUGCellType* ct = m_ctrl->GetCellType(
		m_ctrl->m_editCell.GetCellType() );
	if(ct->CanOverLap(&m_ctrl->m_editCell) == FALSE)
		m_tempAutoSize = FALSE;

	//get the original height
	RECT rect;
	GetClientRect(&rect);
	m_origHeight = rect.bottom +4;
	m_currentHeight = m_origHeight;

	CalcEditRect();

	m_cancel = FALSE;
	m_continueFlag = FALSE;

	SetSel(0,-1);
	
	CEdit::OnSetFocus(pOldWnd);
}

/***************************************************
OnSetCursor
	The framework calls this member function if mouse input is not captured
	and the mouse causes cursor movement within the CWnd object.
Params:
	pWnd		- please see MSDN for more information on the parameters.
	nHitTest
	message
Returns:
	Nonzero to halt further processing, or 0 to continue.
*****************************************************/
BOOL CUGEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(*pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(NULL,IDC_IBEAM));
	return 1;
}

/***************************************************
OnUpdate
	notification handler is called as the result of the EN_UPDATE notification
	message.  This notificatoin is sent when an edit control is about to redraw
	itself after it has formatted the text, but before it displays the text. 
	This makes it possible to resize the edit control window
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGEdit::OnUpdate() 
{
	CalcEditRect();

	if (UGXPThemes::IsThemed())
	    UpdateCtrl();
}


/***************************************************
CalcEditRect
	function is used to re-calculate the area covered by the edit control, it
	is important piece of an auto expanding edit control.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGEdit::CalcEditRect()
{
	if(!m_tempAutoSize || !m_ctrl->m_GI->m_enableCellOverLap)
		return;
	
	CString string;
	RECT	editRect;
	RECT	parentRect;
	RECT	fmtRect;
	CDC*	dc;
	CFont*	oldFont = NULL;

	GetWindowText(string);
	string += _T(" X X");

	GetWindowRect(&editRect);
	m_ctrl->m_editParent->ScreenToClient(&editRect);

	m_ctrl->m_editParent->GetClientRect(&parentRect);

	dc = GetDC();

	CopyRect(&fmtRect,&editRect);
	fmtRect.right = parentRect.right;
	fmtRect.left +=2;

//	if (!UGXPThemes::GetTextExtent(m_hWnd, *dc, XPCellTypeData, ThemeStateNormal, string.GetBuffer(string.GetLength()), string.GetLength(), DT_LEFT | DT_WORDBREAK, &fmtRect))
	{
		if(GetFont() != NULL)
			oldFont = (CFont *)dc->SelectObject(GetFont());
		
		dc->DrawText(string,&fmtRect,DT_CALCRECT|DT_WORDBREAK);
 		
		fmtRect.left -=2;

		if(GetFont() != NULL && oldFont != NULL )
			dc->SelectObject(oldFont);
		ReleaseDC(dc);
	}

	//if(fmtRect.right < editRect.right)
	//	fmtRect.right = editRect.right;

	if(fmtRect.bottom >= parentRect.bottom)
		fmtRect.bottom = parentRect.bottom - 1;

	
	if(fmtRect.right > editRect.right)
	{
		int col;
		long row;
		RECT rect;
		m_ctrl->GetCellFromPoint(fmtRect.right+1,fmtRect.top+1,&col,&row,&rect);
		if(rect.right > parentRect.right)
			rect.right = parentRect.right;

		fmtRect.right = rect.right -1;
	}
	
	if(fmtRect.bottom > editRect.bottom)
	{
		fmtRect.right = parentRect.right;
		int col;
		long row;
		RECT rect;
		m_ctrl->GetCellFromPoint(fmtRect.left +1,fmtRect.bottom+1,&col,&row,&rect);
		fmtRect.bottom = rect.bottom -1;
	}

	if(fmtRect.bottom >= parentRect.bottom)
		fmtRect.bottom = parentRect.bottom - 1;

	if(fmtRect.bottom < editRect.bottom)
		fmtRect.bottom = editRect.bottom;
	if(fmtRect.right < editRect.right)
		fmtRect.right = editRect.right;

	if(fmtRect.right != editRect.right || fmtRect.bottom != editRect.bottom)
	{
		MoveWindow(&fmtRect,TRUE);
		GetParent()->ValidateRect(&fmtRect);
		m_currentHeight = fmtRect.bottom - fmtRect.top;
	}
}

/***************************************************
SetAutoSize
	function is used to enable or disable auto-sizing mode of the edit control.
Params:
	state		- new state of the auto size flag (TRUE/FALSE) indicating
				  if the edit control is allowed to automatically expand.
Returns:
	previous state
*****************************************************/
BOOL CUGEdit::SetAutoSize(BOOL state)
{
	BOOL old = m_autoSize;

	if(state)
		m_autoSize = TRUE;
	else 
		m_autoSize = FALSE;

	return old;
}

/***************************************************
PreTranslateMessage
	function is overwritten here to provide handling of the ESC key when the grid
	is placed on a dialog.
Params:
	pMsg		- please see MSDN for more information on the parameters.
Returns:
	Nonzero if the message was translated and should not be dispatched.
*****************************************************/
BOOL CUGEdit::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE )
		{
			m_cancel = TRUE;
			m_continueFlag = FALSE;
			m_ctrl->SetFocus();
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}
