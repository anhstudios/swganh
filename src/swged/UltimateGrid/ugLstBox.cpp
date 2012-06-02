/*************************************************************************
				Class Implementation : CUGLstBox
**************************************************************************
	Source file : ugLstBox.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGLstBox::CUGLstBox()
{
	m_HasFocus	= FALSE;
	m_col		= NULL;
	m_row		= NULL;
	m_selected	= FALSE;

	m_ctrl		= NULL;
}

CUGLstBox::~CUGLstBox()
{
}

BEGIN_MESSAGE_MAP(CUGLstBox, CListBox)
	//{{AFX_MSG_MAP(CUGLstBox)
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
OnSetFocus
	event is fired when the list box is gaining focus.
	The drop list cell type uses this event handler
	to setup mouse capture.
Params:
	pOldWnd	- pointer to window that has lost focus
Return:
	<none>
****************************************************/
void CUGLstBox::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);

	ReleaseCapture();
	SetCapture();

	m_HasFocus = TRUE;
	m_selected = FALSE;
}

/***************************************************
OnKillFocus
	event is called when the list control is loosing
	the focus.  The drop list cell type uses this event
	to clear the mouse capture, and to hide the list
	control.
Params:
	pNewWnd	- pointer to the window that is gaining
			  the focus.
Return:
	<none>
****************************************************/
void CUGLstBox::OnKillFocus(CWnd* pNewWnd) 
{
	ReleaseCapture();

	CListBox::OnKillFocus(pNewWnd);

	DestroyWindow();
	
	m_HasFocus = FALSE;
	if(m_selected == FALSE){
		*m_col = -2;
		*m_row = -2;
	}
	m_selected = FALSE;
	m_ctrl->m_CUGGrid->SendMessage(WM_PAINT,0,0);
}

/***************************************************
OnMouseActivate
	The framework calls this member function when the
	cursor is in an inactive window and the user presses
	a mouse button. 
Params:
	The drop list cell type does not use any of the
	parameters passed into this event handler.  To
	get more information on the meaning of these
	parameters please refer to the MSDN.
Return:
	MA_ACTIVATE to allow the inactive window to activate.
****************************************************/
int CUGLstBox::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(pDesktopWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	return MA_ACTIVATE;	
}

/***************************************************
OnLButtonUp
	event handler is called when user clicks left mouse
	button in the drop list.  The drop list cell type
	uses this event as a signal that the user has
	selected an item.
Params:
	The drop list cell type does not use any of the
	parameters passed into this event handler.  To
	get more information on the meaning of these
	parameters please refer to the MSDN.
Return:
	<none>
****************************************************/
void CUGLstBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

	ReleaseCapture();

	//update the string in the current cell if the list box had a selected item
	if(GetCurSel() != LB_ERR)
	{
		Select();		
	}
}

/***************************************************
OnKeyDown
	event is called when user presses a key when the drop
	list window is active.  The drop list cell type checks
	the key pressed for ESC or ENTER keys.
Params:
	nChar	- if the user hits the ESC key than the drop
			  list will be hidden and selection cancelled.
			  If the user hits ENTER key than the item
			  currently selected will be set to the cell.
Return:
	<none>
****************************************************/
void CUGLstBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_ESCAPE)
		DestroyWindow();

	if(nChar == VK_RETURN)
	{
		if(GetCurSel() != LB_ERR)
		{
			Select();
		}
		else
		{
			m_HasFocus = FALSE;
			DestroyWindow();
		}
	}
	
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

/***************************************************
Select
	this internal function is used by the drop list
	cell type to set the item selected by the user
	to the cell.  It will also fire the OnCellTypeNotify
	notification to inform CUGCtrl derived class 
	that user made his selection.
Params:
	<none>
Return:
	<none>
****************************************************/
void CUGLstBox::Select()
{
	CUGCell cell;
	CString string;
	int col = *m_col; //m_ctrl->GetCurrentCol();
	long row = *m_row; //m_ctrl->GetCurrentRow();

	m_selected = TRUE;

	// make sure that the cell is not read only before making
	// user's selection permanent
	m_ctrl->GetCellIndirect(col,row,&cell);
	if ( cell.GetReadOnly() == FALSE )
	{
		//get the string selected
		GetText(GetCurSel(),string);

		// send notification with index of the selected item
		m_cellType->OnCellTypeNotify(	m_cellTypeId, col, row,
									UGCT_DROPLISTSELECTEX, (long)GetCurSel());

		//notify the user of the selection
		if(m_cellType->OnCellTypeNotify(m_cellTypeId,col,row, //set the id
			UGCT_DROPLISTSELECT,(LONG_PTR)&string) != FALSE){

			m_ctrl->GetCellIndirect(col,row,&cell);
			cell.SetText(string);
			m_ctrl->SetCell(col,row,&cell);

			// notify the user that the selection was set
			m_cellType->OnCellTypeNotify(m_cellTypeId,col,row,UGCT_DROPLISTPOSTSELECT,(LONG_PTR)&string);

			m_ctrl->RedrawCell(col,row);
		}
	}
	
	m_HasFocus = FALSE;
	if(m_col != NULL)
		*m_col = -1;
	if(m_row != NULL)
		*m_row = -1;

	m_ctrl->m_CUGGrid->SetFocus();
}

/***************************************************
OnGetDlgCode
	is required when the grid is on the dialog.
Params:
	<none>
Return:
	<none>
****************************************************/
UINT CUGLstBox::OnGetDlgCode() 
{
	return CListBox::OnGetDlgCode() | DLGC_WANTALLKEYS | DLGC_WANTARROWS;
}
