/*************************************************************************
				Class Implementation : CUGGrid
**************************************************************************
	Source file : UGGrid.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"

#include <math.h>
#include "UGCtrl.h"
/*	define WM_HELPHITTEST and WM_COMMANDHELP messages
	required for the context sensitive help support*/
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED                 0x031A
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGGrid::CUGGrid()
{
	m_doubleBufferMode	= FALSE;
	m_keyRepeatCount	= 0;
	m_hasFocus			= FALSE;
	m_cellTypeCapture	= FALSE;
	m_bitmap			= NULL;
	m_tempDisableFocusRect = FALSE;
	m_bHandledMouseDown	= FALSE;
}

CUGGrid::~CUGGrid()
{
	if(m_bitmap != NULL)
		delete m_bitmap;
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGGrid, CWnd)
	//{{AFX_MSG_MAP(CUGGrid)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()	
	ON_WM_SETCURSOR()
	ON_WM_KEYUP()
	ON_WM_GETDLGCODE()
	ON_WM_CREATE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, ToolTipNeedText)

	#ifdef WM_MOUSEWHEEL
		ON_WM_MOUSEWHEEL()
	#endif

END_MESSAGE_MAP()

/***************************************************
OnCreate
	Please see MSDN for more details on this event
	handler.
Params:
	lpCreateStruct	- Points to a CREATESTRUCT structure
					  that contains information about the
					  CWnd object being created. 
Returns:
	OnCreate must return 0 to continue the creation of the
	CWnd object. If the application returns –1, the window
	will be destroyed.
*****************************************************/
int CUGGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(m_GI->m_enableHints)
		EnableToolTips(TRUE);

	return 0;
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
BOOL CUGGrid::ToolTipNeedText( UINT id, NMHDR* pTTTStruct, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(*pResult);
	
	static CString string;
	int col;
	long row;
	// get mouse coordinates
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( &point );

	// check if the mouse pointer is over a cell
	if ( m_ctrl->GetCellFromPoint( point.x, point.y, &col, &row ) == UG_SUCCESS )
	{
		if ( m_ctrl->OnHint( col, row, UG_GRID, &string ) == TRUE )
		{
			TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
			// Do this to Enable multiline ToolTips
			::SendMessage( pTTT->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX );
			::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX );
			::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_INITIAL, 200 );
			::SendMessage( pTTT->hdr.hwndFrom, TTM_SETDELAYTIME, TTDT_RESHOW, 200 );
			// set tool tip's string
			pTTT->lpszText = const_cast<LPTSTR>((LPCTSTR)string);

			return TRUE;
		}
	}
	return FALSE;
}

/***************************************************
OnToolHitTest
	The framework calls this member function to determine whether a point is in
	the bounding rectangle of the specified tool. If the point is in the
	rectangle, it retrieves information about the tool.
Params:
	point	- please see MSDN for more information on the parameters.
	pTI
Returns:
	If 1, the tooltip control was found; If -1, the tooltip control was not found.
*****************************************************/
int CUGGrid::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	int col;
	long row;
	static int lastCol = -2;
	static long lastRow = -2;
	
	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row) == UG_SUCCESS)
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
OnPaint
	This routine is responsible for gathering the
	information to draw, get the selected state
	plus draw in an optomized fashion
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::OnPaint() 
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	CClientDC dc(this);
	//redraw the cells in any invalid region
	CRect clipRect;

	if ( GetUpdateRect( clipRect ) == 0 )
		return;

	ValidateRect( NULL );

	//if( dc.GetClipBox( clipRect ) != NULLREGION )
	if ( clipRect.left == 0 && clipRect.right == 0 &&
		 clipRect.top == 0 && clipRect.bottom == 0 )
	{
		// redraw all cells in the grid.
		m_drawHint.AddHint(0,0,m_GI->m_numberCols,m_GI->m_numberRows);
	}
	else
	{
		int startCol	= 0,
			endCol		= m_GI->m_numberCols;
		long startRow	= 0,
			 endRow		= m_GI->m_numberRows;
		// determine the top-left and bottom-right cells of the clip rectangle
		m_ctrl->GetCellFromPoint( clipRect.left, clipRect.top, &startCol, &startRow );
		m_ctrl->GetCellFromPoint( clipRect.right, clipRect.bottom, &endCol, &endRow );
		// if the bottom right cell in the range is part of a join,
		// than get information on 'join end' cell.
		CUGCell cell;
		m_ctrl->GetCell( endCol, endRow, &cell );
		BOOL bOrigin;
		int stCol;
		long stRow;
		if ( cell.GetJoinInfo( &bOrigin, &stCol, &stRow ) == UG_SUCCESS )
		{
			endCol = endCol + stCol;
			endRow = endRow + stRow;
		}

		// redraw the region of cells that are affected by the invalid rect
		m_drawHint.AddHint( startCol, startRow, endCol, endRow );

		// if the current cell is not part of the clip region,
		// add the current cell to the draw hints
		if (!(startCol >= m_ctrl->GetCurrentCol() && endCol <= m_ctrl->GetCurrentCol() &&
			 startRow >= m_ctrl->GetCurrentRow() && endRow <= m_ctrl->GetCurrentRow()))
		{	
			if ( m_GI->m_highlightRowFlag == FALSE )
				m_drawHint.AddHint( m_ctrl->GetCurrentCol(), m_ctrl->GetCurrentRow());
			else
				m_drawHint.AddHint( 0, m_ctrl->GetCurrentRow(), m_GI->m_numberCols, m_ctrl->GetCurrentRow());
		}
	}

	//double buffering
	CDC * db_dc = NULL;
	if(m_doubleBufferMode)
	{
		db_dc = new CDC;
		db_dc->CreateCompatibleDC(NULL);
		db_dc->SelectObject(m_bitmap);
	}
	
	DrawCellsIntern(&dc,db_dc);
	m_drawHint.ClearHints();

	if(db_dc!= NULL)
		delete db_dc;
}

