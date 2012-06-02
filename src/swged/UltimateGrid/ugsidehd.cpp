/*************************************************************************
				Class Implementation : CUGSideHdg
**************************************************************************
	Source file : UGSideHd.cpp
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
CUGSideHdg::CUGSideHdg()
{
	//init the varialbes
	m_isSizing	= FALSE;
	m_canSize	= FALSE;

	//set the row height focus rect to be offscreen
	m_focusRect.top = -1;
	m_focusRect.bottom = -1;
}

CUGSideHdg::~CUGSideHdg()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGSideHdg, CWnd)
	//{{AFX_MSG_MAP(CUGSideHdg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, ToolTipNeedText )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
OnPaint
	This routine is responsible for gathering information on cells to draw,
	and draw in an optomized fashion.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGSideHdg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	m_drawHint.AddHint(m_GI->m_numberSideHdgCols * -1,0,0,m_GI->m_numberRows);

	DrawCellsIntern(&dc);
	
	m_drawHint.ClearHints();
}

/***************************************************
DrawCellsIntern
	function is the key to the fast redraw functionality in the Ultimate Grid.
	This function is responsible for drawing cells within the grid area, it
	makes sure that only the cells that are marked as invalid
	(by calling CUGDrawHint::IsInvalid function) are redrawn.
Params:
	dc		- pointer DC to draw on
Returns:
	<none>
*****************************************************/
void CUGSideHdg::DrawCellsIntern(CDC *dc)
{
	CRect rect(0,0,0,0), cellRect;
	CUGCell cell;
	CUGCellType * cellType;
	int dcID;

	int xIndex, col;
	long yIndex, row = 0;

	m_bottomRow = m_GI->m_numberRows;

	m_ctrl->OnScreenDCSetup(dc,NULL,UG_SIDEHEADING);

	if(m_GI->m_defFont != NULL)
		dc->SelectObject((CFont*)m_GI->m_defFont);

	for(yIndex = 0; yIndex <m_GI->m_numberRows; yIndex++)
	{
		if(yIndex == m_GI->m_numLockRows)
			yIndex = m_GI->m_topRow;

		row = yIndex;

		rect.top = rect.bottom;
		rect.bottom += m_ctrl->GetRowHeight(row);
		rect.right = 0;

		if(rect.top == rect.bottom)
			continue;

		for(xIndex = (m_GI->m_numberSideHdgCols*-1) ;xIndex <0;xIndex++)
		{	
			col = xIndex;
			row = yIndex;

			rect.left = rect.right;
			rect.right  += GetSHColWidth(col);

			//draw if invalid
			if(m_drawHint.IsInvalid(col,row) != FALSE)
			{
				CopyRect(&cellRect,&rect);

				m_ctrl->GetCellIndirect(col,row,&cell);

				if(cell.IsPropertySet(UGCELL_JOIN_SET))
				{
					GetCellRect(col,row,&cellRect);
					m_ctrl->GetJoinStartCell(&col,&row,&cell);
					if(m_drawHint.IsValid(col,row))
						continue;
					m_drawHint.SetAsValid(col,row);
				}

				dcID = dc->SaveDC();
				cellType = m_ctrl->GetCellType(cell.GetCellType());
				cellType->OnDraw(dc,&cellRect,col,row,&cell,0,0);

				dc->RestoreDC(dcID);
			}
		}
		if(rect.bottom > m_GI->m_gridHeight)
			break;
	}

	m_bottomRow = row;

	if(rect.bottom  < m_GI->m_gridHeight)
	{
		rect.top = rect.bottom;
		rect.bottom = m_GI->m_gridHeight;
		rect.left = 0;
		rect.right = m_GI->m_sideHdgWidth;
		// fill-in the area that is not covered by cells
		CBrush brush( m_ctrl->OnGetDefBackColor( UG_SIDEHEADING ));
		dc->FillRect( rect, &brush );
	}
}

/************************************************
Update 
	function makes sure that the last column in the side heading has
	proper width before the window is redrawn.
Params:
	<none>
Returns:
	<none>
*************************************************/
void CUGSideHdg::Update()
{
	//calc the last col width
	int width = 0;

	// For VC6/2002/2003/2005 compatibility
	int xIndex = -1;

	for(;xIndex > (m_GI->m_numberSideHdgCols*-1);xIndex--)
	{
		width += GetSHColWidth(xIndex);
	}

	width = m_GI->m_sideHdgWidth - width;

	if( width < 0 )
		width = 0;

	m_ctrl->SetSH_ColWidth(xIndex,width);

	//redraw the window
	InvalidateRect(NULL);
	UpdateWindow();
}

