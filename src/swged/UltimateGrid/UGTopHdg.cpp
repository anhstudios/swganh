/*************************************************************************
				Class Implementation : CUGTopHdg
**************************************************************************
	Source file : UGTopHdg.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"
#include "UGCell.h"
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
CUGTopHdg::CUGTopHdg()
{
	//init the varialbes
	m_isSizing	= FALSE;
	m_canSize	= FALSE;
	
	//set the last focus rect position
	m_focusRect.left	= -1;
	m_focusRect.right	= -1;
	m_focusRect.top		= -1;
	m_focusRect.bottom	= -1;

	m_swapStartCol = -1;	//columns to swap
	m_swapEndCol = -1;	
}

CUGTopHdg::~CUGTopHdg()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGTopHdg, CWnd)
	//{{AFX_MSG_MAP(CUGTopHdg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_HELPINFO()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, ToolTipNeedText )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
OnPaint
	This routine is responsible for gathering information on cells to draw,
	and draw in an optimized fashion.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGTopHdg::OnPaint() 
{
	if ( m_GI->m_paintMode == FALSE )
		return;

	CPaintDC dc(this); // device context for painting
	
	m_drawHint.AddHint(0,m_GI->m_numberTopHdgRows * -1,m_GI->m_numberCols,0);

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
void CUGTopHdg::DrawCellsIntern(CDC *dc)
{	
	CRect rect(0,0,0,0), cellRect;
  	CUGCell cell;
	CUGCellType * cellType;
	int dcID;
	int xIndex,col;
	long yIndex,row;
	
	m_ctrl->OnScreenDCSetup(dc,NULL,UG_TOPHEADING);
	
	//set the default font
	if(m_GI->m_defFont != NULL)
		dc->SelectObject((CFont *) m_GI->m_defFont);

	int blankRight = 0;

	for(yIndex = (m_GI->m_numberTopHdgRows * -1); yIndex < 0 ; yIndex++)
	{
		row = yIndex;
	
		for(xIndex = 0;xIndex < m_GI->m_numberCols;xIndex++)
		{
			if(xIndex == m_GI->m_numLockCols)
				xIndex = m_GI->m_leftCol;
			col = xIndex;
			row = yIndex;
	
			//draw if invalid
			if(m_drawHint.IsInvalid(col,row) != FALSE)
			{
				GetCellRect(col,row,&rect);
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
					
				if(cellRect.left < cellRect.right)
				{
					cellType = m_ctrl->GetCellType(cell.GetCellType());

					dcID = dc->SaveDC();
					
					if(m_swapEndCol >=0 && col == m_swapStartCol)
						cellType->OnDraw(dc,&cellRect,col,row,&cell,1,0);
					else
						cellType->OnDraw(dc,&cellRect,col,row,&cell,0,0);

					dc->RestoreDC(dcID);
				}
			}
			if(rect.right > m_GI->m_gridWidth)
				break;
		}
		if(blankRight < rect.right)
			blankRight = rect.right;
	}
	if(blankRight < m_GI->m_gridWidth)
	{
		rect.top = 0;
		rect.bottom = m_GI->m_topHdgHeight;
		rect.left = blankRight;
		rect.right = m_GI->m_gridWidth;
		// fill-in the area that is not covered by cells
		CBrush brush( m_ctrl->OnGetDefBackColor( UG_TOPHEADING ));
		dc->FillRect( rect, &brush );
	}
}

/************************************************
Update 
	function makes sure that the last row in the top heading has proper
	height before the window is redrawn.
Params:
	<none>
Return:
	<none>
*************************************************/
void CUGTopHdg::Update()
{	
	//calc the last row height
	//find the row
	int height = 0;

	// For VC6/2002/2003/2005 compatibility
	int yIndex= -1;

	for(;yIndex > (m_GI->m_numberTopHdgRows * -1) ;yIndex--)
	{
		height += GetTHRowHeight(yIndex);
	}

	height = m_GI->m_topHdgHeight - height;

	if(height < 0)
		height = 0;

	m_ctrl->SetTH_RowHeight(yIndex,height);

	//redraw the window
	InvalidateRect(NULL);
	UpdateWindow();
}