/***************************************************
PaintDrawHintsNow
	Redraws all of the cells in that have been added
	the the CUGDrawHints (m_drawHint).
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::PaintDrawHintsNow(LPCRECT invalidateRect){

	InvalidateRect(invalidateRect,FALSE);

	CPaintDC dc(this); // device context for painting

	//double buffering
	CDC * db_dc = NULL;
	if(m_doubleBufferMode){
		db_dc = new CDC;
		db_dc->CreateCompatibleDC(NULL);
		db_dc->SelectObject(m_bitmap);
	}
	
	DrawCellsIntern(&dc,db_dc);

	if(db_dc!= NULL)
		delete db_dc;
}

/***************************************************
DrawCellsIntern
	function is the key to the fast redraw functionality in the Ultimate Grid.
	This function is responsible for drawing cells within the grid area, it
	makes sure that only the cells that are marked as invalid
	(by calling CUGDrawHint::IsInvalid function) are redrawn.
Params:
	dc		- screen DC
	db_dc	- off-screen DC, used when double buffering is enabled.
Returns:
	<none>
*****************************************************/
void CUGGrid::DrawCellsIntern(CDC *dc,CDC *db_dc)
{
	RECT		rect = {0,0,0,0};
	RECT		cellRect;
	RECT		tempRect;
	
	CUGCell		cell,tempCell;
	CString		string;
	CSize		size;

	RECT		focusRect = {-1,-1,-1,-1};
	CUGCellType * cellType;

	int			rightBlank	= -1;
	int			bottomBlank = -1;

	int			col,x;
	long		row,y;

	int			selectBlock;

	int			dcID;
	CDC*		origDC = dc;

	//double buffering
	if(m_doubleBufferMode)
	{
		origDC = dc;
		dc =  db_dc;
		//get the default font to use
		if(m_GI->m_defFont != NULL){
			origDC->SelectObject(m_GI->m_defFont);
			dc->SelectObject(m_GI->m_defFont);
		}
	}
	else{
		//get the default font to use
		if(m_GI->m_defFont != NULL){
			dc->SelectObject(m_GI->m_defFont);
		}
	}
	
	m_ctrl->OnScreenDCSetup(origDC,db_dc,UG_GRID);
	
	//set the right and bottom vars to point to
	//the extremes, if the right or bottom is
	//sooner then they will be updated in the
	//main drawing loop
	m_GI->m_rightCol = m_GI->m_numberCols;
	m_GI->m_bottomRow = m_GI->m_numberRows;
	
	//main draw loop, this loop goes through all visible
	//cells and checks to see if they need redrawing
	//if they do then the cell is retrieved and drawn
	for(y = 0; y < m_GI->m_numberRows;y++)
	{
		//skip rows hidden under locked rows
		if(y == m_GI->m_numLockRows)
			y = m_GI->m_topRow;

		row = y;

		//calc the top bottom and right side of the rect 
		//for the current cell to be drawn
		rect.top = rect.bottom;

		if(m_GI->m_uniformRowHeightFlag)
			rect.bottom += m_GI->m_defRowHeight;
		else
			rect.bottom += m_GI->m_rowHeights[row];

		if(rect.top == rect.bottom)
			continue;

		rect.right = 0;

		//check all visible cells in the current row to 
		//see if they need drawing
		for(x = 0;x < m_GI->m_numberCols;x++){

			//skip cols hidden under locked cols
			if(x == m_GI->m_numLockCols)
				x = m_GI->m_leftCol;

			row = y;
			col = x;

			//calc the left and right side of the rect
			rect.left = rect.right;
			rect.right  += m_GI->m_colInfo[col].width;

			if(rect.left == rect.right)
				continue;

			//check to see if the cell need to be redrawn
			if(m_drawHint.IsInvalid(col,row) != FALSE){
	
				//copy the rect, then use the cellRect from here
				//this is done since the cellRect may be modified
				CopyRect(&cellRect,&rect);

				//get the cell to draw
				m_ctrl->GetCellIndirect(col,row,&cell);

				//check to see if the cell is joined
				if(cell.IsPropertySet(UGCELL_JOIN_SET)){
					m_ctrl->GetCellRect(col,row,&cellRect);
					m_ctrl->GetJoinStartCell(&col,&row,&cell);
					if(m_drawHint.IsValid(col,row))
						continue;
					m_drawHint.SetAsValid(col,row);
				}

				//get the cell type to draw the cell
				if(cell.IsPropertySet(UGCELL_CELLTYPE_SET)){
					cellType = m_ctrl->GetCellType(cell.GetCellType());
				}
				else
				{
					cellType = m_ctrl->GetCellType(-1);
				}


				dcID = dc->SaveDC();

				//draw the cell, check to see if it is 'current' and/or selected
				CopyRect(&tempRect,&cellRect);
				if(row == m_GI->m_currentRow && ( col == m_GI->m_currentCol || m_GI->m_highlightRowFlag))
					cellType->OnDraw(dc,&cellRect,col,row,&cell,0,1);
				else{
					if(m_GI->m_multiSelect->IsSelected(col,row,&selectBlock))
						cellType->OnDraw(dc,&cellRect,col,row,&cell,selectBlock+1,0);
					else
						cellType->OnDraw(dc,&cellRect,col,row,&cell,0,0);
				}
				CopyRect(&cellRect,&tempRect);

				//draw a black line at the right edge of the locked columns
				if(m_GI->m_numLockCols >0){
					if(col == m_GI->m_leftCol){
						dc->SelectObject(GetStockObject(BLACK_PEN));
						dc->MoveTo(cellRect.left-1,cellRect.top);
						dc->LineTo(cellRect.left-1,cellRect.bottom+1);
					}
					else if(col == m_GI->m_numLockCols-1){
						dc->SelectObject(GetStockObject(BLACK_PEN));
						dc->MoveTo(cellRect.right-1,cellRect.top);
						dc->LineTo(cellRect.right-1,cellRect.bottom+1);
					}
				}
				//draw a black line at the bottom of the locked rows
				if(m_GI->m_numLockRows >0){
					if(row == m_GI->m_topRow ){
						dc->SelectObject(GetStockObject(BLACK_PEN));
						dc->MoveTo(cellRect.left,cellRect.top-1);
						dc->LineTo(cellRect.right+1,cellRect.top-1);
					}
					else if(row == m_GI->m_numLockRows-1){
						dc->SelectObject(GetStockObject(BLACK_PEN));
						dc->MoveTo(cellRect.left,cellRect.bottom-1);
						dc->LineTo(cellRect.right+1,cellRect.bottom-1);
					}
				}

				dc->RestoreDC(dcID);
			}

			//check to see if the focus rect should be drawn
			//this function should be called all the time
			//even if it is off screen
			if(row == m_GI->m_currentRow && (col == m_GI->m_currentCol || 
				m_GI->m_highlightRowFlag)){
				CopyRect(&focusRect,&cellRect);
			}

			//check to see if the right side of the rect is past the edge
			//of the grid drawing area, if so then break
			if(rect.right > m_GI->m_gridWidth){
				m_GI->m_rightCol = col;
				break;
			}
		
		}

		//check to see if there is blank space on the right side of the grid
		//drawing area
		if(rect.right < m_GI->m_gridWidth && m_GI->m_rightCol == m_GI->m_numberCols){
			rightBlank = rect.right;
		}

		//check to see if the bottom of the rect is past the bottom of the 
		//grid drawing area, if so then break
		if(rect.bottom > m_GI->m_gridHeight){
			m_GI->m_bottomRow = row;
			break;
		}

		//check for extra rows
		if(y >= (m_GI->m_numberRows-1)){
			long origNumRows = m_GI->m_numberRows;
			long newRow = y+1;
			m_ctrl->VerifyCurrentRow(&newRow);
			if(m_GI->m_numberRows > origNumRows){
				m_drawHint.AddHint(0,y+1,m_GI->m_numberCols-1,y+1);
			}
		}
	}

	//clear the draw hints
	m_drawHint.ClearHints();

	//check to see if there is blank space on the bottom of the grid
	//drawing area
	if(rect.bottom < m_GI->m_gridHeight && m_GI->m_bottomRow == m_GI->m_numberRows)
		bottomBlank = rect.bottom;
	
	//fill in the blank grid drawing areas
	if(rightBlank >=0 || bottomBlank >=0){
		CBrush brush(m_ctrl->OnGetDefBackColor(UG_GRID));
		GetClientRect(&rect);
		if(rightBlank >=0){
			rect.left = rightBlank;
			dc->FillRect(&rect,&brush);
		}
		if(bottomBlank >=0){
			rect.left = 0;
			rect.top = bottomBlank;
			dc->FillRect(&rect,&brush);
		}
	}

	//double buffering
	if(m_doubleBufferMode){
		dc = origDC;
		dc->BitBlt(0,0,m_GI->m_gridWidth,m_GI->m_gridHeight,db_dc,0,0,SRCCOPY);
	}	
	
	//draw the focus rect, if the flag was set above
	if(!m_tempDisableFocusRect){
		if((m_hasFocus || m_ctrl->m_findDialogRunning) && !m_ctrl->m_editInProgress)		
		{
			if(m_GI->m_highlightRowFlag)
			{
				focusRect.left = 0;

				if(rect.right < m_GI->m_gridWidth)
					focusRect.right = rect.right;
				else
				{
					if( m_GI->m_bExtend )
						focusRect.right = m_GI->m_gridWidth;
					else
					{
						int iStartCol = m_ctrl->GetLeftCol(), iEndCol = m_ctrl->GetNumberCols() - 1;
						int iWidth = 0;
						for( int iLoop = iStartCol ; iLoop <= iEndCol ; iLoop++ )
							iWidth += m_ctrl->GetColWidth( iLoop );

						focusRect.right = iWidth;
					}
				}
			}
			m_ctrl->OnDrawFocusRect(dc,&focusRect);
		}
	}

	//reset temporary flags
	m_tempDisableFocusRect = FALSE;
}