/************************************************
Moved
	function makes sure that the grid view has moved vertically
	before the window update is allowed.  With this check the Ultimate
	Grid eliminates un-necessary redraws.
Params:
	<none>
Returns:
	<none>
*************************************************/
void CUGSideHdg::Moved()
{
	if(m_GI->m_topRow == m_GI->m_lastTopRow && m_GI->m_currentRow == m_GI->m_lastRow)
		return;

	if(GetUpdateRect(NULL))
	{
		UpdateWindow();
		return;
	}

	int yScroll = 0;
	long yIndex;
	CDC * pDC;
	BOOL redrawAll = FALSE;

	//if the top row has changed then calc the scroll distance
	if(m_GI->m_topRow != m_GI->m_lastTopRow)
	{
		if(m_GI->m_topRow > m_GI->m_lastTopRow)
		{
			for(yIndex = m_GI->m_lastTopRow;yIndex < m_GI->m_topRow; yIndex++)
			{
				yScroll += m_ctrl->GetRowHeight(yIndex);
				if(yScroll > m_GI->m_gridHeight)
				{
					redrawAll = TRUE;
					break;
				}
			}
		}
		else
		{
			for(yIndex = m_GI->m_topRow;yIndex < m_GI->m_lastTopRow; yIndex++)
			{
				yScroll -= m_ctrl->GetRowHeight(yIndex);
				if(yScroll < -m_GI->m_gridHeight)
				{
					redrawAll = TRUE;
					break;
				}
			}
		}
		if(redrawAll)
		{
			yScroll = 0;
			//redraw the whole heading
			m_drawHint.AddToHint(m_GI->m_numberSideHdgCols * -1,0,0,m_GI->m_numberRows);
		}
	}

	//create the device context
	pDC = GetDC();

	//do scrolling of the window here
	if(yScroll != 0)
	{
		RECT scrollRect;
		GetClientRect(&scrollRect);
		scrollRect.top += m_GI->m_lockRowHeight;
		pDC->ScrollDC(0,-yScroll,&scrollRect,&scrollRect,NULL,NULL);
		
		//add the draw hints for the area the was uncovered by the scroll
		if(yScroll >0)
			m_drawHint.AddToHint(m_GI->m_numberSideHdgCols * -1,m_bottomRow,0,m_GI->m_numberRows);
		else if(yScroll < 0)
			m_drawHint.AddToHint(m_GI->m_numberSideHdgCols * -1,0,0,m_GI->m_lastTopRow);
	}

	//add the last and current cells 
	m_drawHint.AddHint(m_GI->m_numberSideHdgCols * -1,m_GI->m_lastRow,0,m_GI->m_lastRow);
	m_drawHint.AddHint(m_GI->m_numberSideHdgCols * -1,m_GI->m_currentRow,0,m_GI->m_currentRow);
	//call the grids main drawing routine
	DrawCellsIntern(pDC);
	//clear the draw hints
	m_drawHint.ClearHints();

	//close the device context
	ReleaseDC(pDC);
}

