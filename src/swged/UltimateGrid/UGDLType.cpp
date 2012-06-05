/*************************************************************************
				Class Implementation : CUGDropListType
**************************************************************************
	Source file : UGDLType.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/

#include "stdafx.h"
#include "UGCtrl.h"
//#include "UGDLType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
CUGDropListType - Constructor
	Initialize all member variables
***************************************************/
CUGDropListType::CUGDropListType()
{
	//set up the variables
	m_pen.CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNFACE));
	m_brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	m_btnWidth = GetSystemMetrics(SM_CXVSCROLL);

	m_btnDown = FALSE;

	m_btnCol = -1;
	m_btnRow = -1;

	// create the list control
	m_listBox = new CUGLstBox;
	// make sure that this cell types do not overlap
	m_canOverLap = FALSE;
	m_useThemes = true;
}

/***************************************************
~CUGDropListType - Denstructor
	Clean up all allocated resources
***************************************************/
CUGDropListType::~CUGDropListType()
{
	// clean up, delete the listbox object
	delete m_listBox;
}

/***************************************************
GetName  - overloaded CUGCellType::GetName
	Returns a readable name for the cell type.
	Returned value is used to help end-users
	to see what cell type are available.

    **See CUGCellType::GetName for more details
	about this function

Params:
	<none>
Return:
	cell type name
****************************************************/
LPCTSTR CUGDropListType::GetName()
{
	return _T("DropList");
}

/***************************************************
GetUGID  - overloaded CUGCellType::GetUGID
	Returns a GUID for the cell type, this number
	is unique for each cell type and never changes.
	This number can be used to find the cell types
	added to this instance of the Ultimate Grid.

    **See CUGCellType::GetUGID for more details
	about this function

Params:
	<none>
Returns:
	UGID (which is actually a GUID)
****************************************************/
LPCUGID CUGDropListType::GetUGID()
{
	static const UGID ugid = { 0x1667a6a0, 0xf746, 
							0x11d0, 0x9c, 0x7f, 
							0x0, 0x80, 0xc8, 0x3f, 
							0x71, 0x2f };

	return &ugid;
}

/***************************************************
GetEditArea  - overloaded CUGCellType::GetEditArea
	Returns the editable area of a cell type.
	Some celltypes (i.e. drop list) require to
	have certain portion of the cell not covered
	by the edit control.  In case of the drop list
	the drop button should not be covered.

    **See CUGCellType::GetEditArea for more details
	about this function

Params:
	rect - pointer to the cell rectangle, it can
		   be used to modify the edit area
Return:
	UG_SUCCESS or UG_ERROR, currently the Utltimate
	Grid does not check the return value.
****************************************************/
int CUGDropListType::GetEditArea(RECT *rect)
{	
	rect->right -=(m_btnWidth+1);

	return UG_SUCCESS;
}

/****************************************************
GetBestSize
	Returns the best (nominal) size for a cell using
	this cell type, with the given cell properties.
Params:
	dc		- device context to use to calc the size on	
	size	- return the best size in this param
	cell	- pointer to a cell object to use for the calc.
Return:
	<none>
*****************************************************/
void CUGDropListType::GetBestSize(CDC *dc,CSize *size,CUGCell *cell)
{
	CUGCellType::GetBestSize( dc, size, cell );
	// in the case of the drop list celltype we will take the normal
	// width of the text selected, and if the drop button is vible,
	// we will increate the width by the width of the button.
	if (!( cell->GetCellTypeEx()&UGCT_DROPLISTHIDEBUTTON ))
	{
		size->cx += m_btnWidth;
	}
}