/***************************************************
RedrawCell
	function is called to redraw a single cell, it will add provided cell
	to draw hints (CUGDrawHint) and forces the window to update.
Params:
	col, row	- coordinates of cell to redraw
Returns:
	<none>
*****************************************************/
void CUGGrid::RedrawCell(int col,long row)
{
	m_drawHint.AddHint(col,row);
	Moved();
}

/***************************************************
RedrawRow
	function is called to redraw a single cell, it will add provided cells
	to draw hints (CUGDrawHint) and forces the window to update.
Params:
	row			- indicates the row number to update.
Returns:
	<none>
*****************************************************/
void CUGGrid::RedrawRow(long row)
{
	m_drawHint.AddHint(0,row,m_GI->m_numberCols-1,row);
	Moved();
}

/***************************************************
RedrawCol
	function is called to redraw a single cell, it will add provided cells
	to draw hints (CUGDrawHint) and forces the window to update.
Params:
	col			- indicates the column number to update.
Returns:
	<none>
*****************************************************/
void CUGGrid::RedrawCol(int col)
{
	m_drawHint.AddHint(col,0,col,m_GI->m_numberRows-1);
	Moved();
}

/***************************************************
RedrawRange
	function is called to redraw a single cell, it will add provided cells
	to draw hints (CUGDrawHint) and forces the window to update.
Params:
	startCol,	- parameters passed in indicate the range of cells to be redrawn
	startRow,	  the range's top-left corned is indicated by the StartCol and
	endCol,		  StartRow, and the bottom-right corner by the endCol and endRow.
	endRow
Returns:
	<none>
*****************************************************/
void CUGGrid::RedrawRange(int startCol,long startRow,int endCol,long endRow)
{
	m_drawHint.AddHint(startCol,startRow,endCol,endRow);
	Moved();
}