/************************************************
OnLButtonDown
	Finds the cell that was clicked in, sends a notification
	updates the current cells position.
	It also sets mouse capture, which will be used with sizing operation.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int col;
	long row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	if(m_canSize)
	{
		m_isSizing = TRUE;
		SetCapture();
	}
	else if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,1,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnSH_LClicked(col,row,1,&rect,&point,processed);
	}
}

/************************************************
OnLButtonUp
	Finds the cell that was clicked in, and sends a notification to 
	CUCtrl::OnSH_LClicked.  It also releases the mouse capture.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int col;
	long row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(m_isSizing)
	{
		m_isSizing = FALSE;
		m_focusRect.top = -1;
		m_focusRect.bottom = -1;		
		ReleaseCapture();

		if(m_colOrRowSizing == 0)	//col sizing
		{
			m_ctrl->OnSideHdgSized(&m_GI->m_sideHdgWidth);
		}
		else
		{
			int height = m_ctrl->GetRowHeight(m_sizingColRow);
			// send the resize notification to all visible cells in this row
			for(int nIndex=m_ctrl->GetLeftCol(); nIndex<m_ctrl->GetRightCol(); nIndex++)
			{
				CUGCellType* pCellType=m_ctrl->GetCellType(nIndex,m_sizingColRow);
				if(pCellType!=NULL)
				{
					pCellType->OnChangedCellHeight(nIndex,m_sizingColRow,&height);
				}
			}
			m_ctrl->OnRowSized(m_sizingColRow,&height);
			m_ctrl->SetRowHeight(m_sizingColRow,height);
		}
		m_ctrl->AdjustComponentSizes();

	}
	else if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnSH_LClicked(col,row,0,&rect,&point,processed);
	}	
}

/************************************************
OnLButtonDblClk
	Finds the cell that was clicked in and sends a notification to
	CUCtrl::OnSH_DClicked.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int col;
	long row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnSH_DClicked(col,row,&rect,&point,processed);
	}
}

/************************************************
OnMouseMove
	Checks to see if the mouse is over a cell separation line, if it is then it
	checks to see if sizing is allowed.  If it is then the cusor is changed to
	the sizing cusror.
	If sizing is in progress then row height or column width within the heading are
	updated.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnMouseMove(UINT nFlags, CPoint point)
{
	//check to see ifthe mouse is over a cell separation
	//but only if the mouse is not currently sizing 
	if(m_isSizing == FALSE && (nFlags&MK_LBUTTON) == 0 && m_GI->m_userSizingMode >0)
	{
		m_canSize = FALSE;

		//side heading column width sizing
		int width = m_GI->m_sideHdgWidth;
		for(int col = 0; col < m_GI->m_numberSideHdgCols ;col++)
		{
			if(point.x < width+3 && point.x > width-3)
			{
				if(m_ctrl->OnCanSizeSideHdg() == FALSE)
					return;

				m_canSize = TRUE;
				m_colOrRowSizing = 0;
				m_sizingColRow = col;
				m_sizingStartSize = m_GI->m_sideHdgWidths[col];
				m_sizingStartPos = point.x;
				m_sizingStartWidth = m_GI->m_sideHdgWidth;

				SetCursor(m_GI->m_WEResizseCursor);
				return;
			}
			width -= m_GI->m_sideHdgWidths[col];
		}

		//side heading row height sizing
		int height = 0;
		for(long row = 0;row < m_GI->m_numberRows;row++)
		{
			if(row == m_GI->m_numLockRows)
				row = m_GI->m_topRow;

			height += m_ctrl->GetRowHeight(row);
			if(height > m_GI->m_gridHeight)
				break;

			if(point.y < height+3 && point.y > height-3)
			{
				if(m_ctrl->GetRowHeight(row+1) == 0 && (row+1) < m_GI->m_numberRows)
					row++;

				if(m_ctrl->OnCanSizeRow(row) == FALSE)
					return;

				m_canSize = TRUE;
				m_colOrRowSizing = 1;
				m_sizingColRow = row;
				m_sizingStartSize = m_ctrl->GetRowHeight(row);
				m_sizingStartPos = point.y;
				if(m_GI->m_uniformRowHeightFlag)
				{
					if( 0 != m_GI->m_defRowHeight )
						m_sizingNumRowsDown =  point.y / m_GI->m_defRowHeight;
				}	
				SetCursor(m_GI->m_NSResizseCursor);
				return;
			}
		}

	}

	//perform the sizing
	else if(m_isSizing)
	{
		if(m_colOrRowSizing == 0)	//col sizing
		{
			int width = m_sizingStartSize +( point.x - m_sizingStartPos);
			if(width <0)
				width = 0;
			m_GI->m_sideHdgWidths[m_sizingColRow] = width;
			width = m_sizingStartWidth + (point.x - m_sizingStartPos);
			if(width <0)
				width = 0;

			m_ctrl->OnSideHdgSizing(&width);

			m_GI->m_sideHdgWidth = width;
			m_ctrl->AdjustComponentSizes();

		}
		else					//row sizing
		{
			int height;
			if(m_GI->m_uniformRowHeightFlag)
			{
				if(point.y <1)
					point.y = 1;
				if(m_sizingNumRowsDown>0)
					height = point.y / m_sizingNumRowsDown;
				else
					height = point.y;
			}
			else
				height = m_sizingStartSize+(point.y - m_sizingStartPos);

			if(height < 0)
				height = 0 ;

			// send the resize notification to all visible cells in this row
			for(int nIndex=m_ctrl->GetLeftCol(); nIndex<m_ctrl->GetRightCol(); nIndex++)
			{
				CUGCellType* pCellType=m_ctrl->GetCellType(nIndex,m_sizingColRow);
				if(pCellType!=NULL)
				{
					pCellType->OnChangingCellHeight(nIndex,m_sizingColRow,&height);
				}
			}

			m_ctrl->OnRowSizing(m_sizingColRow,&height);

			m_ctrl->SetRowHeight(m_sizingColRow,height);

			if(m_GI->m_userSizingMode == 1) // focus rect
			{
				Update();
				CDC* dc = m_ctrl->m_CUGGrid->GetDC();
				dc->DrawFocusRect(&m_focusRect);
				m_focusRect.top = point.y-1;
				m_focusRect.bottom = point.y+1;
				m_focusRect.left = 0;
				m_focusRect.right = m_GI->m_gridWidth;
				dc->DrawFocusRect(&m_focusRect);
				m_ctrl->m_CUGGrid->ReleaseDC(dc);

			}
			else
				m_ctrl->RedrawAll();
		}
	}
}

/************************************************
OnRButtonDown
	function sends a mouse click notification to the main grid class, 
	checks to see if menus are enabled. If so then menu specific
	notifications are sent as well.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnRButtonDown(UINT nFlags, CPoint point)
{
	int col;
	long row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnSH_RClicked(col,row,1,&rect,&point,processed);
	}

	if(m_GI->m_enablePopupMenu)
	{
		ClientToScreen(&point);
		m_ctrl->StartMenu(col,row,&point,UG_SIDEHEADING);
	}
}

/************************************************
OnRButtonUp
	Sends a mouse click notification to the main grid class.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Returns:
	<none>
*************************************************/
void CUGSideHdg::OnRButtonUp(UINT nFlags, CPoint point)
{
	int col;
	long row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnSH_RClicked(col,row,0,&rect,&point,processed);
	}
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
BOOL CUGSideHdg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(*pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	if(!m_canSize)
		SetCursor(m_GI->m_arrowCursor);
	else if(m_colOrRowSizing == 0)
		SetCursor(m_GI->m_WEResizseCursor);
	else
		SetCursor(m_GI->m_NSResizseCursor);
		
	return 1;
}

