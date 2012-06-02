/*************************************************************************
				Class Implementation : CUGnrBtn
**************************************************************************
	Source file : UGnrBtn.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"
// define WM_HELPHITTEST messages
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGCnrBtn::CUGCnrBtn()
{
	//init the variables
	m_canSize = FALSE;
	m_isSizing = FALSE;
}

CUGCnrBtn::~CUGCnrBtn()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGCnrBtn, CWnd)
	//{{AFX_MSG_MAP(CUGCnrBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, ToolTipNeedText )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUGCnrBtn message handlers

/************************************************
OnPaint
	Paints the corner button object. The corner button object uses a CUGCell
	located at position -1,-1 from the default datasource then uses the cell
	type associated with this cell to draw itself with.
Params:
	<none>
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnPaint() 
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	CPaintDC dc(this); // device context for painting

	m_ctrl->OnScreenDCSetup(&dc,NULL,UG_TOPHEADING);
	
	RECT rect;
	GetClientRect(&rect);

	CUGCellType * cellType;
	CUGCell cell;
	m_ctrl->GetCellIndirect(-1,-1,&cell);

	//get the cell type to draw the cell
	if(cell.IsPropertySet(UGCELL_CELLTYPE_SET))
		cellType = m_ctrl->GetCellType(cell.GetCellType());
	else
		cellType = m_ctrl->GetCellType(-1);
	
	CFont * pOldFont = NULL;

	// get the default font if there is one
	if( m_GI->m_defFont != NULL )
	{
		pOldFont = dc.SelectObject( ( CFont* )m_GI->m_defFont );
	}
	
	cellType->OnDraw(&dc,&rect,-1,-1,&cell,0,0);

	if( m_GI->m_defFont != NULL && pOldFont != NULL )
		dc.SelectObject( pOldFont );
}
/************************************************
Update
	This function is called by the grid when certian properties are changed and
	when the grid is resized.  This function must repaint itself if a change
	was made that will affect this object
Params:
	<none>
Return:
	<none>
*************************************************/
void CUGCnrBtn::Update()
{
	InvalidateRect(NULL);
	UpdateWindow();
}

/************************************************
Moved
	This function is called whenever the current cell in the grid is moved or
	the drag cell is moved. If the movement affects this cell then it should 
	be repainted.
Params:
	<none>
Return:
	<none>
*************************************************/
void CUGCnrBtn::Moved()
{
}

/************************************************
OnLButtonDblClk
	This function is called during a double click on this object. This function
	will then notify the main grid class of the event.
Params:
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL processed = FALSE;
	UNREFERENCED_PARAMETER(nFlags);

	m_ctrl->OnCB_DClicked(&m_GI->m_cnrBtnRect,&point,processed);
}

/************************************************
OnLButtonDown
	This function is called when the left mouse button was just pressed over 
	this object.  This function will then notify the main grid class of the 
	event.  If the mouse is at the edge of the cell where user resizing can
	take please then resizing is enabled (if the user resize option is enabled)
Params:
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL processed = FALSE;
	UNREFERENCED_PARAMETER(nFlags);

	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	if(m_canSize)
	{
		processed = TRUE;
		m_isSizing = TRUE;
		SetCapture();
	}

	m_ctrl->OnCB_LClicked(1,&m_GI->m_cnrBtnRect,&point,processed);
}

/************************************************
OnLButtonUp
	This function is called when the left mouse button was just released over 
	this object.  This function will then notify the main grid class of the
	event.  If resizing was in progress then it is terminated and a notification
	on the resize is made to the main grid.
Params
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return
	<none>
*************************************************/
void CUGCnrBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL processed = FALSE;
	UNREFERENCED_PARAMETER(nFlags);

	if(m_isSizing)
	{
		processed = TRUE;
		m_isSizing = FALSE;
		ReleaseCapture();

		if (m_sizingHeight)
		{
			m_ctrl->OnTopHdgSized(&m_GI->m_topHdgHeight);
		}
		else
		{
			m_ctrl->OnSideHdgSized(&m_GI->m_sideHdgWidth);
		}
	}
	
	m_ctrl->OnCB_LClicked(0,&m_GI->m_cnrBtnRect,&point,processed);
}