/***************************************************
TempDisableFocusRect
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::TempDisableFocusRect(){
	m_tempDisableFocusRect = TRUE;
}


/***************************************************
Update
	function forces window update.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::Update()
{
	InvalidateRect(NULL);
	UpdateWindow();
	return;
}

/***************************************************
Moved
	This routine recalcs the internal variables
	plus causes the grid to re-draw itself
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::Moved()
{
	int xScroll = 0;
	int yScroll = 0;
	int x;
	long y;
	CDC * pDC, *screenDC,*db_DC = NULL;

	if(GetUpdateRect(NULL)){
		UpdateWindow();
		return;
	}
	if(m_ctrl->m_findDialogRunning){
		Invalidate();
		UpdateWindow();
		return;
	}
	if(m_GI->m_enableVScrollHints || m_GI->m_enableHScrollHints){
		Invalidate();
		UpdateWindow();
		return;
	}

	/*	If the grid window is overlapped by other windows, 
		we should repaint the whole grid.*/
	HWND hPrevWnd, hNextWnd, hActiveWnd, hDtWnd;
	RECT rectGrid, rectOther, rectDest, rectSub, rectVisible;

	// Check if the grid window is overlapped by other windows
	GetWindowRect(&rectGrid);
	::CopyRect(&rectVisible, &rectGrid);
	for (hPrevWnd = m_hWnd; (hNextWnd = ::GetWindow(hPrevWnd, GW_HWNDPREV)) != NULL; hPrevWnd = hNextWnd){
	    ::GetWindowRect(hNextWnd, &rectOther);
		if (::IsWindowVisible(hNextWnd) && IntersectRect(&rectDest, &rectGrid, &rectOther)){
			::SubtractRect(&rectSub, &rectVisible, &rectDest);
			::CopyRect(&rectVisible, &rectSub);
		}
	}
	if ( !EqualRect(&rectVisible, &rectGrid) ) {
		Invalidate();
		UpdateWindow();
		return;
	}

	// Check if the grid is cliped by the active window (e.g. MDI frame window)
	hActiveWnd = ::GetActiveWindow();
	if ( (NULL != hActiveWnd) && (m_hWnd != hActiveWnd) ) {
		::GetWindowRect(hActiveWnd, &rectOther);
		if (::IsWindowVisible(hActiveWnd) && IntersectRect(&rectDest, &rectGrid, &rectOther)){
			if ( !EqualRect(&rectDest, &rectGrid) ) {
				Invalidate();
				UpdateWindow();
				return;
			}
		}
	}

	// Check if the grid is cliped by the desktop window
	hDtWnd = ::GetDesktopWindow();
	if ( (NULL != hDtWnd) && (m_hWnd != hDtWnd) ) {
		::GetWindowRect(hDtWnd, &rectOther);
		if (::IsWindowVisible(hDtWnd) && IntersectRect(&rectDest, &rectGrid, &rectOther)){
			if ( !EqualRect(&rectDest, &rectGrid) ) {
				Invalidate();
				UpdateWindow();
				return;
			}
		}
	}

	BOOL redrawAll = FALSE;

	//if the left col has changed then calc the scroll distance
	if(m_GI->m_leftCol != m_GI->m_lastLeftCol)
	{
		if(m_GI->m_leftCol > m_GI->m_lastLeftCol)
		{
			for(x = m_GI->m_lastLeftCol;x < m_GI->m_leftCol; x++)
			{
				xScroll += m_ctrl->GetColWidth(x);
				if(xScroll > m_GI->m_gridWidth)
				{
					redrawAll = TRUE;
					break;
				}
			}
		}
		else
		{
			for(x = m_GI->m_leftCol;x < m_GI->m_lastLeftCol; x++)
			{
				xScroll -= m_ctrl->GetColWidth(x);
				if(xScroll < -m_GI->m_gridWidth)
				{
					redrawAll = TRUE;
					break;
				}
			}
		}
		if(redrawAll)
		{
			xScroll = 0;
			//redraw the whole grid
			m_drawHint.AddHint(0,0,m_GI->m_numberCols,m_GI->m_numberRows);
		}
	}

	//if the top row has changed then calc the scroll distance
	if(m_GI->m_topRow != m_GI->m_lastTopRow)
	{
		if(m_GI->m_topRow > m_GI->m_lastTopRow)
		{
			for(y = m_GI->m_lastTopRow;y < m_GI->m_topRow; y++)
			{
				yScroll += m_ctrl->GetRowHeight(y);
				if(yScroll > m_GI->m_gridHeight)
				{
					redrawAll = TRUE;
					break;
				}
			}
		}
		else
		{
			for(y = m_GI->m_topRow;y < m_GI->m_lastTopRow; y++)
			{
				yScroll -= m_ctrl->GetRowHeight(y);
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
			//redraw the whole grid
			m_drawHint.AddHint(0,0,m_GI->m_numberCols,m_GI->m_numberRows);
		}
	}

	//create the dc
	screenDC = GetDC();
	if(m_doubleBufferMode)
	{
		pDC = new CDC;
		pDC->CreateCompatibleDC(NULL);
		pDC->SelectObject(m_bitmap);
		db_DC = pDC;
	}
	else
	{
		pDC = screenDC;
	}

	//do scrolling of the window here
	if(xScroll != 0 || yScroll != 0)
	{
		RECT scrollRect;
		GetClientRect(&scrollRect);
		if(xScroll == 0)
			scrollRect.top += m_GI->m_lockRowHeight;
		if(yScroll == 0)
			scrollRect.left += m_GI->m_lockColWidth;
		pDC->ScrollDC(-xScroll,-yScroll,&scrollRect,&scrollRect,NULL,NULL);
		
		//add the draw hints for the area the was uncovered by the scroll
		if(xScroll >0)
			m_drawHint.AddHint(m_GI->m_rightCol,0,m_GI->m_numberCols,m_GI->m_numberRows);
		else if(xScroll < 0)
			m_drawHint.AddHint(0,0,m_GI->m_lastLeftCol,m_GI->m_numberRows);
		if(yScroll >0)
			m_drawHint.AddHint(0,m_GI->m_bottomRow,m_GI->m_numberCols,m_GI->m_numberRows);
		else if(yScroll < 0)
			m_drawHint.AddHint(0,0,m_GI->m_numberCols,m_GI->m_lastTopRow);
	}

	//add the last and current cells - add row support later
	if(m_GI->m_highlightRowFlag)
	{
		m_drawHint.AddHint(0,m_GI->m_lastRow,m_GI->m_numberCols,m_GI->m_lastRow);
		m_drawHint.AddHint(0,m_GI->m_currentRow,m_GI->m_numberCols,m_GI->m_currentRow);
	}
	else
	{
		m_drawHint.AddHint(m_GI->m_lastCol,m_GI->m_lastRow);
		m_drawHint.AddHint(m_GI->m_currentCol,m_GI->m_currentRow);
	}

	//call the grids main drawing routine
	DrawCellsIntern(screenDC,db_DC);

	//double buffering
	if(m_doubleBufferMode)
		delete db_DC;
	
	//close the device context
	ReleaseDC(screenDC);
}