/************************************************
Moved
	function makes sure that the grid view has moved horizontally
	before the window update is allowed.  With this check the Ultimate
	Grid eliminates un-necessary redraws.
Params:
	<none>
Returns:
	<none>
*************************************************/
void CUGTopHdg::Moved()
{
	if(m_GI->m_leftCol == m_GI->m_lastLeftCol)
		return;

	//redraw the window
	InvalidateRect(NULL);
	UpdateWindow();
}

/***************************************************
CheckForUserResize
	function is called when user clicks the left mouse button or moves the
	mouse cursor over the area in the top heading that would allow column
	or row sizing.  This function is used to determine if sizing is allowed.
	Instead of having a return value, this function sets member vairables
	to proper state.
Params:
	point		- pos of the mouse pointer
Returns:
	<none>
*****************************************************/
void CUGTopHdg::CheckForUserResize(CPoint *point)
{
	if(m_GI->m_userSizingMode == FALSE)
		return;

	//top heading column sizing
	int width = 0;
	for(int col = 0;col < m_GI->m_numberCols;col++)
	{	
		if(col == m_GI->m_numLockCols && col < m_GI->m_leftCol)
			col = m_GI->m_leftCol;
		
		width += m_ctrl->GetColWidth(col);
		if(width > m_GI->m_gridWidth)
			break;

		if(point->x < width+3 && point->x > width-3)
		{	
			if(m_ctrl->GetColWidth(col+1) == 0 && (col+1) < m_GI->m_numberCols)
				col++;

			if(m_ctrl->OnCanSizeCol(col) == FALSE)
				return;

			m_canSize = TRUE;
			m_colOrRowSizing	= 0;				// 0-col 1-row
			m_sizingColRow		= col;				//column/row being sized
			m_sizingStartSize	= m_ctrl->GetColWidth(col);//original size
			m_sizingStartPos	= point->x;			//original start pos

			SetCursor(m_GI->m_WEResizseCursor);
			return;
		}
	}

	//top heading row sizing
	int height = m_GI->m_topHdgHeight;
	for(int row = 0; row < m_GI->m_numberTopHdgRows; row++)
	{		
		if(point->y < height+3  && point->y > height-3)
		{		
			if(m_ctrl->OnCanSizeTopHdg() == FALSE)
				return;
			
			m_canSize = TRUE;
			m_colOrRowSizing	= 1;				// 0-col 1-row
			m_sizingColRow		= row;				//column/row being sized
			m_sizingStartSize	= m_GI->m_topHdgHeights[row];//original size
			m_sizingStartPos	= point->y;			//original start pos
			m_sizingStartHeight = m_GI->m_topHdgHeight;

			SetCursor(m_GI->m_NSResizseCursor);
			return;
		}
		height -= m_GI->m_topHdgHeights[row];
	}

	if(m_canSize)
	{
		m_canSize = FALSE;
		SetCursor(m_GI->m_arrowCursor);
	}
}
/************************************************
OnMouseMove
	Checks to see if the mouse is over a cell separation line, if it is then it
	checks to see if sizing is allowed.  If it is then the cusor is changed to
	the sizing cusror.
	If sizing is in progress then column width or top heading row height are
	updated.
	This is also where the column swapping operation is handled.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Return:
	<none>
*************************************************/
void CUGTopHdg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//check to see if the mouse is over a cell separation 
	//if the mouse is not currently sizing
	if(m_isSizing == FALSE && (nFlags&MK_LBUTTON) == 0 && m_GI->m_userSizingMode >0)
	{
		//check for user resize position
		CheckForUserResize(&point);		
	}
	else if(m_isSizing)
	{
		if(m_colOrRowSizing == 0)	//col sizing
		{
			if(point.x < m_sizingStartPos - m_sizingStartSize)
				point.x = m_sizingStartPos - m_sizingStartSize;

			int width = m_sizingStartSize + (point.x - m_sizingStartPos);

			// send notifications to cell types.  In order to provide acceptable performance
			// this notification will only be sent to the visible rows.
			for(int nIndex=m_ctrl->GetTopRow(); nIndex<m_ctrl->GetBottomRow(); nIndex++)
			{
				CUGCellType* pCellType=m_ctrl->GetCellType(m_sizingColRow,nIndex);
				if(pCellType!=NULL)
				{
					pCellType->OnChangingCellWidth(m_sizingColRow,nIndex,&width);
				}
			}
			//send notification to control
			m_ctrl->OnColSizing(m_sizingColRow,&width);

			//just draw a focus rect
			if(m_GI->m_userSizingMode == 1)
			{
				m_ctrl->SetColWidth(m_sizingColRow,width);
				Update();
				
				CDC* dc = m_ctrl->m_CUGGrid->GetDC();
				dc->DrawFocusRect(&m_focusRect);
				m_focusRect.top = 0;
				m_focusRect.bottom = m_GI->m_gridHeight;
				m_focusRect.left = point.x-1;
				m_focusRect.right = point.x+1;
				dc->DrawFocusRect(&m_focusRect);
				m_ctrl->m_CUGGrid->ReleaseDC(dc);

			}
			else	//update on the fly
			{
				m_ctrl->SetColWidth(m_sizingColRow,width);
				m_ctrl->RedrawAll();				
			}
		}
		else	//row sizing
		{
			int height = m_sizingStartSize + (point.y - m_sizingStartPos);
			if(height <0)
				height = 0;
			m_GI->m_topHdgHeights[m_sizingColRow] = height;

			height = m_sizingStartHeight + (point.y - m_sizingStartPos);
			if(height <0)
				height = 0;

			if(m_ctrl->OnTopHdgSizing(&height) == TRUE)
			{
				m_GI->m_topHdgHeight = height;
				m_ctrl->AdjustComponentSizes();
			}
		}
	}
	//check for column swapping
	else if(m_GI->m_enableColSwapping && m_swapStartCol >= 0)
	{	
		MSG msg;
		
		//while column swapping enable mouse scrolling of the grid
		if(point.x < 0 || point.x > m_GI->m_gridWidth)
		{	
			//remove the focus rectangle
			CDC* dc = m_ctrl->m_CUGGrid->GetDC();
			dc->DrawFocusRect(&m_focusRect);
			m_focusRect.left	= -1;
			m_focusRect.right	= -1;
			m_ctrl->m_CUGGrid->ReleaseDC(dc);		

			while(1)
			{
				if(point.x < 0)
					m_ctrl->MoveLeftCol(UG_LINEUP);
				else if(point.x > m_GI->m_gridWidth)
					m_ctrl->MoveLeftCol(UG_LINEDOWN);

				//check for messages, if ther are none then scroll some more
				while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{
					if(msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONUP)
						return;

					GetMessage(&msg,NULL,0,0);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else
		{
			int col,row;
			RECT rect;

			//find the column that the mouse is over
			if(GetCellFromPoint(&point,&col,&row,&rect) != UG_SUCCESS)
				return;

			//only start swapping if the mouse has moved over
			//a different column since the mouse button was pressed
			if(col != m_swapStartCol || m_swapEndCol >=0)
			{	
				if(( point.x - rect.left) > ((rect.right - rect.left)/2))
				{
					col ++;
					rect.left = rect.right;
				}

				if(col > m_GI->m_numberCols)
					col = m_GI->m_numberCols;

				//the firt time this is called redraw the top heading
				//so that the startswap cell is updated
				if(m_swapEndCol < 0)
				{
					m_swapEndCol = col;
					Update();
				}

				//store the current col number
				m_swapEndCol = col;

				//draw a focus rect showing where the swap will
				//take place
				CDC* dc = m_ctrl->m_CUGGrid->GetDC();
				dc->DrawFocusRect(&m_focusRect);
				m_focusRect.top = 0;
				m_focusRect.bottom = m_GI->m_gridHeight;
				m_focusRect.left = rect.left;
				m_focusRect.right = rect.left+2;
				dc->DrawFocusRect(&m_focusRect);
				m_ctrl->m_CUGGrid->ReleaseDC(dc);		
			}
		}
	}
}

/************************************************
OnLButtonDown
	Finds the cell that was clicked in, sends a notification
	updates the current cells position.
	It also sets mouse capture, which will later be used during column
	swaping operation.
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*************************************************/
void CUGTopHdg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int col;
	int row;
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
		//store the column where the button was pressed
		//just in case a swap is to take place
		if(m_ctrl->OnColSwapStart(col) != FALSE)
			m_swapStartCol = col;

		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,1,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnTH_LClicked(col,row,1,&rect,&point,processed);
	}
	
	SetCapture();
}