/***************************************************
OnDClicked - overloaded CUGCellType::OnDClicked
	This function is called when the left mouse 
	button is double clicked over a cell.
	The drop list cell type determines if the 
	user double clicks on the area that is covered
	by the drop list button, if this is the case than
	this event is passed on to the OnLClicked.

    **See CUGCellType::OnDClicked for more details
	about this function

Params:
	col - column that was clicked in
	row - row that was clicked in
	rect - rectangle of the cell that was clicked in
	point - point where the mouse was clicked
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGDropListType::OnDClicked(int col,long row,RECT *rect,POINT *point)
{
	if(point->x > (rect->right - m_btnWidth))
		return OnLClicked(col,row,1,rect,point);
	
	return FALSE;
}

/***************************************************
OnLClicked - overloaded CUGCellType::OnLClicked
	This function is called when the left mouse 
	button is clicked over a cell.
	The drop list cell type uses this event to
	show the drop list control when the user
	clicks on the drop list button.
	The list control will also be hiden when
	user clicks on the same cell while it is
	showing.
   	
	**See CUGCellType::OnLClicked for more details
	about this function

Params:
	col - column that was clicked in
	row - row that was clicked in
	updn - TRUE if the mouse button just went down
		 - FALSE if the mouse button just went up
	rect - rectangle of the cell that was clicked in
	point - point where the mouse was clicked
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGDropListType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point)
{	
	if( m_ctrl->m_GI->m_multiSelectFlag > 0 && ( GetKeyState( VK_SHIFT ) < 0 || GetKeyState( VK_CONTROL ) < 0 ))
		// The drop list should not process mouse click events when
		// user selects cells.
		return TRUE;

	if(updn)
	{
		if(point->x > (rect->right - m_btnWidth))
		{			
			if(col == m_btnCol && row == m_btnRow)
			{
				m_btnCol = -1;
				m_btnRow = -1;			
				if(m_listBox->GetSafeHwnd() != NULL)
					m_listBox->DestroyWindow();
			}
			else
			{
				//copy the droplist button co-ords
				CopyRect(&m_btnRect,rect);
				m_btnRect.left = rect->right - m_btnWidth;
			
				//redraw the button
				m_btnDown = TRUE;
				m_btnCol = col;
				m_btnRow = row;
				m_ctrl->RedrawCell(m_btnCol,m_btnRow);

				//start the drop list
				StartDropList();
				return TRUE;
			}
		}
		else if(m_btnCol ==-2)
		{
			m_btnCol = -1;
			m_btnRow = -1;			
			return FALSE;
		}
	}
	else if(m_btnDown)
	{
		m_btnDown = FALSE;
		m_ctrl->RedrawCell(col,row);
		return TRUE;
	}

	return FALSE;
}

/***************************************************
OnMouseMove - overloaded CUGCellType::OnMouseMove
	This function is called when the mouse  is over
	a cell of this celltype.
	The Drop list cell type uses this notification
	to visually represent to the user that the left
	mouse button was pressed over the button area,
	but it now has been moved on and the list will
	not be shown if the button is released.

   	**See CUGCellType::OnMouseMove for more details
	about this function

Params:
	col - column that the mouse is over
	row - row that the mouse is over
	point - point where the mouse is
	flags - mouse move flags (see WM_MOUSEMOVE)
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
***************************************************/
BOOL CUGDropListType::OnMouseMove(int col,long row,POINT *point,UINT flags)
{	
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);

	if((flags&MK_LBUTTON) == FALSE)
		return TRUE;

	if(point->x >= m_btnRect.left && point->x <= m_btnRect.right)
	{
		if(point->y >= m_btnRect.top && point->y <= m_btnRect.bottom)
		{
			if(!m_btnDown)
			{
				m_btnDown = TRUE;
				m_ctrl->RedrawCell(m_btnCol,m_btnRow);
			}
		}
		else if(m_btnDown)
		{
			m_btnDown = FALSE;
			m_ctrl->RedrawCell(m_btnCol,m_btnRow);
		}
	}
	else if(m_btnDown)
	{
		m_btnDown = FALSE;
		m_ctrl->RedrawCell(m_btnCol,m_btnRow);
	}

	return FALSE;
}