/************************************************
OnMouseMove
	This function is called when the mouse is moved over this object.  If user
	resizing is allowed then this function check to see if the mouse is located
	at a position where resizing can take place, and sets a m_canSize flag.
	If user resizing has been started then this function resizes the top or
	side heading, according to the position of the mouse.
Params:
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	//check to see if the mouse is over a separation line
	if(m_isSizing == FALSE && (nFlags&MK_LBUTTON)==0 && m_GI->m_userSizingMode >0)
	{
		m_canSize = FALSE;
		m_sizingWidth = FALSE;
		m_sizingHeight = FALSE;
		
		RECT rect;
		GetClientRect(&rect);
		if(point.x > rect.right -4 && m_ctrl->OnCanSizeSideHdg())
		{
			m_canSize = TRUE;
			m_sizingWidth = TRUE;
			SetCursor(m_GI->m_WEResizseCursor);
		}
		else if(point.y > rect.bottom -4 && m_ctrl->OnCanSizeTopHdg())
		{
			m_canSize = TRUE;
			m_sizingHeight = TRUE;
			SetCursor(m_GI->m_NSResizseCursor);
		}
	}
	else if(m_isSizing)
	{
		if(m_sizingHeight)
		{
			if(point.y <0)
				point.y =0;
			
			int height = point.y;
			m_ctrl->OnTopHdgSizing(&height);
	
			m_ctrl->SetTH_Height(height);
			m_ctrl->AdjustComponentSizes();
		}
		else
		{
			if(point.x <0)
				point.x =0;
			int width = point.x;
			
			m_ctrl->OnSideHdgSizing(&width);

			m_ctrl->SetSH_Width(width);
			m_ctrl->AdjustComponentSizes();
		}
	}
}

/************************************************
OnRButtonDown
	This function is called during a right mouse down event.  This function
	will then notify the main grid class of the event.  If popup menus are
	enabled then this function calls the grids StartMenu function.
Params:
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL processed = FALSE;
	UNREFERENCED_PARAMETER(nFlags);

	m_ctrl->OnCB_RClicked(1,&m_GI->m_cnrBtnRect,&point,processed);

	if(m_GI->m_enablePopupMenu){
		ClientToScreen(&point);
		m_ctrl->StartMenu(-1,-1,&point,UG_CORNERBUTTON);
	}
}

/************************************************
OnRButtonUp
	This function is called during a right mouse up event. This function will
	then notify the main grid class of the event.
Params:
	nFlags	- mouse button flags
	point	- point where mouse was clicked
Return:
	<none>
*************************************************/
void CUGCnrBtn::OnRButtonUp(UINT nFlags, CPoint point) 
{
	BOOL processed = FALSE;
	UNREFERENCED_PARAMETER(nFlags);

	m_ctrl->OnCB_RClicked(0,&m_GI->m_cnrBtnRect,&point,processed);
}

/************************************************
OnSetCursor
	The framework calls this member function if mouse input is not captured
	and the mouse causes cursor movement within the CWnd object.
Params:
	pWnd		- please see MSDN for more information on the parameters.
	nHitTest
	message
Returns:
	Nonzero to halt further processing, or 0 to continue.
*************************************************/
BOOL CUGCnrBtn::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(*pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	if(m_canSize)
	{
		if(m_sizingWidth && m_sizingHeight)
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
		else if(m_sizingWidth)
			SetCursor(m_GI->m_WEResizseCursor);
		else if(m_sizingHeight)
			SetCursor(m_GI->m_NSResizseCursor);
	}
	else
		SetCursor(m_GI->m_arrowCursor);

	return 1;
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help being activated (with mouse) 
	over the corner button.
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Return:
	Context help ID to be displayed
*************************************************/
LRESULT CUGCnrBtn::OnHelpHitTest(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( -1, -1, UG_CORNERBUTTON );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help being activated (with mouse) 
	over the corner button if the grid is on the dialog.
Params:
	HELPINFO - structure that contains information on selected help topic
Return:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGCnrBtn::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( -1, -1, UG_CORNERBUTTON ));
		// prevent further handling of this message
		return TRUE;
	}
	return FALSE;
}

/***************************************************
OnToolHitTest
	The framework calls this member function to detemine whether a point is in
	the bounding rectangle of the specified tool. If the point is in the
	rectangle, it retrieves information about the tool.
Params:
	point	- please see MSDN for more information on the parameters.
	pTI
Returns:
	If 1, the tooltip control was found; If -1, the tooltip control was not found.
*****************************************************/
int CUGCnrBtn::OnToolHitTest(  CPoint point, TOOLINFO *pTI ) const
{
	UNREFERENCED_PARAMETER(point);

	int col = -1, row = -1;
	CRect rect;
	static int lastCol = -2;
	static int lastRow = -2;

	if(col != lastCol || row != lastRow)
	{
		lastCol = col;
		lastRow = row;
		CancelToolTips();
		return -1;
	}

	pTI->uFlags =  TTF_NOTBUTTON | TTF_ALWAYSTIP |TTF_IDISHWND ;
	pTI->uId = (UINT_PTR)m_hWnd;
	pTI->hwnd = (HWND)m_hWnd;
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return 1;
}

/***************************************************
ToolTipNeedText (TTN_NEEDTEXT)
	message handler will be called whenever a text is required for a tool tip.
	The Ultimate Grid uses the CUGCtrl::OnHint virtual function to obtain
	proper text to be displayed in the too tip.
Params:
		id			- Identifier of the control that sent the notification. Not
					  used. The control id is taken from the NMHDR structure.
		pTTTStruct	- A pointer to theNMTTDISPINFO structure. This structure is
					  also discussed further in The TOOLTIPTEXT Structure.
		pResult		- A pointer to result code you can set before you return.
					  TTN_NEEDTEXT handlers can ignore the pResult parameter.
Returns:
	<none>
*****************************************************/
BOOL CUGCnrBtn::ToolTipNeedText( UINT id, NMHDR* pTTTStruct, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(*pResult);

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;

	static CString string;
	int col = -1;
	int row = -1;

	if ( m_ctrl->OnHint(col,row,UG_CORNERBUTTON,&string) == TRUE )
	{
		// Do this to Enable multiline ToolTips
		::SendMessage( pTTT->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX );
		::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX );
		::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_INITIAL, 200 );
		::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_RESHOW, 200 );
		pTTT->lpszText = const_cast<LPTSTR>((LPCTSTR)string);
		return TRUE;
	}

	return FALSE;
}