/************************************************
OnLButtonUp
	Finds the cell that was clicked in
	Sends a notification to CUCtrl::OnTH_LClicked
	Releases the mouse capture, and completes column swapping (if enabled)
Params:
	nFlags		- please see MSDN for more information on the parameters.
	point
Returns:
	<none>
*************************************************/
void CUGTopHdg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int col;
	int row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(m_isSizing){

		if(m_colOrRowSizing == 0)	//col sizing
		{
			//send notifications
			int width = m_ctrl->GetColWidth(m_sizingColRow);
			// send notifications to cell types.  In order to provide acceptable performance
			// this notification will only be sent to the visible rows.
			for(int nIndex=m_ctrl->GetTopRow(); nIndex<m_ctrl->GetBottomRow(); nIndex++)
			{
				CUGCellType* pCellType=m_ctrl->GetCellType(m_sizingColRow,nIndex);
				if(pCellType!=NULL)
				{
					pCellType->OnChangedCellWidth(m_sizingColRow,nIndex,&width);
				}
			}
			m_ctrl->OnColSized(m_sizingColRow,&width);
			if(width != m_ctrl->GetColWidth(m_sizingColRow))
				m_ctrl->SetColWidth(m_sizingColRow,width);
		}
		else
		{
			m_ctrl->OnTopHdgSized(&m_GI->m_topHdgHeight);
		}
		m_isSizing			= FALSE;

		//send notification
		m_ctrl->m_CUGGrid->InvalidateRect(NULL);
		m_ctrl->AdjustComponentSizes();
	}	
	else if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnLClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnTH_LClicked(col,row,0,&rect,&point,processed);

	}

	//column swapping
	if(m_GI->m_enableColSwapping && m_swapStartCol >= 0)
	{	
		int end = m_swapEndCol;
		if(m_swapStartCol < end) // this needs to be done since the internal
			end --;				 // calc. does not take into account that the 
								 // start col will be removed and all the cols
								 // are going to slide over one position
		if(m_ctrl->OnCanColSwap(m_swapStartCol,end) != FALSE)
		{
			m_ctrl->MoveColPosition(m_swapStartCol,m_swapEndCol,TRUE);

			// send OnColSwapped notification to inform user that the swap is completed
			m_ctrl->OnColSwapped(m_swapStartCol,end);

			//only redraw if swapping or a potential swap took place
			if(m_swapEndCol >=0)
			{
				m_swapEndCol = -1;	
				m_ctrl->RedrawAll();
			}
			else
			{
				Invalidate();
			}
		}
		else
		{
			m_swapEndCol = -1;
			m_ctrl->RedrawAll();
		}
	}

	//reset variables
	m_swapStartCol		= -1;
	m_swapEndCol		= -1;	
	m_focusRect.left	= -1;
	m_focusRect.right	= -1;

	ReleaseCapture();
}