/***************************************************
OnKillFocus - overloaded CUGCellType::OnKillFocus
	This function is called when a the drop list cell type
	looses focus.

   	**See CUGCellType::OnKillFocus for more details
	about this function

Params:
	col - column that just lost focus
	row - row that just lost focus
	cell - pointer to the cell object located at col/row
Return:
	<none>
****************************************************/
void CUGDropListType::OnKillFocus(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(cell);

	m_btnCol = -1;
	m_btnRow = -1;

	if ( m_listBox->GetSafeHwnd() != NULL )
	{
		m_listBox->ShowWindow( SW_HIDE );
		m_listBox->DestroyWindow();
	}
}

/***************************************************
OnKeyDown - overloaded CUGCellType::OnKeyDown
	This function is called when a cell of this type
	has focus and a key is pressed.(See WM_KEYDOWN)
	The drop list cell type allows the user to show
	the list with either an Enter key or the CTRL-Down
	arrow combination.

  	**See CUGCellType::OnKeyDown for more details
	about this function

Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code, of the key that was pressed
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGDropListType::OnKeyDown(int col,long row,UINT *vcKey)
{	
	if((*vcKey==VK_RETURN) || ((*vcKey==VK_DOWN)&&(GetKeyState(VK_CONTROL) < 0)))
	{
		m_btnCol = col;
		m_btnRow = row;
		StartDropList();
		*vcKey =0;
		return TRUE;
	}

	return FALSE;
}

/***************************************************
OnCharDown  - overloaded CUGCellType::OnCharDown
	The drop list only handles this notification to inform the 
	grid that the enter and down arrow keys were already handled.

	**See CUGCellType::OnCharDown for more details
	about this function

Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code, of the key that was pressed
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGDropListType::OnCharDown(int col,long row,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);

	if(*vcKey==VK_RETURN)
		return TRUE;
	if(*vcKey==VK_DOWN)
		return TRUE;
	return FALSE;
}

/***************************************************
OnDraw - overloaded CUGCellType::OnDraw
	The drop list cell type draws its text using the
	base classes DrawText routine, plus draws the
	drop down button

    **See CUGCellType::OnDraw for more details
	about this function
Params
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	col		- column that is being drawn
	row		- row that is being drawn
	cell	- cell that is being drawn
	selected- TRUE if the cell is selected, otherwise FALSE
	current - TRUE if the cell is the current cell, otherwise FALSE
Return
	none
****************************************************/
void CUGDropListType::OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current)
{
	if (!m_drawThemesSet)
		m_useThemes = cell->UseThemes();

	int left = rect->left;
	RECT rectout;
	CPen * oldpen;

	int style = 0;
	if(cell->IsPropertySet(UGCELL_CELLTYPEEX_SET))
		style = cell->GetCellTypeEx();

	//if the cell is not current and hide button is on
	//then dont draw the button
	if(style&UGCT_DROPLISTHIDEBUTTON && !current){
		CUGCellType::OnDraw(dc,rect,col,row,cell,selected,current);
		return;
	}

	float fScale = 1.0;

#ifdef UG_ENABLE_PRINTING
	fScale = m_ctrl->GetUGPrint()->GetPrintVScale(dc);
#endif

	RECT rcCombo = *rect;

	rcCombo.left = rcCombo.right - (int) (fScale * m_btnWidth);


	if(!m_useThemes || !UGXPThemes::DrawBackground(NULL, *dc, XPCellTypeData, UGXPThemes::GetState(selected > 0, current > 0), &rcCombo, NULL)
		|| !UGXPThemes::DrawBackground(NULL, *dc, XPCellTypeCombo, UGXPThemes::GetState(selected > 0, current > 0), &rcCombo, NULL))
	{

		DrawBorder(dc,rect,rect,cell);

		// The printer and the monitor have different resolutions.
		// So we should adjust the size of the button.

		rect->left = rect->right - (int) (fScale * m_btnWidth);

#ifdef UG_ENABLE_PRINTING
		if (dc->IsPrinting())
		{
			rect->left += (int) fScale;
			rect->right -= (int) fScale;
			rect->top += (int) fScale;
			rect->bottom -= (int) fScale;
		}
#endif

		// draw the 3D border

		if(m_btnDown && current){
			cell->SetBorder(UG_BDR_RECESSED);
			DrawBorder(dc,rect,&rectout,cell);
		}
		else{
			cell->SetBorder(UG_BDR_RAISED);
			DrawBorder(dc,rect,&rectout,cell);
		}

		//fill the border in
		dc->FillRect(&rectout,&m_brush);
		
		//make a line to separate the border from the rest ofthe cell
		oldpen = (CPen *)dc->SelectObject((CPen *)&m_pen);
		dc->MoveTo(rect->left-1,rect->top);
		dc->LineTo(rect->left-1,rect->bottom);
		dc->SelectObject(oldpen);

		//draw the down arrow
		if (dc->IsPrinting())
		{
#ifdef UG_ENABLE_PRINTING
			CRgn rgn;

			int	nWidth = rect->right - rect->left;
			int	nHeight = rect->bottom - rect->top;

			POINT point[] = {
				{rect->left + nWidth * 3 / 10, rect->top + nHeight * 5 / 12} ,
				{rect->left + nWidth * 7 / 10, rect->top + nHeight * 5 / 12},
				{rect->left + nWidth / 2, rect->top + nHeight * 7 / 12}
			};

			rgn.CreatePolygonRgn(point, 3, ALTERNATE);

			CBrush Brush;
			Brush.CreateSolidBrush(RGB(0,0,0));
			dc->FillRgn(&rgn, &Brush);

			dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN)));
			dc->MoveTo(rect->left,rect->top);
			dc->LineTo(rect->right,rect->top);