/************************************************
GetCellRect
	Returns the rectangle for the given cell co-ordinates.
	If the cell is joined then the given co-ordinates 
	are modified to point to the start cell for the join.
Params:
	col		- column to find the rect of
	row		- row to find the rect of
	rect	- rectangle to calculate
Returns:
	UG_SUCCESS	- success
*************************************************/
int CUGSideHdg::GetCellRect(int col,long row,RECT *rect)
{
	return GetCellRect(&col,&row,rect);

}
/************************************************
GetCellRect
	Returns the rectangle for the given cell co-ordinates.
	If the cell is joined then the given co-ordinates 
	are modified to point to the start cell for the join.
Params:
	col		- column to find the rect of
	row		- row to find the rect of
	rect	- rectangle to calculate
Returns:
	UG_SUCCESS	- success
*************************************************/
int CUGSideHdg::GetCellRect(int *col,long *row,RECT *rect)
{
	int xIndex,yIndex;
	int width	= 0;
	int height	= 0;

	int startCol	= *col;
	long startRow	= *row;
	int endCol		= *col;
	long endRow		= *row;

	rect->left		= 0;
	rect->top		= 0;
	rect->right		= m_GI->m_sideHdgWidth;
	rect->bottom	= 0;
	
	//if the specified cell is within a join then find the joined range
	if(m_GI->m_enableJoins)
	{
		if(m_ctrl->GetJoinRange(&startCol,&startRow,&endCol,&endRow) == UG_SUCCESS)
		{
			*col = startCol;
			*row = startRow;
		}
	}

	//find the col
	for(xIndex= (m_GI->m_numberSideHdgCols * -1);xIndex < 0;xIndex++)
	{
		if(xIndex == startCol)
			rect->left = width;
		
		width += GetSHColWidth(xIndex);

		if(xIndex == endCol)
		{
			rect->right = width;
			break;
		}
	}

	//find the row
	if(startRow >= m_GI->m_numLockRows)
	{
		rect->top = m_GI->m_lockRowHeight;
		rect->bottom = m_GI->m_lockRowHeight;
	}
	for(yIndex= 0;yIndex < m_GI->m_numberRows ;yIndex++)
	{		
		if(yIndex == m_GI->m_numLockRows)
			yIndex = m_GI->m_topRow;

		if(yIndex == startRow)
			rect->top = height;

		height += m_ctrl->GetRowHeight(yIndex);

		if(yIndex == endRow)
		{
			rect->bottom = height;	
			break;
		}
	}

	return UG_SUCCESS;
}