/************************************************
OnLButtonDblClk
	Finds the cell that was clicked in and sends a notification to
	CUCtrl::OnTH_DClicked.
	If the user double clicked on the column separator area than calls
	BestFit on 20 rows starting on the top most visible row.  Once this
	operation is completed the OnChangedCellWidth, and OnColSized 
	notifications will be sent.
Params:
	<none>
Returns:
	<none>
*************************************************/
void CUGTopHdg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int		col,
			row;
	RECT	rect;
	BOOL	processed = FALSE;

	UNREFERENCED_PARAMETER(nFlags);

	if(m_canSize)
	{	
		//check to see if the column should be BestFit
		if(m_GI->m_userBestSizeFlag)
		{
			m_ctrl->BestFit(m_sizingColRow,m_sizingColRow,20,UG_BESTFIT_TOPHEADINGS);
			CheckForUserResize(&point);

			//send notifications
			int width = m_ctrl->GetColWidth(m_sizingColRow);
			// send notifications to cell types.  In order to provide acceptable performance
			// this notification will only be sent to the visible rows.
			for(int nIndex=m_ctrl->GetTopRow(); nIndex<m_ctrl->GetBottomRow(); nIndex++)
			{
				CUGCellType* pCellType=m_ctrl->GetCellType(m_sizingColRow,nIndex);
				if(pCellType!=NULL)
				{
					pCellType->OnChangedCellWidth(m_sizingColRow,nIndex,&width);
				}
			}
			m_ctrl->OnColSized(m_sizingColRow,&width);
			if(width != m_ctrl->GetColWidth(m_sizingColRow))
				m_ctrl->SetColWidth(m_sizingColRow,width);

			m_isSizing = FALSE;
		}
	}
	else if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		processed = m_ctrl->GetCellType(col,row)->OnDClicked(col,row,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnTH_DClicked(col,row,&rect,&point,processed);
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
Return:
	<none>
*************************************************/
void CUGTopHdg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int col;
	int row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetFocus() != m_ctrl->m_CUGGrid)
		m_ctrl->m_CUGGrid->SetFocus();

	if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnRClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnTH_RClicked(col,row,1,&rect,&point,processed);
	}

	if(m_GI->m_enablePopupMenu)
	{
		ClientToScreen(&point);
		m_ctrl->StartMenu(col,row,&point,UG_TOPHEADING);
	}
}

