/*************************************************************************
				Class Implementation : CUGTab
**************************************************************************
	Source file : UGTab.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "ugctrl.h"
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
CUGTab::CUGTab()
{
	m_tabCount		= 0;
	m_currentTab	= 0;
	m_tabOffset		= 0;

	m_scrollWidth	= GetSystemMetrics(SM_CXHSCROLL) *2;

	m_defFont.CreateFont(14,0,0,0,500,0,0,0,0,0,0,0,0,_T("Arial"));
	m_font = &m_defFont;

	m_resizeReady	= FALSE;
	m_resizeInProgress = FALSE;

	m_bestWidth		= 150;
}

CUGTab::~CUGTab()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGTab, CWnd)
	//{{AFX_MSG_MAP(CUGTab)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
GetTabItemWidth
	function is used to calculate width of a string as it will be displayed,
	using default DC and current font.
Params:
	string		- the string to measure
Returns:
	width in pixels of the string in questoin.
*****************************************************/
int CUGTab::GetTabItemWidth( CString string )
{
	//get the string width
	CDC* dc = GetDC();
	
	CFont * oldfont = dc->SelectObject(m_font);
	CSize size = dc->GetTextExtent(string);
	dc->SelectObject(oldfont);
	ReleaseDC(dc);

	return size.cx +10;
}

/***************************************************
AddTab
	functoin adds new tab to the tab control, the new tab will be added
	to the end of the display list.
Params:
	string		- caption of the new tab
	ID			- Id of the new tab
Returns:
	UG_SUCCESS	- tab was added successfuly
	UG_ERROR	- tab could not be added
*****************************************************/
int CUGTab::AddTab( CString string, long ID )
{
	if ( m_tabCount < UTMAXTABS )
	{
		m_tabStrings[m_tabCount]	= string;
		m_tabIDs[m_tabCount]		= ID;
		m_tabWidths[m_tabCount]		= GetTabItemWidth( string );
		m_tabTextColors[m_tabCount] = GetSysColor( COLOR_WINDOWTEXT );
		m_tabBackColors[m_tabCount] = GetSysColor( COLOR_BTNFACE );
		m_tabTextHColors[m_tabCount] = RGB(0,0,0);
		m_tabBackHColors[m_tabCount] = RGB(255,255,255);
	
		m_tabCount++;
	
		AdjustScrollBars();
	
		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/***************************************************
InsertTab
	function is similar in function to the AddTab, with only one difference
	it allows to insert new tab at a specified index location.
Params:
	pos			- insert position
	string		- caption of the new tab
	ID			- Id of the new tab
Returns:
	UG_SUCCESS	- tab was added successfuly
	UG_ERROR	- tab could not be added
*****************************************************/
int CUGTab::InsertTab( int pos, CString string, long ID )
{
	if ( pos < 0 || pos > m_tabCount )
		return UG_ERROR;
	if ( pos == m_tabCount )
		return AddTab(string,ID);
	
	if ( m_tabCount < UTMAXTABS )
	{	
		for ( int loop = m_tabCount; loop > pos; loop -- )
		{
			m_tabStrings[loop]		= m_tabStrings[loop-1];
			m_tabIDs[loop]			= m_tabIDs[loop-1];
			m_tabWidths[loop]		= m_tabWidths[loop-1];
			m_tabTextColors[loop]	= m_tabTextColors[loop-1];
			m_tabBackColors[loop]	= m_tabBackColors[loop-1];
			m_tabTextHColors[loop]	= m_tabTextHColors[loop-1];
			m_tabBackHColors[loop]	= m_tabBackHColors[loop-1];
		}
		m_tabCount++;

		if ( m_currentTab >= pos )
			m_currentTab++;

		m_tabStrings[pos]		= string;
		m_tabIDs[pos]			= ID;
		m_tabWidths[pos]		= GetTabItemWidth( string );
		m_tabTextColors[pos]	= GetSysColor( COLOR_WINDOWTEXT );
		m_tabBackColors[pos]	= GetSysColor( COLOR_BTNFACE );
		m_tabTextHColors[pos]	= RGB(0,0,0);
		m_tabBackHColors[pos]	= RGB(255,255,255);

		AdjustScrollBars();
		Invalidate( TRUE );

		return UG_SUCCESS;
	}

	return UG_ERROR;
}

/***************************************************
DeleteTab
	function is used to delete a tab from the tab control.
Params:
	ID			- Id of the tab to delete.
Returns:
	UG_SUCCESS	- tab was successfuly deleted
	UG_ERROR	- tab matching ID was not found
*****************************************************/
int CUGTab::DeleteTab( long ID )
{
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			for ( int loop2 = loop; loop2 < m_tabCount-1;loop2++ )
			{
				m_tabStrings[loop2]		= m_tabStrings[loop2+1];
				m_tabIDs[loop2]			= m_tabIDs[loop2+1];
				m_tabWidths[loop2]		= m_tabWidths[loop2+1];
				m_tabTextColors[loop2]	= m_tabTextColors[loop2+1];
				m_tabBackColors[loop2]	= m_tabBackColors[loop2+1];
				m_tabTextHColors[loop2]	= m_tabTextHColors[loop2+1];
				m_tabBackHColors[loop2]	= m_tabBackHColors[loop2+1];
			}

			m_tabCount--;

			if ( m_currentTab == loop )
				SetCurrentTab(m_tabIDs[0]);
			else if ( m_currentTab > 0 )
				m_currentTab--;

			AdjustScrollBars();
			Invalidate( TRUE );
			
			return UG_SUCCESS;
		}
	}
	return UG_ERROR;
}