/***************************************************
OnKeyDown
	Passes all keystrokes to a callback routine in CUGCtrl (OnKeyDown)
	then processes them.  The callback has ability to diallow further processing
	of the event.
Params:
	nChar		- please see MSDN for more information on the parameters.
	nRepCnt
	nFlags
Returns:
	<none>
*****************************************************/
void CUGGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);

	m_GI->m_moveType = 0;	//key(default)

	int increment = 1; //default number of units to move

	if(m_GI->m_ballisticKeyMode > 0)
	{
		m_keyRepeatCount++;

		int value = (m_keyRepeatCount / m_GI->m_ballisticKeyMode);
		increment = value*value*value + 1;

		if( m_GI->m_ballisticKeyDelay > 0 )
		{
			if( value == 0 && m_keyRepeatCount > 1 )
				Sleep(m_GI->m_ballisticKeyDelay);
		}
	}

	//send a notify message to the cell type class
	BOOL processed = m_ctrl->GetCellType(m_GI->m_currentCol,m_GI->m_currentRow)->
		OnKeyDown(m_GI->m_currentCol,m_GI->m_currentRow,&nChar);

	//send a keydown notify message
	m_ctrl->OnKeyDown(&nChar,processed);

	int  curCol = m_GI->m_currentCol;
	long curRow = m_GI->m_currentRow;
	int  JoinCellStartCol, JoinCellEndCol, colOff, rowOff;
	long JoinCellStartRow, JoinCellEndRow;

	//process the keystroke
	if(nChar == VK_LEFT)
	{
		JoinCellStartCol = curCol - increment;
		JoinCellStartRow = curRow;
		if ( (JoinCellStartCol >= 0) && (m_ctrl->GetJoinStartCell(&JoinCellStartCol,&JoinCellStartRow) == UG_SUCCESS) )
		{
			m_ctrl->GotoCell(m_GI->m_dragCol - (curCol - JoinCellStartCol),
							 m_GI->m_dragRow - (curRow - JoinCellStartRow));
		}
		else
			m_ctrl->GotoCol(m_GI->m_dragCol - increment);
	}
	else if(nChar == VK_RIGHT)
	{
		JoinCellStartCol = curCol;
		JoinCellStartRow = curRow;
		colOff = increment;
		if ( m_ctrl->GetJoinRange(&JoinCellStartCol,&JoinCellStartRow, &JoinCellEndCol,&JoinCellEndRow) == UG_SUCCESS )
			colOff = (JoinCellEndCol - JoinCellStartCol) + increment;

		JoinCellStartRow = curRow;
		JoinCellStartCol = curCol + colOff;
		if (m_ctrl->GetJoinStartCell(&JoinCellStartCol,&JoinCellStartRow) == UG_SUCCESS)
		{
			m_ctrl->GotoCell(m_GI->m_dragCol - (curCol - JoinCellStartCol),
							 m_GI->m_dragRow - (curRow - JoinCellStartRow));
		}
		else
			m_ctrl->GotoCol(m_GI->m_dragCol + colOff);
	}
	else if(nChar == VK_UP)
	{
		JoinCellStartCol = curCol;
		JoinCellStartRow = curRow - increment;
		if ( (JoinCellStartRow >= 0) && (m_ctrl->GetJoinStartCell(&JoinCellStartCol,&JoinCellStartRow) == UG_SUCCESS) )
		{
			m_ctrl->GotoCell(m_GI->m_dragCol - (curCol - JoinCellStartCol),
							 m_GI->m_dragRow - (curRow - JoinCellStartRow));
		}
		else
			m_ctrl->GotoRow(m_GI->m_dragRow - increment);
	}
	else if(nChar == VK_DOWN)
	{
		JoinCellStartCol = curCol;
		JoinCellStartRow = curRow;
		rowOff = increment;
		if ( m_ctrl->GetJoinRange(&JoinCellStartCol,&JoinCellStartRow, &JoinCellEndCol,&JoinCellEndRow) == UG_SUCCESS )
			rowOff = (JoinCellEndRow - JoinCellStartRow) + increment;

		JoinCellStartCol = curCol;
		JoinCellStartRow = curRow + rowOff;
		if (m_ctrl->GetJoinStartCell(&JoinCellStartCol,&JoinCellStartRow) == UG_SUCCESS)
		{
			m_ctrl->GotoCell(m_GI->m_dragCol - (curCol - JoinCellStartCol),
							 m_GI->m_dragRow - (curRow - JoinCellStartRow));
		}
		else
			m_ctrl->GotoRow(m_GI->m_dragRow + rowOff);
	}
	else if(nChar == VK_PRIOR)
		m_ctrl->MoveCurrentRow(UG_PAGEUP);
	else if(nChar == VK_NEXT)
		m_ctrl->MoveCurrentRow(UG_PAGEDOWN);
	else if(nChar == VK_HOME){
		if(GetKeyState(VK_CONTROL) <0)
			m_ctrl->MoveCurrentRow(UG_TOP);
		else
			m_ctrl->MoveCurrentCol(UG_LEFT);
	}
	else if(nChar == VK_END)
	{
		if(GetKeyState(VK_CONTROL) <0)
			m_ctrl->MoveCurrentRow(UG_BOTTOM);
		else
			m_ctrl->MoveCurrentCol(UG_RIGHT);
	}
}