#endif
		}
		else
		{
			int x= (int) ((fScale * m_btnWidth-5)/2) + rect->left;
			int y = ((rect->bottom - rect->top -3)/2) + rect->top;

			// create a pen object that will be used to draw the arrow on the button
			CPen *arrowPen = NULL;
			if ( cell->GetReadOnly() == TRUE )
			{
				arrowPen = new CPen;
				arrowPen->CreatePen( PS_SOLID, 1, RGB(128,128,128));
			}
			else
				arrowPen = (CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN));

			oldpen = dc->SelectObject( arrowPen );

			// draw the arrow
			dc->MoveTo(x,y);
			dc->LineTo(x+5,y);
			dc->MoveTo(x+1,y+1);
			dc->LineTo(x+4,y+1);
			dc->MoveTo(x+2,y+2);
			dc->LineTo(x+2,y+1);

			if ( cell->GetReadOnly() == TRUE )
			{
				// clean up after temporary pen object
				dc->SelectObject(oldpen);
				arrowPen->DeleteObject();
				delete arrowPen;
				// when the arrow is disabled, add a while outline line
				dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN)));
				dc->MoveTo(x+3,y+2);
				dc->LineTo(x+5,y);
				dc->MoveTo(x+3,y+3);
				dc->LineTo(x+6,y);
			}
		}
	}

	//draw the text in using the default drawing routine
	rect->left = left;
	rect->right -= (int) (fScale * m_btnWidth+1);

	DrawText(dc,rect,0,col,row,cell,selected,current);
}