/************************************************
OnRButtonUp
	Sends a mouse click notification to the main grid class.
Params:
	nFlags		- mouse button states
	point		- point where the mouse is
Return:
	<none>
*************************************************/
void CUGTopHdg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	int col;
	int row;
	RECT rect;

	UNREFERENCED_PARAMETER(nFlags);

	if(GetCellFromPoint(&point,&col,&row,&rect) == UG_SUCCESS)
	{
		//send a notification to the cell type	
		BOOL processed = m_ctrl->GetCellType(col,row)->OnRClicked(col,row,0,&rect,&point);
		//send a notification to the main grid class
		m_ctrl->OnTH_RClicked(col,row,0,&rect,&point,processed);
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
BOOL CUGTopHdg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(*pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	if(!m_canSize)
	{
		SetCursor(m_GI->m_arrowCursor);
		return 1;
	}
	else if(m_colOrRowSizing == 0)
		SetCursor(m_GI->m_WEResizseCursor);
	else
		SetCursor(m_GI->m_NSResizseCursor);

	return 1;
	
}

/************************************************
PreCreateWindow
	The Utlimate Grid overwrites this function to further customize the
	top heading window that is created.
Params:
	cs			- please see MSDN for more information on the parameters.
Returns:
	Nonzero if the window creation should continue; 0 to indicate creation failure.
*************************************************/
BOOL CUGTopHdg::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = cs.style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CWnd::PreCreateWindow(cs);
}