/***************************************************
OnLButtonDown
	Finds the cell that was clicked in, sends a notification
	updates the current cells position.
	It also sets mouse capture.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( GetFocus() != this )
		SetFocus();

	if(m_ctrl->m_editInProgress)
		return;

	int col = -1;
	long row = -1;
	RECT rect;
	BOOL processed = FALSE;

	SetCapture();

	//setup the move type flags
	m_GI->m_moveType = 1;	//lbutton
	m_GI->m_moveFlags = nFlags;

	//check to see what cell was clicked in, and move there
	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row,&rect) == UG_SUCCESS){
	
		m_ctrl->GotoCell(col,row);

		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,1,&rect,&point);

		if(processed)
			m_cellTypeCapture = TRUE;
	}

	m_ctrl->OnLClicked(col,row,1,&rect,&point,processed);

	m_GI->m_moveType = 0;	//key(default)

	// indicate that this mouse button down event was handled by the grid,
	// allowing for the mouse button up to be processed.
	m_bHandledMouseDown = TRUE;
}

/***************************************************
OnLButtonUp
	Finds the cell that was clicked in
	Sends a notification to CUCtrl::OnLClicked
	Releases mouse capture
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int col = -1;
	long row = -1;
	RECT rect;
	BOOL processed = FALSE;

	if ( m_bHandledMouseDown != TRUE )
		return;

	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row,&rect) == UG_SUCCESS){

		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
	}

	m_ctrl->OnLClicked(col,row,0,&rect,&point,processed);

	ReleaseCapture();

	m_GI->m_dragCol = m_GI->m_currentCol;
	m_GI->m_dragRow = m_GI->m_currentRow;

	m_cellTypeCapture = FALSE;
	m_bHandledMouseDown = FALSE;
}

/***************************************************
OnLButtonDblClk
	Finds the cell that was clicked in
	Sends a notification to CUCtrl::OnDClicked
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int col = -1;
	long row = -1;
	RECT rect;
	BOOL processed = FALSE;
	
	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row,&rect) == UG_SUCCESS){

		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnDClicked(col,row,&rect,&point);
	}

	m_ctrl->OnDClicked(col,row,&rect,&point,processed);
}

/***************************************************
OnMouseMove
	Finds the cell that the mouse is over and if the left mouse button is down
	then the current cell postion is updated
	Sends optional notification messages
	Also scrolls the current cell if the mouse is outside the window area.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	int col;
	long row;
	BOOL vertViewMove = FALSE,
		 horsViewMove = FALSE;

	if(m_ctrl->m_editInProgress)
		return;

	if(m_cellTypeCapture)
	{
		col = m_GI->m_currentCol;
		row = m_GI->m_currentRow;
		//send a notification to the cell type
		BOOL processed = m_ctrl->GetCellType(col,row)->OnMouseMove(col,row,&point,nFlags);
		//send a notification to the main grid class
		m_ctrl->OnMouseMove(col,row,&point,nFlags,processed);

		return;
	}


	m_GI->m_moveType = 3;	//mouse move
	m_GI->m_moveFlags = nFlags;

	//check to see if the mouse is over a cell
	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row) == UG_SUCCESS)
	{
		//send a notification to the cell type
		BOOL processed = m_ctrl->GetCellType(col,row)->OnMouseMove(col,row,&point,nFlags);
		//send a notification to the main grid class
		m_ctrl->OnMouseMove(col,row,&point,nFlags,processed);
	}

	if(nFlags & MK_LBUTTON)
	{
		MSG msg;
		int moved = FALSE;

		while(1)
		{
			if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row) == UG_SUCCESS)
			{
				if ( row == m_GI->m_bottomRow )
					vertViewMove = TRUE;
				if ( col == m_GI->m_rightCol )
					horsViewMove = TRUE;

				// The 'm_bScrollOnParialCells' flag controls the scrolling behavior
				// when the mouse is dragged over partially visible cell.  By default
				// we will treat this area same as area 'outside of the grid' meaning
				// that the grid will scroll using current ballistics settigs.
				// You can set this flag to FALSE if you do not want the grid to
				// scroll when user's mouse is over the partially visible cells,
				// the grid will only do that when mouse is moved outside of the view.
				if ( m_GI->m_bScrollOnParialCells == FALSE && ( vertViewMove == TRUE || horsViewMove == TRUE ))
					return;

				if ( m_bHandledMouseDown != TRUE )
					return;

				//send a notification to the cell type
				if(m_GI->m_numLockCols == 0 && m_GI->m_numLockRows == 0)
					m_ctrl->GotoCell(col,row);
				else
				{
					if(m_GI->m_dragCol > m_GI->m_numLockCols && col < m_GI->m_numLockCols)
						m_ctrl->MoveCurrentCol(UG_LINEUP);
					else
						m_ctrl->GotoCol(col);

					if(m_GI->m_dragRow > m_GI->m_numLockRows && row < m_GI->m_numLockRows)
						m_ctrl->MoveCurrentRow(UG_LINEUP);
					else
						m_ctrl->GotoRow(row);
				}	
			}
			//if the mouse is off the left side
			if(point.x < 0)
			{
				//if ballistic mode
				if(m_GI->m_ballisticMode)
				{
					int increment = (int)pow((double)((point.x * -1)/ m_GI->m_defColWidth +1), 
						m_GI->m_ballisticMode); 
					m_ctrl->GotoCol(m_GI->m_dragCol - increment);
					if(increment == 1)
						Sleep(m_GI->m_ballisticDelay);
				}
				else
					m_ctrl->MoveCurrentCol(UG_LINEUP);
				moved = TRUE;
			}
			//if the mouse is off the right side
			else if(point.x > m_GI->m_gridWidth || horsViewMove)
			{
				//if ballistic mode
				if(m_GI->m_ballisticMode)
				{
					int increment = (int)pow((double)(point.x - m_GI->m_gridWidth) / 
						m_GI->m_defColWidth +1, m_GI->m_ballisticMode); 
					m_ctrl->GotoCol(m_GI->m_dragCol + increment);
					if(increment == 1)
						Sleep(m_GI->m_ballisticDelay);
				}
				else
					m_ctrl->MoveCurrentCol(UG_LINEDOWN);
				moved = TRUE;
			}
			//if the mouse is above the top
			if(point.y < 0)
			{
				//if ballistic mode
				if(m_GI->m_ballisticMode)
				{
					long increment = (long)pow((double)((point.y* -1) / m_GI->m_defRowHeight +1), 
						m_GI->m_ballisticMode); 
					m_ctrl->GotoRow(m_GI->m_dragRow - increment);
					if(increment == 1)
						Sleep(m_GI->m_ballisticDelay);
				}
				else
					m_ctrl->MoveCurrentRow(UG_LINEUP);
				moved = TRUE;
			}
			//if the mouse is below the bottom
			else if(point.y > m_GI->m_gridHeight || vertViewMove)
			{
				//if ballistic mode
				if(m_GI->m_ballisticMode)
				{
					long increment = (long)pow((double)((point.y-m_GI->m_gridHeight) / 
						m_GI->m_defRowHeight +1), m_GI->m_ballisticMode); 
					m_ctrl->GotoRow(m_GI->m_dragRow + increment);
					if(increment == 1)
						Sleep(m_GI->m_ballisticDelay);
				}
				else
					m_ctrl->MoveCurrentRow(UG_LINEDOWN);
				moved = TRUE;
			}


			moved = FALSE;

			//check for messages, if there are not then scroll some more
			while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
			{
				if(msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONUP)
				{
					m_GI->m_moveType = 0;	//key(default)

					return;
				}
				GetMessage(&msg,NULL,0,0);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	m_GI->m_moveType = 0;	//key - default
}

/***************************************************
OnRButtonDown
	Finds the cell that the mouse is in
	Sends series of notifications to the CUGCrtl
	Pops up a menu if enabled and populated.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_ctrl->m_editInProgress)
		return;

	int col = -1;
	long row = -1;
	BOOL processed = FALSE;
	RECT rect;

	m_GI->m_moveType = 2;	//2button
	m_GI->m_moveFlags = nFlags;

	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row,&rect) == UG_SUCCESS)
	{
		m_ctrl->GotoCell(col,row);
		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnRClicked(col,row,1,&rect,&point);
	}
	
	m_ctrl->OnRClicked(col,row,1,&rect,&point,processed);


	m_GI->m_moveType = 0;	//key(default)

	if(m_GI->m_enablePopupMenu)
	{
		ClientToScreen(&point);
		m_ctrl->StartMenu(col,row,&point,UG_GRID);
	}

	m_GI->m_moveType = 0;//key - default
	m_bHandledMouseDown = TRUE;
}

/***************************************************
OnRButtonUp
	Finds the cell that the mouse is in
	Sends a OnRClicked notification to the CUGCtrl.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*****************************************************/