/***************************************************
GetTabID
	function is used to retrieve ID information on
	any given tab.  This is needed to perform actions
	on tabs (ie DeleteTab).
Params:
	nIndex	- indicates the index if the tab to locate
Returns:
	returns ID associated with given tab, or 0 if failed.
*****************************************************/
long CUGTab::GetTabID( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_tabCount )
		return m_tabIDs[nIndex];

	// the index provided was is out of bounds, return error
	return -1;
}

/***************************************************
GetTabIndex
	The GetTabIndex function can be used to retrieve 
	the screen location of a given tab.
Params:
	nID		- ID of the tab to retrieve information on
Returns:
	returns ID associated with given tab,
	or -1 if given ID not found
*****************************************************/
int CUGTab::GetTabIndex( long nID )
{
	for ( int nTabIndex = 0; nTabIndex < m_tabCount; nTabIndex++ )
	{
		if ( m_tabIDs[nTabIndex] == nID )
		{
			return nTabIndex;
		}
	}

	return -1;
}

/***************************************************
GetTabCount
	returns current number of tabs.
Params:
	<none>
Returns:
	returns current number of tabs.
*****************************************************/
int CUGTab::GetTabCount()
{
	return m_tabCount;
}

/***************************************************
SetTabLabel
	The SetTabLabel function can be used to change the label
	string of an existing tab.
Params:
	nID		- ID of the tab to search out
	sLabel	- new label
Returns:
	returns UG_SUCCESS if information was set properly
		or UG_ERROR when ID is not found
*****************************************************/
int CUGTab::SetTabLabel( int nID, CString sLabel )
{
	for ( int nTabIndex = 0; nTabIndex < m_tabCount; nTabIndex++ )
	{
		if ( m_tabIDs[nTabIndex] == nID )
		{	// set the new information
			m_tabStrings[nTabIndex]	= sLabel;
			m_tabWidths[nTabIndex]	= GetTabItemWidth( sLabel );
			// redraw the tab window
			AdjustScrollBars();
			Invalidate();
			UpdateWindow();

			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
}

/***************************************************
GetTabLabel
	The GetTabLabel function can be used to retrieve tab's
	label string information based on the tab's ID
Params:
	nID		- ID of the tab to get information on
	sLabel	- reference to a CString object that will receive the information
Returns:
	returns UG_SUCCESS if information was retrieved properly
		or UG_ERROR when ID is not found
*****************************************************/
int CUGTab::GetTabLabel( int nID, CString &sLabel )
{
	for ( int nTabIndex = 0; nTabIndex < m_tabCount; nTabIndex++ )
	{
		if ( m_tabIDs[nTabIndex] == nID )
		{	
			// Set the information user desires to provided object
			sLabel = m_tabStrings[nTabIndex];
			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
}

/***************************************************
GetTabLabel
	function is used to retrieve label string of a tab.
Params:
	nIndex	- index of the tab
Returns:
	returns found label string.
*****************************************************/
CString CUGTab::GetTabLabel( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_tabCount )
		return m_tabStrings[nIndex];

	// the index provided was is out of bounds, return error
	return _T("");
}

/***************************************************
SetCurrentTab
	funciton is used to set new 'current' tab, this can be as a result
	of user's mouse action or programmatic.
Params:
	ID			- Id of the new selected tab
Returns:
	UG_SUCCESS	- if the tab was successfully set
	UG_ERROR	- tab matching the ID was not found
*****************************************************/
int CUGTab::SetCurrentTab( long ID )
{
	int width = -5;

	for ( int loop = 0;loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			m_currentTab = loop;
			if ( width < 0 )
				width = 0;

			if ( width < m_tabOffset )
				m_tabOffset = width;
			else if ( width > ( m_tabOffset + ( m_GI->m_tabWidth - ( m_tabWidths[loop] * 2 ) - 10 )))
				m_tabOffset = width - ( m_GI->m_tabWidth - ( m_tabWidths[loop] * 2 ) -10 );

			if ( width > m_maxTabOffset )
				m_tabOffset = m_maxTabOffset;

			Invalidate( TRUE );
			return UG_SUCCESS;
		}
		width += m_tabWidths[loop];
	}
	return UG_ERROR;
}

/***************************************************
GetCurrentTab
	returns index of currently selected tab.
Params:
	<none>
Returns:
	returns index of currently selected tab.
*****************************************************/
int CUGTab::GetCurrentTab()
{
	return m_currentTab;
}

/***************************************************
AdjustScrollBars
	function is called to recalculate the scrollbar's width
	and current offset position.
Params:
	<none>
Returns:
	UG_SUCCESS	- scrollbar's size successfuly adjusted
	UG_ERROR	- if the tab control does not have any tabs to show
*****************************************************/
int CUGTab::AdjustScrollBars()
{
	if ( m_tabCount < 1 )
		return UG_ERROR;

	//get the total width and setup the scroll bars
	int totalTabWidths = 0;

	for ( int nTabIndex =0; nTabIndex < m_tabCount; nTabIndex++ )
		totalTabWidths += m_tabWidths[nTabIndex];

	CRect rect;
	GetClientRect( rect );
	int width = rect.right - m_scrollWidth;
	int dif = totalTabWidths - width;

	if ( dif > 0 )
	{
		m_maxTabOffset = dif + 20;
	}
	else
	{
		m_maxTabOffset = 0;
	}

	return UG_SUCCESS;
}

/***************************************************
OnTabSizing
	function is called by the OnMouseMove event handler when the user
	is in process of sizing tab window.  This function is used to properly
	adjust grid's component sizes to reflect new tab size.
Params:
	width		- indicates new tab's window width
Returns:
	<none>
*****************************************************/
void CUGTab::OnTabSizing( int width )
{
	CRect rect;
	GetWindowRect( rect );
	GetParent()->ScreenToClient( rect );

	CRect parentRect;
	GetParent()->GetClientRect( parentRect );

	if( width < 40 )
		width = 40;
	
	m_GI->m_tabWidth	= width;
	m_ctrl->m_tabSizing = TRUE;
	m_ctrl->AdjustComponentSizes();
	m_ctrl->m_tabSizing = FALSE;
}

/***************************************************
Update
	function forces window update.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGTab::Update()
{
	InvalidateRect( NULL );
	UpdateWindow();
}

/***************************************************
OnPaint
	Please see MSDN for more details on this event handler.
	The CUGTab control takes this opportunity to propery draw its tabs.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGTab::OnPaint() 
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	CPaintDC dc(this); // device context for painting
	int idDC = dc.SaveDC();	
	CFont * oldfont = dc.SelectObject( m_font );
	CPen whitepen( PS_SOLID, 1, RGB(255,255,255));
	CPen blackpen( PS_SOLID, 1, RGB(0,0,0));
	CPen graypen( PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
	CPen darkgraypen( PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
	CBrush *pBrush = NULL;
	CBrush *pOldBrush = NULL;
	POINT points[4];
	CRect currentRect;
	CRect rect;
	CRect clientRect;

	GetClientRect( rect );
	GetClientRect( clientRect );
	// prepare the bacground area
	CBrush brush( GetSysColor( COLOR_BTNFACE ));
	dc.FillRect( rect, &brush );

	rect.left = m_scrollWidth - m_tabOffset;
	
	rect.bottom --;
	rect.right --;

	//draw each tab
	for ( int nTabIndex = 0; nTabIndex < m_tabCount; nTabIndex++ )
	{	
		rect.right = rect.left + m_tabWidths[nTabIndex];

		if ( nTabIndex != m_currentTab )
		{
			pBrush = new CBrush( m_tabBackColors[nTabIndex] );
			pOldBrush = dc.SelectObject( pBrush );
			dc.SetTextColor( m_tabTextColors[nTabIndex] );
			dc.SetBkMode( TRANSPARENT );
			//draw the tab	
			points[0].x = rect.left;
			points[0].y = rect.top;
			points[1].x = rect.left + 7;
			points[1].y = rect.bottom;
			points[2].x = rect.right + 2;
			points[2].y = rect.bottom;
			points[3].x = rect.right + 9;
			points[3].y = rect.top;
			dc.Polygon( points, 4 );
			// clean up allocated brush
			dc.SelectObject( pOldBrush );
			delete pBrush;
			pBrush = NULL;
			// Draw tab's text
			rect.left += 10;
			rect.top ++;
			dc.ExtTextOut( rect.left, rect.top, 0, rect, m_tabStrings[nTabIndex], NULL );	
			rect.left -= 10;
			rect.top --;
		}
		else
			CopyRect( currentRect, rect );

		rect.left = rect.right;
	}

	// Draw the current tab
	pBrush = new CBrush( m_tabBackHColors[m_currentTab] );
	pOldBrush = dc.SelectObject( pBrush );
	dc.SetTextColor( m_tabTextHColors[m_currentTab] );
	dc.SetBkMode( TRANSPARENT );
	// restore the rect of the current tab
	CopyRect( rect, currentRect );
	//draw the tab	
	points[0].x = rect.left;
	points[0].y = rect.top;
	points[1].x = rect.left + 7;
	points[1].y = rect.bottom;
	points[2].x = rect.right + 2;
	points[2].y = rect.bottom;
	points[3].x = rect.right + 9;
	points[3].y = rect.top;
	dc.Polygon( points, 4 );
	// blank out the top line
	CPen *pPen = new CPen( PS_SOLID, 1, m_tabBackHColors[m_currentTab] );
	CPen *pOldPen = dc.SelectObject( pPen );
	dc.MoveTo( rect.left + 1, rect.top );
	dc.LineTo( rect.right + 9, rect.top );
	// clean up the allocated brush and pen
	dc.SelectObject( pOldBrush );
	delete pBrush;
	dc.SelectObject( pOldPen );
	delete pPen;
	// Draw tab's text
	rect.left += 10;
	rect.top ++;
	dc.ExtTextOut( rect.left, rect.top, 0, rect, m_tabStrings[m_currentTab], NULL );	

	// Draw the sizing bar
	CopyRect( rect, clientRect );
	rect.right --;
	dc.SelectObject( &blackpen );
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );
	dc.SelectObject( &darkgraypen );
	rect.right --;
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );
	dc.SelectObject( &graypen );
	rect.right --;
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );
	rect.right --;
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );
	dc.SelectObject( &whitepen );
	rect.right --;
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );
	dc.SelectObject( &graypen );
	rect.right --;
	dc.MoveTo( rect.right, rect.top );
	dc.LineTo( rect.right, rect.bottom );

	// Force the scroll buttons to update
	m_scroll.Invalidate();
	m_scroll.UpdateWindow();
	// Draw the black line over the scroll buttons
	dc.SelectObject( &blackpen );
	dc.MoveTo( rect.left, rect.top );
	dc.LineTo( rect.left + m_scrollWidth + 1, rect.top );
	// clean up
	dc.SelectObject( oldfont );
	dc.RestoreDC( idDC );
}

/***************************************************
OnCreate
	Please see MSDN for more details on this event handler.
	The CUGTab control takes this opportunity to create, size and position
	its scrollbar.
Params:
	lpCreateStruct	- Points to a CREATESTRUCT structure
					  that contains information about the
					  CWnd object being created. 
Returns:
	OnCreate must return 0 to continue the creation of the
	CWnd object. If the application returns –1, the window
	will be destroyed.
*****************************************************/
int CUGTab::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	
	// determine default size of the scrollbar
	CRect rect;
	GetClientRect( rect );
	rect.right = m_scrollWidth;

	// create the scrollbar
	m_scroll.Create( WS_CHILD|WS_VISIBLE, rect, this, UTABSCROLLID );
	
	return 0;
}

/***************************************************
OnLButtonDown
	1.	determines which tab user clicked on, it one is found then OnTabSelected
		notification is fired.
	2.	if user clicks the button over the area that allows him to resize tab
		window, then the mouse capture and resize-in-progress flag are set.
		The resizing functionality will be completed in OnMouseMove handler.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGTab::OnLButtonDown( UINT nFlags, CPoint point ) 
{
	//check to see if the tabs are to be resized
	if ( m_resizeReady )
	{
		SetCapture();
		m_resizeInProgress = TRUE;
	}
	//find the tab that the mouse is over
	else
	{
		int left = m_scrollWidth - m_tabOffset;
		
		for ( int nTabIndex =0; nTabIndex < m_tabCount; nTabIndex++ )
		{	
			int right = left + m_tabWidths[nTabIndex];
			
			//check to see if the mouse is over a given tab
			if ( point.x >= left && point.x <= ( right + 2 ))
			{	
				m_currentTab = nTabIndex;

				//check to make sure that the tab is fully visible
				CRect rect;
				GetClientRect( rect );
				left -= 5;
				if ( left < m_scrollWidth )
				{
					m_tabOffset -= ( m_scrollWidth - left );
					if ( m_tabOffset < 0 )
						m_tabOffset = 0;
				}
				right += 19;
				if ( right > rect.right )
				{
					m_tabOffset += ( right - rect.right );
					if ( m_tabOffset > m_maxTabOffset )
						m_tabOffset = m_maxTabOffset;
				}

				Invalidate( TRUE );
				
				//send a notify
				m_ctrl->OnTabSelected( m_tabIDs[nTabIndex] );
			}

			left = right;
		}
	}
	CWnd::OnLButtonDown( nFlags, point );
}

/***************************************************
OnSize
	The framework calls this member function after the window’s size has
	changed.  The CUGTab takes this opportunity to re-position its scrollbar.
Params:
	nType		- please see MSDN for more information on the parameters.
	cx
	cy
Returns:
	<none>
*****************************************************/
void CUGTab::OnSize( UINT nType, int cx, int cy ) 
{
	CWnd::OnSize( nType, cx, cy );
	
	CRect rect;
	GetClientRect( rect );
	rect.right = m_scrollWidth;
	rect.top++;
	m_scroll.MoveWindow( rect, TRUE );

	AdjustScrollBars();
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
void CUGTab::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) 
{
	if ( nSBCode == SB_LINEUP )
	{
		if ( m_tabOffset > 0 )
		{
			m_tabOffset -= 5;
			Invalidate( TRUE );
		}
	}
	else if( nSBCode == SB_LINEDOWN )
	{
		if( m_tabOffset < m_maxTabOffset - 5 )
		{
			m_tabOffset += 5;
			Invalidate( TRUE );
		}
	}

	CWnd::OnHScroll( nSBCode, nPos, pScrollBar );
}

/***************************************************
PreCreateWindow
	The Utlimate Grid overwrites this function to further customize the
	creation of the tab window.
Params:
	cs			- please see MSDN for more information on the parameters.
Returns:
	Nonzero if the window creation should continue; 0 to indicate creation failure.
*****************************************************/
BOOL CUGTab::PreCreateWindow( CREATESTRUCT& cs ) 
{
	cs.style |= WS_CLIPCHILDREN;	
	return CWnd::PreCreateWindow( cs );
}

/***************************************************
OnMouseMove
	event handler is used during the resize process of the tab window.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*****************************************************/
void CUGTab::OnMouseMove( UINT nFlags, CPoint point ) 
{
	UNREFERENCED_PARAMETER( nFlags );
	
	//resize in progress
	if ( m_resizeInProgress )
	{
		OnTabSizing( point.x );
	}
	//check for resize position
	else
	{
		CRect rect;
		GetClientRect( rect );
		if ( point.x > rect.right - 7 )
		{
			if ( !m_resizeReady )
			{
			  	m_resizeReady = TRUE;
			}
		}
		else if ( m_resizeReady )
		{
		  	m_resizeReady = FALSE;
		}
	}
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
BOOL CUGTab::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message ) 
{
	UNREFERENCED_PARAMETER( *pWnd );
	UNREFERENCED_PARAMETER( nHitTest );
	UNREFERENCED_PARAMETER( message );

	if ( m_resizeReady )
		SetCursor( m_GI->m_WEResizseCursor );
	else
		SetCursor( LoadCursor( NULL, IDC_ARROW ));

	return TRUE;
}

/***************************************************
OnLButtonUp
	Releases the mouse capture, and resets the flag indicating resize.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGTab::OnLButtonUp( UINT nFlags, CPoint point ) 
{
	UNREFERENCED_PARAMETER( nFlags );
	UNREFERENCED_PARAMETER( point );

	ReleaseCapture();
	m_resizeInProgress = FALSE;
}

/***************************************************
OnRButtonDown
	function checks to see if menus are enabled. If so then menu specific
	notification is sent to the CUGCtrl or derived object.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*****************************************************/
void CUGTab::OnRButtonDown( UINT nFlags, CPoint point ) 
{
	UNREFERENCED_PARAMETER( nFlags );

	if ( m_GI->m_enablePopupMenu )
	{
		int tab = -1;
		//find the tab the mouse is over
		int left = m_scrollWidth - m_tabOffset;
		
		for ( int tabIndex = 0; tabIndex < m_tabCount; tabIndex++ )
		{	
			int right = left + m_tabWidths[tabIndex];
			
			//check to see if the mouse is over a given tab
			if ( point.x >= left && point.x <= ( right+2 ))
			{
				tab = tabIndex;
				break;
			}
			left = right;
		}

		ClientToScreen( &point );
		m_ctrl->StartMenu( tab, 0, &point, UG_TAB );
	}
}

/***************************************************
SetTabBackColor
	function is used to set new back color for specified item.
Params:
	ID			- Id of the tab to change
	color		- RGB value of the color to set
Returns:
	UG_SUCCESS	- the color was successfuly chaged
	UG_ERROR	- item ID not found
*****************************************************/
int CUGTab::SetTabBackColor( long ID,COLORREF color )
{
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			m_tabBackColors[loop] = color;
			return UG_SUCCESS;
		}
	}
	return UG_ERROR;
}