/***************************************************
StartDropList
	This internal function is used to parse out 
	the label string, which contains the items to 
	show in the list.  Also this function will
	show the list box in the proper location on the
	screen.
Params:
	<none>
Return:
	UG_ERROR	- the list is already visible.
	UG_SUCCESS	- everything went smoothly, the drop
				  list was shown
***************************************************/
int CUGDropListType::StartDropList()
{
	RECT		rect;
	RECT		clientRect;
	int			dif,len,pos,startpos;
	CFont *		font = NULL;
	LOGFONT		lf;
	CFont *		cfont;
	CStringList list;	  //lists of strings for the list are assembed here
	CString *	items;	  //points to the string used to assemble the list

	//return false if it is already up
	if(	m_listBox->m_HasFocus)
		return UG_ERROR;

	CUGCell cell;
	m_ctrl->GetCellIndirect(m_btnCol,m_btnRow,&cell);

	// make sure that the read only cells do not show droplist
	if ( cell.GetReadOnly() == TRUE )
		return UG_SUCCESS;

	//get the current row and col
	m_btnCol  = m_ctrl->GetCurrentCol();
	m_btnRow = m_ctrl->GetCurrentRow();

	//add the strings to the CStringList
	CString string;
	cell.GetLabelText(&string);
	items = &string;
	len = items->GetLength();
	pos =0;
	startpos =0;
	while(pos <len){
		if(items->GetAt(pos)== _T('\n')){
			list.AddTail(items->Mid(startpos,pos-startpos));
			startpos = pos+1;
		}
		pos++;
	}

	//notify the user of the list, so it can be modified if needed
	if ( OnCellTypeNotify(m_ID,m_btnCol,m_btnRow,UGCT_DROPLISTSTART,(LONG_PTR)&list) == FALSE )
		// if FALSE was returned from OnCellTypeNotify call, than the developer does not
		// wish to show the drop list at the moment.
		return UG_ERROR;
	
	// set default font height
	lf.lfHeight = 12;

	//get the font
	if(cell.IsPropertySet(UGCELL_FONT_SET))
		font = cell.GetFont();
	else if(m_ctrl->m_GI->m_defFont != NULL)
		font = m_ctrl->m_GI->m_defFont;
	
	if(font == NULL)
		font = CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FONT));

	if(font != NULL)
	{
		//find the height of the font
		cfont = font;
		cfont->GetLogFont(&lf); 	// lf.lfHeight

		if( lf.lfHeight < 0 ){
			HDC hDC = CreateCompatibleDC(NULL);
			lf.lfHeight = -MulDiv(lf.lfHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
			DeleteDC(hDC);
		}
	}

	//get the rectangle for the listbox
	m_ctrl->GetCellRect(m_btnCol,m_btnRow,&rect);
	rect.top = rect.bottom;
	rect.left+=10;
	rect.right+=10;
	len = (int)list.GetCount();
	if(len >15)
		len = 15;
	rect.bottom += lf.lfHeight * len + 6;
	
	m_ctrl->m_CUGGrid->GetClientRect(&clientRect);
	if(rect.bottom > clientRect.bottom){
		dif = rect.bottom - clientRect.bottom;
		rect.bottom -= dif;
		rect.top -= dif;
		if(rect.top <0)
			rect.top = 0;
	}
	if(rect.right > clientRect.right){
		dif = rect.right - clientRect.right;
		rect.right -= dif;
		rect.left -= dif;
		if(rect.left <0)
			rect.left = 0;
	}

	//create the CListBox
	m_listBox->m_ctrl = m_ctrl;
	m_listBox->m_cellType = this;
	m_listBox->m_cellTypeId = m_ID;
	m_listBox->Create(WS_CHILD|WS_BORDER|WS_VSCROLL,rect,m_ctrl->m_CUGGrid,123);
	
	//set up the font
	if(font != NULL)
		m_listBox->SetFont(font);
	
	//resize the window again since a new font is being used
	m_listBox->MoveWindow(&rect,FALSE);

	//add the items to the list
	len = (int)list.GetCount();
	POSITION position = list.GetHeadPosition();
	pos =0;
	while(pos < len){
		m_listBox->AddString(list.GetAt(position));
		pos++;
		if(pos < len)
			list.GetNext(position);
	}

	//give the list box pointers to the cell
	m_listBox->m_col = &m_btnCol;
	m_listBox->m_row = &m_btnRow;

	m_listBox->ShowWindow(SW_SHOW);
	m_listBox->SetFocus();
	
	m_btnDown = FALSE;
	m_ctrl->RedrawCell(m_btnCol,m_btnRow);

	return UG_SUCCESS;
}