void CUGGrid::OnRButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int col = -1;
	long row = -1;
	RECT rect;
	BOOL processed = FALSE;

	if ( m_bHandledMouseDown != TRUE )
		return;

	if(m_ctrl->GetCellFromPoint(point.x,point.y,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnRClicked(col,row,0,&rect,&point);
	}

	m_ctrl->OnRClicked(col,row,0,&rect,&point,processed);

	m_GI->m_dragCol = m_GI->m_currentCol;
	m_GI->m_dragRow = m_GI->m_currentRow;
	m_bHandledMouseDown = FALSE;
}

/***************************************************
OnChar
	passed the handling of this event to the OnCharDown in both the cell's
	celltype and the CUGCtrl.
Params:
	nChar		- please see MSDN for more information on the parameters.
	nRepCnt
	nFlags
Returns:
	<none>
*****************************************************/
void CUGGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);
	BOOL processed = FALSE;

	//send a notification to the cell type	
	processed = m_ctrl->GetCellType(m_GI->m_currentCol,m_GI->m_currentRow)->
		OnCharDown(m_GI->m_currentCol,m_GI->m_currentRow,&nChar);

	m_ctrl->OnCharDown(&nChar,processed);
}

/***************************************************
PreCreateWindow
	The Utlimate Grid overwrites this function to further customize the
	window that is created.
Params:
	cs			- please see MSDN for more information on the parameters.
Returns:
	Nonzero if the window creation should continue; 0 to indicate creation failure.
*****************************************************/
BOOL CUGGrid::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = cs.style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CWnd::PreCreateWindow(cs);
}

/***************************************************
OnSetFocus
	The grid uses this event handler to setup internal variables and passes
	further processig to both the CWnd and CUGCtrl.
Params:
	pOldWnd		- pointer to window that lost the focus
Returns:
	<none>
*****************************************************/
void CUGGrid::OnSetFocus(CWnd* pOldWnd)
{
	m_hasFocus = TRUE;
	m_cellTypeCapture = FALSE;
	m_keyRepeatCount = 0;

	// pass the notification handling
	CWnd::OnSetFocus( pOldWnd );
	m_ctrl->OnSetFocus( UG_GRID );
	// redraw the current cell
	CRect cellRect;

	if ( m_GI->m_highlightRowFlag == TRUE )
	{
		m_ctrl->GetRangeRect( 0, m_GI->m_currentRow, m_GI->m_numberCols - 1, m_GI->m_currentRow, cellRect );
		m_drawHint.AddHint( 0, m_GI->m_currentRow, m_GI->m_numberCols - 1, m_GI->m_currentRow );
	}
	else
	{
		m_ctrl->GetCellRect( m_GI->m_currentCol, m_GI->m_currentRow, cellRect );
		m_drawHint.AddHint( m_GI->m_currentCol, m_GI->m_currentRow );
	}

	PaintDrawHintsNow( cellRect );
}

/***************************************************
OnKillFocus
	clears internal flag indicating that the grid no longer has focus
	and passes further processign to both the CWnd and CUGCtrl.
Params:
	pNewWnd		- pointer to window that is gaining the focus.
Returns:
	<none>
*****************************************************/
void CUGGrid::OnKillFocus(CWnd* pNewWnd)
{
	m_hasFocus = FALSE;

	CWnd::OnKillFocus(pNewWnd);
	// notify the grid that the grid lost focus
	m_ctrl->OnKillFocus(UG_GRID, pNewWnd);

	RedrawCell(m_GI->m_currentCol,m_GI->m_currentRow);
}

/***************************************************
OnMouseActivate
	The framework calls this member function when the cursor is in an inactive
	window and the user presses a mouse button.
Params:
	pDesktopWnd	- please see MSDN for more information on the parameters.
	nHitTest
	message
Returns:
	Specifies to activate the CWnd and to discard the mouse event.
*****************************************************/
int CUGGrid::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	CWnd::OnMouseActivate( pDesktopWnd, nHitTest, message );
	return MA_ACTIVATE;
}

/***************************************************
SetDoubleBufferMode
	turns on and off the double buffer functionality.  When the double buffer
	mode is on then the grid will first is drawn to an off-screen DC before
	the results are copied over to the screen.
Params:
	mode		- double buffer mode (TRUE or FALSE)
Returns:
	UG_SUCCESS	- on success
	UG_ERROR	- if creation of off-screen buffer was unsuccessful
*****************************************************/
int CUGGrid::SetDoubleBufferMode(int mode)
{
	if(m_bitmap != NULL)
	{
		delete m_bitmap;
		m_bitmap = NULL;
	}

	m_doubleBufferMode = mode;

	if(mode == TRUE)
	{
		m_bitmap = new CBitmap;

		CDC * pDC = GetDC();
		RECT rect;
	
		GetClientRect(&rect);
		if(rect.right ==0)
			rect.right =1;
		if(rect.bottom ==0)
			rect.bottom =1;

		if(m_bitmap->CreateCompatibleBitmap(pDC,rect.right,rect.bottom) == 0)
		{
			delete m_bitmap;
			m_bitmap = NULL;
			m_doubleBufferMode = FALSE;
			return UG_ERROR;
		}

		//clear the bitmap
		CBitmap * oldbitmap = pDC->SelectObject(m_bitmap);
		if(oldbitmap)
			pDC->SelectObject(oldbitmap);

		ReleaseDC(pDC);
	}
	
	return UG_SUCCESS;
}