/***************************************************
SetTabTextColor
	function is used to set new text color for specified item.
Params:
	ID			- Id of the tab to change
	color		- RGB value of the color to set
Returns:
	UG_SUCCESS	- the color was successfuly chaged
	UG_ERROR	- item ID not found
*****************************************************/
int CUGTab::SetTabTextColor( long ID, COLORREF color )
{
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			m_tabTextColors[loop] = color;
			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
}

/***************************************************
SetTabHBackColor
	function is used to set new selected back color
	for specified item.
Params:
	ID			- Id of the tab to change
	color		- RGB value of the color to set
Returns:
	UG_SUCCESS	- the color was successfuly chaged
	UG_ERROR	- item ID not found
*****************************************************/
int CUGTab::SetTabHBackColor( long ID, COLORREF color )
{
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			m_tabBackHColors[loop] = color;
			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
}

/***************************************************
SetTabHTextColor
	function is used to set new selected text color
	for specified item.
Params:
	ID			- Id of the tab to change
	color		- RGB value of the color to set
Returns:
	UG_SUCCESS	- the color was successfuly chaged
	UG_ERROR	- item ID not found
*****************************************************/
int CUGTab::SetTabHTextColor( long ID, COLORREF color )
{
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		if ( m_tabIDs[loop] == ID )
		{
			m_tabTextHColors[loop] = color;
			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
}

/***************************************************
SetTabFont
	function is used to provide new font object to draw text on all tabs,
	this function will also re-calculate widths of all tabs to reflect
	new font set.
Params:
	font		- pointer to CFont object to use
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGTab::SetTabFont( CFont * font )
{
	if ( font != NULL )
		m_font = font;
	
	for ( int loop = 0; loop < m_tabCount; loop++ )
	{
		m_tabWidths[loop] = GetTabItemWidth( m_tabStrings[loop] );
	}

	AdjustScrollBars();
	Invalidate( TRUE );

	return UG_SUCCESS;
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over tab area
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Returns:
	Context help ID to be displayed
*************************************************/
LRESULT CUGTab::OnHelpHitTest( WPARAM, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col = m_ctrl->GetCurrentCol();
	long row = m_ctrl->GetCurrentRow();
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_TAB );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) over tab area
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Returns:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGTab::OnHelpInfo( HELPINFO* pHelpInfo ) 
{
	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
	{
		// this message is fired as result of mouse activated help in a dialog
		int col = m_ctrl->GetCurrentCol();
		long row = m_ctrl->GetCurrentRow();
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( col, row, UG_TAB ));
		// prevent further handling of this message
		return TRUE;
	}
	return FALSE;
}