/************************************************
GetCellRect
	Returns the rectangle for the given cell co-ordinates.
	If the cell is joined then the given co-ordinates 
	are modified to point to the start cell for the join.
Params
	col		- column to find the rect of
	row		- row to find the rect of
	rect	- rectangle to calculate
Return
	UG_SUCCESS	- success
*************************************************/
int CUGTopHdg::GetCellRect(int col,long row,RECT *rect)
{
	return GetCellRect(&col,&row,rect);
}
/************************************************
GetCellRect
	Returns the rectangle for the given cell co-ordinates.
	If the cell is joined then the given co-ordinates 
	are modified to point to the start cell for the join.
Params
	col		- column to find the rect of
	row		- row to find the rect of
	rect	- rectangle to calculate
Return
	UG_SUCCESS	- success, this function will never fail
*************************************************/
int CUGTopHdg::GetCellRect(int *col,long *row,RECT *rect)
{
	int xIndex,yIndex;
	int width	= 0;
	int height	= 0;

	int startCol	= *col;
	int startRow	= *row;
	int endCol		= *col;
	int endRow		= *row;

	rect->left		= 0;
	rect->top		= 0;
	rect->right		= 0;
	rect->bottom	= m_GI->m_topHdgHeight;
	
	//if the specified cell is within a join then find the joined range
	if(m_GI->m_enableJoins)
	{
		if(GetJoinRange(&startCol,&startRow,&endCol,&endRow) == UG_SUCCESS)
		{
			*col = startCol;
			*row = startRow;
		}
	}

	//find the col
	if(startCol >= m_GI->m_numLockCols)	//if the col is not within the lock region
	{
		rect->left = m_GI->m_lockColWidth;
		rect->right = m_GI->m_lockColWidth;
	}

	for(xIndex=0;xIndex<m_GI->m_numberCols;xIndex++)
	{
		if(xIndex == m_GI->m_numLockCols)
			xIndex = m_GI->m_leftCol;		
		
		if(xIndex == startCol)
			rect->left = width;
		
		width += m_ctrl->GetColWidth(xIndex);

		if(xIndex == endCol)
		{
			rect->right = width;
			break;
		}
	}

	//find the row
	for(yIndex= (m_GI->m_numberTopHdgRows * -1);yIndex < 0;yIndex++)
	{			
		if(yIndex == startRow)
			rect->top = height;

		height += GetTHRowHeight(yIndex);

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
Return:
	UG_SUCCESS	- if a matching cell was found 
	UG_ERROR	- point provided does not correspond to a cell
*************************************************/
int CUGTopHdg::GetCellFromPoint(CPoint *point,int *col,int *row,RECT *rect)
{
	int ptsFound = 0;
	int xIndex,yIndex;

	rect->left		=0;
	rect->top		=0;
	rect->right		=0;
	rect->bottom	=0;

	//find the col
	for(xIndex=0;xIndex<m_GI->m_numberCols;xIndex++)
	{	
		if(xIndex == m_GI->m_numLockCols)
			xIndex = m_GI->m_leftCol;
		
		rect->right += m_ctrl->GetColWidth(xIndex);

		if(rect->right > point->x)
		{
			rect->left = rect->right - m_ctrl->GetColWidth(xIndex);
			ptsFound ++;
			*col = xIndex;
			break;
		}
	}

	//find the row
	for(yIndex = -m_GI->m_numberTopHdgRows; yIndex< 0;yIndex++)
	{
		rect->bottom += GetTHRowHeight(yIndex);

		if(rect->bottom > point->y)
		{
			rect->top = rect->bottom - GetTHRowHeight(yIndex);
			ptsFound ++;
			*row = yIndex;
			break;
		}
	}

	if(ptsFound == 2)
	{
		// check if the found cell is part of a join, if
		// it is than adjust the information found
		int edCol, edRow;

		if ( GetJoinRange( col, row, &edCol, &edRow ) == UG_SUCCESS )
		{
			long tempRow = *row;
			GetCellRect( col, &tempRow, rect );
			*row = tempRow;
		}

		return UG_SUCCESS;
	}

	*col = -1;
	*row = -1;

	return UG_ERROR;
}

/***************************************************
GetJoinRange
	function returns join information related to the cell specified
	in the col and row parameters.  These parameters might be changed
	to represent starting col, row position of the join.
Params:
	col, row	- identify the cell to retrieve join information on,
				  these parameters will represent the starting pos
				  of the join.
	endCol,		- will be set to the end col, row position of the
	endRow		  join.
Returns:
	UG_SUCCESS	- on success
	UG_ERROR	- if joins are disabled, or cell specified is not part of a join
*****************************************************/
int CUGTopHdg::GetJoinRange(int *col,int *row,int *endCol,int *endRow)
{
	if(m_GI->m_enableJoins == FALSE)
		return UG_ERROR;

	int startCol;
	long startRow, joinRow;
	BOOL origin;
	CUGCell cell;

	m_ctrl->GetCellIndirect(*col,*row,&cell);
	if(cell.IsPropertySet(UGCELL_JOIN_SET) == FALSE)
		return UG_ERROR;

	cell.GetJoinInfo(&origin,&startCol,&startRow);
	if(!origin)
	{
		*col += startCol;
		*row += (int)startRow;
		m_ctrl->GetCellIndirect(*col,*row,&cell);
	}

	cell.GetJoinInfo(&origin,endCol,&joinRow);
	*endCol +=*col;
	*endRow = joinRow + *row;

	return UG_SUCCESS;
}

/***************************************************
GetTHRowHeight
	function returns height (in pixels) of the top heading row that user is
	interested in.  The row number passed in has to be a negaitve value ranging
	from zero to negative value of number of top heading rows set.
	((m_GI->m_numberTopHdgRows) * -1)
Params:
	row			- indicates row of interest
Returns:
	height in pixels of the row in question, or zero if
	row not found.
*****************************************************/
int CUGTopHdg::GetTHRowHeight(int row)
{
	//translate the row number into a 0 based positive index
	row = (row * -1) -1;

	if(row <0 || row > m_GI->m_numberTopHdgRows)
		return 0;

	return m_GI->m_topHdgHeights[row];
}

/************************************************
OnHelpHitTest
	Sent as a result of context sensitive help
	being activated (with mouse) over top heading
Params:
	WPARAM - not used
	LPARAM - x, y coordinates of the mouse event
Return:
	Context help ID to be displayed
*************************************************/
LRESULT CUGTopHdg::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	// this message is fired as result of mouse activated
	// context help being hit over the grid.
	int col, row;
	CRect rect(0,0,0,0);
	CPoint point( LOWORD(lParam), HIWORD(lParam));
	GetCellFromPoint( &point, &col, &row, &rect );
	// return context help ID to be looked up
	return m_ctrl->OnGetContextHelpID( col, row, UG_TOPHEADING );
}

/************************************************
OnHelpInfo
	Sent as a result of context sensitive help
	being activated (with mouse) over top heading
	if the grid is on the dialog
Params:
	HELPINFO - structure that contains information on selected help topic
Return:
	TRUE or FALSE to allow further processing of this message
*************************************************/
BOOL CUGTopHdg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		// this message is fired as result of mouse activated help in a dialog
		int col, row;
		CRect rect(0,0,0,0);
		CPoint point;
		point.x = pHelpInfo->MousePos.x;
		point.y = pHelpInfo->MousePos.y;

		ScreenToClient( &point );
		GetCellFromPoint( &point, &col, &row, &rect );
		// call context help with ID returned by the user notification
		AfxGetApp()->WinHelp( m_ctrl->OnGetContextHelpID( col, row, UG_TOPHEADING ));
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
int CUGTopHdg::OnToolHitTest(  CPoint point, TOOLINFO *pTI ) const
{
	int col, row;
	CRect rect;
	static int lastCol = -2;
	static int lastRow = -2;

	if( m_ctrl->m_CUGTopHdg->GetCellFromPoint( &point, &col, &row, &rect ) == UG_SUCCESS)
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
BOOL CUGTopHdg::ToolTipNeedText( UINT id, NMHDR* pTTTStruct, LRESULT* pResult )
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(*pResult);

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;

	static CString string;
	int col;
	int row;
	CPoint point;
	CRect rect;

	GetCursorPos(&point);
	ScreenToClient(&point);

	if ( GetCellFromPoint( &point,&col,&row, &rect ) == UG_SUCCESS )
	{
		if ( m_ctrl->OnHint(col,row,UG_TOPHEADING,&string) == TRUE )
		{
 			pTTT->lpszText = const_cast<LPTSTR>((LPCTSTR)string);
			return TRUE;
		}
	}

	return FALSE;
}