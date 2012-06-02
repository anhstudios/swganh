/*************************************************************************
				Class Implementation : CUGVScroll
**************************************************************************
	Source file : UGVScrol.cpp
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
CUGVScroll::CUGVScroll()
{
	m_lastMaxTopRow = -2;
	m_lastScrollMode = -2;
	m_lastNumLockRow = -2;
	m_trackRowPos = 0;
}

CUGVScroll::~CUGVScroll()
{
}

/***************************************************
	Message handlers
***************************************************/
BEGIN_MESSAGE_MAP(CUGVScroll, CScrollBar)
	//{{AFX_MSG_MAP(CUGVScroll)
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUGVScroll message handlers

/***************************************************
Update
	function forces window update.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGVScroll::Update()
{
	Moved();
}

/***************************************************
Moved
	Updates vertical scrolbar's position and redraws the grid.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGVScroll::Moved()
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	BOOL bScrolled = FALSE;

	//set the scroll range
	if(	m_lastMaxTopRow != m_GI->m_maxTopRow ||
		m_lastScrollMode != m_GI->m_vScrollMode ||
		m_lastNumLockRow != m_GI->m_numLockRows)
	{
		//set the last value vars
		m_lastMaxTopRow = m_GI->m_maxTopRow;
		m_lastScrollMode = m_GI->m_vScrollMode;
		m_lastNumLockRow = m_GI->m_numLockRows;

		//set up the scrollbar if the number of rows is less than 1000
		if(m_GI->m_maxTopRow <=1000)
		{
			if(UG_SCROLLJOYSTICK == m_GI->m_vScrollMode)
			{
				SCROLLINFO ScrollInfo;
				ScrollInfo.cbSize =sizeof(SCROLLINFO);
				ScrollInfo.fMask = SIF_PAGE | SIF_RANGE |SIF_POS;
				ScrollInfo.nPage = 0;
				ScrollInfo.nMin = 0;
				ScrollInfo.nMax = 2;
				ScrollInfo.nPos = 1;
				SetScrollInfo(&ScrollInfo,FALSE);
				Invalidate();
			}
			else
			{
				SCROLLINFO ScrollInfo;

				ScrollInfo.cbSize =sizeof(SCROLLINFO);
				ScrollInfo.fMask = SIF_PAGE | SIF_RANGE;

				if( m_GI->m_defRowHeight < 1 )
					m_GI->m_defRowHeight =  1;

				ScrollInfo.nPage = m_GI->m_gridHeight / m_GI->m_defRowHeight;

				ScrollInfo.nMin = m_GI->m_numLockRows;
				ScrollInfo.nMax = m_GI->m_maxTopRow + ScrollInfo.nPage -1;
				SetScrollInfo(&ScrollInfo,FALSE);
				Invalidate();
			}
			bScrolled = TRUE;
			m_multiRange = 1;
			m_multiPos	= 1;			
		}
		//set up the scrollbar if the number of rows is greater than 1000
		else
		{
			m_multiRange = (double)1000 / (double)m_GI->m_maxTopRow;
			m_multiPos   = (double)m_GI->m_maxTopRow / (double)1000;
			if(UG_SCROLLJOYSTICK==m_GI->m_vScrollMode)
			{
				SetScrollRange(0,2,FALSE);
				m_multiRange = 1;
				m_multiPos	= 1;
			}
			else
			{
				SCROLLINFO ScrollInfo;
				ScrollInfo.cbSize =sizeof(SCROLLINFO);
				ScrollInfo.fMask = SIF_PAGE | SIF_RANGE;
				ScrollInfo.nPage = 1;
				ScrollInfo.nMin = 0;//m_GI->m_numLockRows;
				ScrollInfo.nMax = 1000;
				SetScrollInfo(&ScrollInfo,FALSE);
			}
			bScrolled = TRUE;
		}
	}

	//set the scroll pos
	if( m_GI->m_lastTopRow != m_GI->m_topRow || bScrolled == TRUE )
	{
		if(UG_SCROLLJOYSTICK == m_GI->m_vScrollMode)
			SetScrollPos(1,TRUE);
		else
			SetScrollPos((int)(m_GI->m_topRow * m_multiRange),TRUE);

		m_ctrl->OnViewMoved( UG_VSCROLL, m_GI->m_lastTopRow, m_GI->m_topRow );
	}
}

/***************************************************
VScroll
	The framework calls this member function when the user clicks a window's
	vertical scroll bar.
Params:
	nSBCode		- please see MSDN for more information on the parameters.
	nPos
	pScrollBar
Returns:
	<none>
*****************************************************/
void CUGVScroll::VScroll(UINT nSBCode, UINT nPos) 
{
	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	m_ctrl->m_GI->m_moveType = 4;

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		m_ctrl->MoveTopRow(UG_LINEDOWN);
		break;
	case SB_LINEUP:
		m_ctrl->MoveTopRow(UG_LINEUP);
		break;
	case SB_PAGEUP:
		m_ctrl->MoveTopRow(UG_PAGEUP);
		break;
	case SB_PAGEDOWN:
		m_ctrl->MoveTopRow(UG_PAGEDOWN);
		break;
	case SB_TOP:
		m_ctrl->MoveTopRow(UG_TOP);
		break;
	case SB_BOTTOM:
		m_ctrl->MoveTopRow(UG_BOTTOM);
		break;
	case SB_THUMBTRACK:
		if(m_GI->m_vScrollMode==UG_SCROLLTRACKING)	//tracking
			m_ctrl->SetTopRow((long)((double)nPos * m_multiPos));

		m_trackRowPos = (long)((double)nPos * m_multiPos) + m_GI->m_numLockRows;

		//scroll hint window
		#ifdef UG_ENABLE_SCROLLHINTS
			if(m_GI->m_enableVScrollHints)
			{
				CString string;
				RECT rect;
				GetWindowRect(&rect);
				rect.top = HIWORD(GetMessagePos());
				m_ctrl->ScreenToClient(&rect);
				m_ctrl->m_CUGHint->SetWindowAlign(UG_ALIGNRIGHT|UG_ALIGNVCENTER);
				m_ctrl->m_CUGHint->SetTextAlign(UG_ALIGNCENTER);

				m_ctrl->OnVScrollHint(m_trackRowPos,&string);
				m_ctrl->m_CUGHint->SetText(string,FALSE);
				//TD - set text before move window...
				m_ctrl->m_CUGHint->MoveHintWindow(rect.left-1,rect.top,40);
				m_ctrl->m_CUGHint->Show();				
			}
		#endif
		break;
	case SB_ENDSCROLL:
		break;
	case SB_THUMBPOSITION:
		#ifdef UG_ENABLE_SCROLLHINTS
			if(m_GI->m_enableVScrollHints)
				m_ctrl->m_CUGHint->Hide();				
		#endif

		m_ctrl->SetTopRow((long)((double)nPos * m_multiPos));

		break;
	}

	m_ctrl->m_CUGSideHdg->Invalidate();
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
void CUGVScroll::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_GI->m_enablePopupMenu)
	{
		ClientToScreen(&point);
		m_ctrl->StartMenu(0,0,&point,UG_VSCROLL);
	}
	
	CScrollBar::OnRButtonDown(nFlags, point);
}

/***************************************************
OnCreate
	Please see MSDN for more details on this event handler.
	The CUGVScroll control takes this opportunity to check if the scroll 
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
int CUGVScroll::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(m_GI->m_enableVScrollHints)
		EnableToolTips(TRUE);
	
	return 0;
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over vertical scroll
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Returns:
	Context help ID to be displayed
*************************************************/
LRESULT CUGVScroll::OnHelpHitTest(WPARAM, LPARAM)
{
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col = m_ctrl->GetCurrentCol();
	long row = m_ctrl->GetCurrentRow();
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_VSCROLL );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) over vertical scroll
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Returns:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGVScroll::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// this message is fired as result of mouse activated help in a dialog
		int col = m_ctrl->GetCurrentCol();
		long row = m_ctrl->GetCurrentRow();
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( col, row, UG_VSCROLL ));
		// prevent further handling of this message
		return TRUE;
	}
	return FALSE;
}