/************************************************
GetCellFromPoint
	Returns the column and row that lies over the given x,y co-ordinates.
	The co-ordinates are relative to the top left hand corner of the grid.
Params:
	point - point to check
	col	  - column that lies over the point
	row   - row that lies over the point
	rect  - rectangle of the cell found
Returns:
	UG_SUCCESS	- if a matching cell was found 
	UG_ERROR	- point provided does not correspond to a cell
*************************************************/
int CUGSideHdg::GetCellFromPoint(CPoint *point,int *col,long *row,RECT *rect)
{
	int ptsFound = 0;
	int xIndex;
	long yIndex;

	rect->left		=0;
	rect->top		=0;
	rect->right		=0;
	rect->bottom	=0;

	//find the row
	for(yIndex=0;yIndex<m_GI->m_numberRows;yIndex++)
	{	
		if(yIndex == m_GI->m_numLockRows)
			yIndex = m_GI->m_topRow;
		
		rect->bottom += m_ctrl->GetRowHeight(yIndex);

		if(rect->bottom > point->y)
		{
			rect->top = rect->bottom - m_ctrl->GetRowHeight(yIndex);
			ptsFound ++;
			*row = yIndex;
			break;
		}
	}

	//find the col
	for(xIndex= m_GI->m_numberSideHdgCols * -1 ;xIndex <0 ;xIndex++)
	{
		rect->right += GetSHColWidth(xIndex);

		if(rect->right > point->x)
		{
			rect->left = rect->right - GetSHColWidth(xIndex);
			ptsFound ++;
			*col = xIndex;
			break;
		}
	}

	if(ptsFound == 2)
		return UG_SUCCESS;

	*col = -1;
	*row = -1;
	return UG_ERROR;
}

/************************************************
GetSHColWidth
	function returns height (in pixels) of a side heading' column that user is
	interested in.  The column number passed in has to be a negative value ranging
	from zero to negative value of number of side heading columns set.
	((m_GI->m_numberSideHdgCols) * -1)
Params:
	col			- indicates row of interest
Returns:
	width in pixels of the column in question, or zero if column not found.
*************************************************/
int CUGSideHdg::GetSHColWidth(int col)
{
	//translate the column number into a 0 based positive index
	col = (col * -1) -1;

	if(col <0 || col > m_GI->m_numberSideHdgCols)
		return 0;

	return m_GI->m_sideHdgWidths[col];
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over side heading
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Returns:
	Context help ID to be displayed
*************************************************/
LRESULT CUGSideHdg::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col;
	long row;
	CRect rect(0,0,0,0);
	CPoint point( LOWORD(lParam), HIWORD(lParam));
	GetCellFromPoint( &point, &col, &row, &rect );
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_SIDEHEADING );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) over side heading
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Returns:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGSideHdg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// this message is fired as result of mouse activated help in a dialog
		int col;
		long row;
		CRect rect(0,0,0,0);
		CPoint point;
		point.x = pHelpInfo->MousePos.x;
		point.y = pHelpInfo->MousePos.y;

		ScreenToClient( &point );
		GetCellFromPoint( &point, &col, &row, &rect );
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( col, row, UG_SIDEHEADING ));
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
int CUGSideHdg::OnToolHitTest(  CPoint point, TOOLINFO *pTI ) const
{
	int col;
	long row;
	CRect rect;
	static int lastCol = -2;
	static long lastRow = -2;	

	if( m_ctrl->m_CUGSideHdg->GetCellFromPoint( &point, &col, &row, &rect ) == UG_SUCCESS)
	{
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
	return -1;
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
BOOL CUGSideHdg::ToolTipNeedText( UINT id, NMHDR* pTTTStruct, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(*pResult);

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;

	static CString string;
	int col;
	long row;
	CPoint point;
	CRect rect;

	GetCursorPos(&point);
	ScreenToClient(&point);

	if ( GetCellFromPoint( &point,&col,&row, &rect ) == UG_SUCCESS )
	{
		if ( m_ctrl->OnHint(col,row,UG_SIDEHEADING,&string) == TRUE )
		{
 			pTTT->lpszText = const_cast<LPTSTR>((LPCTSTR)string);
			return TRUE;
		}
	}

	return FALSE;
}