/***************************************************
OnSize
	The framework calls this member function after the window's size has changed.
	The Ultimate Grid takes this opportunity to reallocate the off-screen bitmap
	used by the double buffer mode.
Params:
	nType		- please see MSDN for more information on the parameters.
	cx
	cY
Returns:
	<none>
*****************************************************/
void CUGGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	// reallocate the double buffer bitmap
	if( m_doubleBufferMode )
		SetDoubleBufferMode( m_doubleBufferMode );
}


/***************************************************
OnMouseWheel
	The framework calls this member function as a user rotates the mouse
	wheel and encounters the wheel's next notch.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	zDelta
	pt
Returns:
	Nonzero if mouse wheel scrolling is enabled; otherwise 0.
*****************************************************/
#ifdef WM_MOUSEWHEEL
BOOL CUGGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_ctrl->m_editInProgress) {
		int distance = zDelta / 120;
		m_GI->m_moveType = 4;
		m_ctrl->SetTopRow(m_GI->m_topRow - (distance * 3));
		m_ctrl->m_CUGSideHdg->Invalidate();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif

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
BOOL CUGGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(*pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	SetCursor(m_GI->m_arrowCursor);
	return TRUE;
}

/***************************************************
OnKeyUp
	The framework calls this member function when a nonsystem key is released,
	the grid passes this event to the cell type and CUGCtrl class for further
	processing.
Params:
	nChar		- please see MSDN for more information on the parameters.
	nRepCnt
	nFlags
Returns:
	<none>
*****************************************************/
void CUGGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);
	m_keyRepeatCount = 0;	

	//send a notify message to the cell type class
	BOOL processed = m_ctrl->GetCellType(m_GI->m_currentCol,m_GI->m_currentRow)->
		OnKeyUp(m_GI->m_currentCol,m_GI->m_currentRow,&nChar);

	//send a keyup notify message
	m_ctrl->OnKeyUp(&nChar,processed);
}

/***************************************************
OnGetDlgCode
	The Ultimate Grid handles this event to ensure proper behavior when
	grid is placed on a dialog.
Params:
	<none>
Returns:
	Please see MSDN for complete list of valid return values.
*****************************************************/
UINT CUGGrid::OnGetDlgCode() 
{
	if(IsChild(GetFocus()))
		return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	else
		return CWnd::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

/***************************************************
GetScrollBarCtrl
	function is used to allow the grid to share its scrollbars with
	parent window.
Params:
	nBar		- scrollbar to return pointer to.
Returns:
	Pointer to a sibling scroll-bar control.
*****************************************************/
CScrollBar* CUGGrid::GetScrollBarCtrl(int nBar) const
{
	if(nBar==SB_HORZ)
		return m_ctrl->m_CUGHScroll;
	else
		return m_ctrl->m_CUGVScroll;
}

/***************************************************
OnVScroll
	The framework calls this member function when the user clicks the window's vertical scroll bar.
Params:
	nSBCode		- please see MSDN for more information on the parameters.
	nPos
	pScrollBar
Returns:
	<none>
*****************************************************/
void CUGGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_ctrl->SendMessage(WM_VSCROLL,MAKEWPARAM(nSBCode,nPos),
		(LPARAM)(pScrollBar!=NULL ? pScrollBar->GetSafeHwnd() : NULL));
}

/***************************************************
OnHScroll
	The framework calls this member function when the user clicks the window's horizontal scroll bar.
Params:
	nSBCode		- please see MSDN for more information on the parameters.
	nPos
	pScrollBar
Returns:
	<none>
*****************************************************/
void CUGGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_ctrl->SendMessage(WM_HSCROLL,MAKEWPARAM(nSBCode,nPos),
		(LPARAM)(pScrollBar!=NULL ? pScrollBar->GetSafeHwnd() : NULL));
}

/***************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over main grid area
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Return:
	Context help ID to be displayed
*****************************************************/
LRESULT CUGGrid::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col;
	long row;
	CRect rect(0,0,0,0);
	int xPos = LOWORD(lParam),
		yPos = HIWORD(lParam);
	m_ctrl->GetCellFromPoint( xPos, yPos, &col, &row, &rect );
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_GRID );
}

/***************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) in main grid area
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Return:
	TRUE or FALSE to allow further processing of this message
*****************************************************/
BOOL CUGGrid::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// this message is fired as result of mouse activated help in a dialog
		int col;
		long row;

		if ( GetKeyState( VK_F1 ) < 0 )
		{
			col = m_ctrl->GetCurrentCol();
			row = m_ctrl->GetCurrentRow();
		}
		else
		{
			CRect rect(0,0,0,0);
			CPoint point;
			point.x = pHelpInfo->MousePos.x;
			point.y = pHelpInfo->MousePos.y;

			ScreenToClient( &point );
			int xPos = point.x,
				yPos = point.y;
			m_ctrl->GetCellFromPoint( xPos, yPos, &col, &row, &rect );
		}

		DWORD dwTopicID = m_ctrl->OnGetContextHelpID( col, row, UG_GRID );
		if ( dwTopicID == 0 )
			return FALSE;

		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( dwTopicID );
		// prevent further handling of this message
		return TRUE;
	}
	return FALSE;
}

LRESULT CUGGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_THEMECHANGED:
		// This function closes all open handles, which will force them all to reload, using the new theme.
		UGXPThemes::CleanUp();
		return 0;
	default:
		return CWnd::WindowProc(message, wParam, lParam);
	}
}

BOOL CUGGrid::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	// Don't erase background when using themes.
	return UGXPThemes::IsThemed();
}