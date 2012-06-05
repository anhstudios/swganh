/*************************************************************************
				Class Implementation : CUGHScroll
**************************************************************************
	Source file : UGHScrol.cpp
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
CUGHScroll::CUGHScroll()
{
	m_lastMaxLeftCol = -1;
	m_lastNumLockCols = -1;
	m_trackColPos = 0;
}

CUGHScroll::~CUGHScroll()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGHScroll, CScrollBar)
	//{{AFX_MSG_MAP(CUGHScroll)
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
Update
	function forces window update.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGHScroll::Update()
{
	Moved();
}

/***************************************************
Moved
	Scroll bar position changed, redraw the grid.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGHScroll::Moved()
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	//update the range if the max left col has changed
	//or if the number of locked columns has changed
	if(m_lastMaxLeftCol != m_GI->m_maxLeftCol || m_lastNumLockCols != m_GI->m_numLockCols)
	{
		m_lastMaxLeftCol = m_GI->m_maxLeftCol;
		m_lastNumLockCols = m_GI->m_numLockCols;

		//set the scroll range
		SCROLLINFO ScrollInfo;
		ScrollInfo.cbSize	= sizeof(SCROLLINFO);
		ScrollInfo.fMask	= SIF_PAGE | SIF_RANGE;
		ScrollInfo.nPage	= (m_GI->m_gridWidth  - m_GI->m_lockColWidth) / m_GI->m_defColWidth;
		ScrollInfo.nMin		= 0;
		ScrollInfo.nMax		= (m_GI->m_maxLeftCol - m_GI->m_numLockCols) + ScrollInfo.nPage -1;
		SetScrollInfo(&ScrollInfo,FALSE);

		if(m_GI->m_hScrollRect.top == m_GI->m_hScrollRect.bottom)
			m_ctrl->AdjustComponentSizes();
	}

	//set the scroll pos
	if(m_GI->m_lastLeftCol != m_GI->m_leftCol)
	{
		SetScrollPos(m_GI->m_leftCol - m_GI->m_numLockCols,TRUE);
		m_ctrl->OnViewMoved( UG_HSCROLL, (long)m_GI->m_lastLeftCol, (long)m_GI->m_leftCol );
	}
}

/***************************************************
OnHScroll
	The framework calls this member function when the user clicks a window's
	horizontal scroll bar.
Params:
	nSBCode		- please see MSDN for more information on the parameters.
	nPos
	pScrollBar
Returns:
	<none>
*****************************************************/
void CUGHScroll::HScroll(UINT nSBCode, UINT nPos) 
{
	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	m_ctrl->m_GI->m_moveType = 4;

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		m_ctrl->MoveLeftCol(UG_COLRIGHT);
		break;
	case SB_LINEUP:
		m_ctrl->MoveLeftCol(UG_COLLEFT);
		break;
	case SB_PAGEUP:
		m_ctrl->MoveLeftCol(UG_PAGELEFT);
		break;
	case SB_PAGEDOWN:
		m_ctrl->MoveLeftCol(UG_PAGERIGHT);
		break;
	case SB_TOP:
		m_ctrl->MoveLeftCol(UG_LEFT);
		break;
	case SB_BOTTOM:
		m_ctrl->MoveLeftCol(UG_RIGHT);
		break;
	case SB_THUMBTRACK:
		if(m_GI->m_hScrollMode == UG_SCROLLTRACKING)	//tracking
			m_ctrl->SetLeftCol(nPos+m_GI->m_numLockCols);

		m_trackColPos = nPos+m_GI->m_numLockCols;

		//if enabled then show scroll hints
		#ifdef UG_ENABLE_SCROLLHINTS
			if(m_GI->m_enableHScrollHints)
			{
				CString string;
				RECT rect;
				GetWindowRect(&rect);
				rect.left = LOWORD(GetMessagePos());
				m_ctrl->ScreenToClient(&rect);
				m_ctrl->m_CUGHint->SetWindowAlign(UG_ALIGNCENTER|UG_ALIGNBOTTOM);
				m_ctrl->m_CUGHint->SetTextAlign(UG_ALIGNCENTER);

				m_ctrl->OnHScrollHint(m_trackColPos,&string);
				// set text before move window...
				m_ctrl->m_CUGHint->SetText(string,FALSE);

				m_ctrl->m_CUGHint->MoveHintWindow(rect.left,rect.top-1,20);
				m_ctrl->m_CUGHint->Show();				
			}
		#endif // UG_ENABLE_SCROLLHINTS
		break;
	case SB_ENDSCROLL:
		break;
	case SB_THUMBPOSITION:
		#ifdef UG_ENABLE_SCROLLHINTS
		if(m_GI->m_enableHScrollHints)
		{
			m_ctrl->m_CUGHint->Hide();				
		}
		#endif

		m_ctrl->SetLeftCol(nPos+m_GI->m_numLockCols);

		break;
	}
}

/***************************************************
OnRButtonDown
	checks if the popup menus are enabled, if so that it starts process to
	show popup menu.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGHScroll::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_GI->m_enablePopupMenu)
	{
		ClientToScreen(&point);
		m_ctrl->StartMenu(0,0,&point,UG_HSCROLL);
	}
	
	CScrollBar::OnRButtonDown(nFlags, point);
}

/***************************************************
OnCreate
	Please see MSDN for more details on this event handler.
	The CUGHScroll control takes this opportunity to check if the scroll 
	hints are enabled, if so then it enables its tool tips.
Params:
	lpCreateStruct	- Points to a CREATESTRUCT structure
					  that contains information about the
					  CWnd object being created. 
Returns:
	OnCreate must return 0 to continue the creation of the
	CWnd object. If the application returns –1, the window
	will be destroyed.
*****************************************************/
int CUGHScroll::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(m_GI->m_enableHScrollHints)
		EnableToolTips(TRUE);
	
	return 0;
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over horizontal scroll
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Returns:
	Context help ID to be displayed
*************************************************/
LRESULT CUGHScroll::OnHelpHitTest(WPARAM, LPARAM)
{
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col = m_ctrl->GetCurrentCol();
	long row = m_ctrl->GetCurrentRow();
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_HSCROLL );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) over horizontal scroll
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Returns:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGHScroll::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// this message is fired as result of mouse activated help in a dialog
		int col = m_ctrl->GetCurrentCol();
		long row = m_ctrl->GetCurrentRow();
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( col, row, UG_HSCROLL ));
		// prevent further handling of this message
		return TRUE;
	}
	return FALSE;
}