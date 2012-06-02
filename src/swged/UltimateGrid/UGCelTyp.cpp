/*************************************************************************
				Class Implementation : CUGCellType
**************************************************************************
	Source file : UGCelTyp.cpp
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
CUGCellType - Constructor
	Setup the default properties for the cell type
	Cell types contain several member variables 
	that modify how the grid works with the cell type
	NOTE: New cell types must be registered with the
	grid before use.
****************************************************/
CUGCellType::CUGCellType()
{
	m_useThemes = true;
	m_drawThemesSet = false;
	m_canTextEdit = TRUE;
	m_drawLabelText = FALSE;
	m_canOverLap = TRUE;
	m_dScaleFactor = 1.0;
}

/***************************************************
~CUGCellType - Destructor
****************************************************/
CUGCellType::~CUGCellType()
{
}

/***************************************************
DrawTextOrLabel
	function is used to provide a cell type with information identifying
	if it should draw the string set by the SetText of SetLabelText.
Params:
	mode		- identifies text drawing mode
					FALSE (0)	- draw text
					TRUE (1)	- draw label text
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGCellType::DrawTextOrLabel(int mode)
{
	m_drawLabelText = mode;
	return UG_SUCCESS;
}

/***************************************************
SetID
	This function is called by the grid framework
	and assigns an ID number to be used by this
	cell type for the lifetime of 'this' instance
	of the grid. The ID number set by the grid
	for additional cell types depends on the order
	that the cells type were added.
Params:
	ID - ID number for the cell type to use
Returns:
	<none>
****************************************************/
void CUGCellType::SetID(int ID)
{
	m_ID = ID;
}

/***************************************************
GetID
	Returns the ID number of this cell type. This
	number will not change during the lifetime of 
	the grid's instance, but it may be different 
	the next time the grid is created.
Params:
	<none>
Returns:
	ID number of the cell
****************************************************/
int CUGCellType::GetID(){
	return m_ID;
}
/***************************************************
GetName
	Returns a readable name for the cell type.
	Returned value is used to help end-users
	to see what cell type are available.
Params:
	<none>
Return
	cell type name
****************************************************/
LPCTSTR CUGCellType::GetName()
{
	return _T("Normal Cell Type (default)");
}

/***************************************************
GetUGID
	Returns a GUID for the cell type, this number
	is unique for each cell type and never changes.
	This number can be used to find the cell types
	added to this instance of the Ultimate Grid.
Params:
	<none>
Returns:
	UGID (which is actually a GUID)	
****************************************************/
LPCUGID CUGCellType::GetUGID()
{
	static const UGID ugid = {0x78a705c1, 0xf3e9, 
							0x11d0, 0x9c, 0x7b, 
							0x0, 0x80, 0xc8, 0x3f, 
							0x71, 0x2f};

	return &ugid;
}

/***************************************************
GetEditArea
	Returns the editable area of a cell type.
	Some celltypes (i.e. drop list) require to
	have certain portion of the cell not covered
	by the edit control.  In case of the drop list
	the drop button should not be covered.
Params:
	rect - pointer to the cell rectangle, it can
		   be used to modify the edit area
Returns:
	UG_SUCCESS or UG_ERROR, currently the Utltimate
	Grid does not check the return value.
****************************************************/
int CUGCellType::GetEditArea(RECT *rect)
{
	UNREFERENCED_PARAMETER(*rect);
	return UG_SUCCESS;
}
/***************************************************
CanTextEdit
	The CanTextEdit function provides the grid
	with valuable information that it needs
	to determine if the user can start edit
	on given cell.

	This is often used on cell types that allow
	only visual user's interaction (progress bar,
	slider, etc)
Params:
	<none>
Return:
	TRUE - if the cell type allows editing
	FALSE - if the cell type does not allow editing
****************************************************/
BOOL CUGCellType::CanTextEdit()
{
	return m_canTextEdit;
}

/***************************************************
CanOverLap
	returns TRUE if this cell type can overlap
	other cells to the right if its text is too
	wide. Generally plain cell types allow overlapping
Params:
	<none>
Return:
	TRUE - if overlapping may occur
	FALSE - if overlapping is not allowed
****************************************************/
BOOL CUGCellType::CanOverLap(CUGCell *cell)
{
	if( cell->GetCellTypeEx() != 0 || cell->IsPropertySet(UGCELL_JOIN_SET) == TRUE )
		return 	FALSE;

	if( cell->IsPropertySet( UGCELL_ALIGNMENT_SET ) == TRUE )
	{
		long alignment = cell->GetAlignment();
		if( alignment & UG_ALIGNLEFT )
			return m_canOverLap;
		else
			return FALSE;
	}

	return FALSE;
}

/***************************************************
SetTextEditMode
	Is used to set the flag that identifies if
	the cell type can be editable.  This flag
	is then returned by the CanTextEdit function.
Params:
	mode - TRUE(on) or FALSE(off)
Return:
	UG_SUCCESS - success, this function will never fail.
****************************************************/
int CUGCellType::SetTextEditMode(int mode)
{
	if(mode == FALSE)
		m_canTextEdit = FALSE;
	else 
		m_canTextEdit = TRUE;

	return UG_SUCCESS;
}

/***************************************************
OnSystemChange
	This function is called for each cell type 
	when system settings change, such as screen
	resolution and or colors.
Params:
	none
Return:
	UG_SUCCESS - success
	UG_ERROR - error		
****************************************************/
int CUGCellType::OnSystemChange()
{
	return UG_SUCCESS;
}

/***************************************************
OnMessage
	This function is called when a windows message
	(UGCT_MESSAGE) was sent to this cell type.
	The wParam part of the message must contain the
	celltype ID (that is returned when a celltype
	is registered). 
Params:
	lParam - generic data, cell type dependant
			comes from the lParam paramenter of 
			a windows message
Return:
	Value is celltype dependant
****************************************************/
long CUGCellType::OnMessage(LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}

/***************************************************
OnCellTypeNotify
	This function is called whenever a celltype wants
	to fire a notification. By default this function
	calls through to CUGCtrl::OnCellTypeNotify, which
	developers generally override to handle the 
	notifications. However this function is useful
	to override when creating a customized cell type
	based off an existing cell type.
Params:
	ID - cell type ID
	col - column of the cell firing the notification
	row - row of the cell firing the notification
	msg - notification message
	param - notification dependant data
Return:
	Value is dependant on the notification being fired
****************************************************/
int CUGCellType::OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param){
	return m_ctrl->OnCellTypeNotify(ID,col,row,msg,param);
}

/***************************************************
OnLClicked
	This function is called when the left mouse 
	button is clicked over a cell using this cell
	type.
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
BOOL CUGCellType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	return FALSE;
}
/***************************************************
OnRClicked
	This function is called when the right mouse 
	button is clicked over a cell using this cell
	type.
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
BOOL CUGCellType::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	return FALSE;
}
/***************************************************
OnDClicked
	This function is called when the left mouse 
	button is double clicked over a cell using this cell
	type.
Params:
	col - column that was clicked in
	row - row that was clicked in
	rect - rectangle of the cell that was clicked in
	point - point where the mouse was clicked
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGCellType::OnDClicked(int col,long row,RECT *rect,POINT *point)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	return FALSE;
}
/***************************************************
OnKeyDown
	This function is called when a cell of this type
	has focus and a key is pressed.(See WM_KEYDOWN)
Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code, of the 
		key that was pressed
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGCellType::OnKeyDown(int col,long row,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*vcKey);
	return FALSE;
}
/***************************************************
OnKeyUp
	This function is called when a cell of this type
	has focus and a key was unpressed.(See WM_KEYUP)
Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code, of the 
		key that was unpressed
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGCellType::OnKeyUp(int col,long row,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*vcKey);
	return FALSE;
}
/***************************************************
OnCharDown
	This function is called when a cell of this type
	has focus and a printable key is pressed.(WM_CHARDOWN)
Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code, of the 
		key that was pressed
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGCellType::OnCharDown(int col,long row,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*vcKey);
	return FALSE;
}
/***************************************************
OnMouseMove
	This function is called when the mouse  is over
	a cell of this celltype.
Params:
	col - column that the mouse is over
	row - row that the mouse is over
	point - point where the mouse is
	flags - mouse move flags (see WM_MOUSEMOVE)
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
****************************************************/
BOOL CUGCellType::OnMouseMove(int col,long row,POINT *point,UINT flags)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(flags);
	return FALSE;
}

/***************************************************
OnChangedCellWidth
	This notification is sent to all visible
	cells is affected column when the user has
	changed width of a column.
Params:
	col - column that the mouse is over
	row - row that the mouse is over
	width - pointer to new column width
Return:
	<none>
****************************************************/
void CUGCellType::OnChangedCellWidth(int col, long row, int* width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*width);
}

/***************************************************
OnChangingCellWidth
	This notification is sent to all visible
	cells is affected column while the user is 
	changing width of a column.
Params:
	col - column that the mouse is over
	row - row that the mouse is over
	width - pointer to new column width
Return:
	<none>
****************************************************/
void CUGCellType::OnChangingCellWidth(int col, long row, int* width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*width);
}

/***************************************************
OnChangedCellHeight
	This notification is sent to all visible
	cells is affected row when the user has
	changed height of a column.
Params:
	col - column that the mouse is over
	row - row that the mouse is over
	height - pointer to new row height
Return:
	<none>
****************************************************/
void CUGCellType::OnChangedCellHeight(int col, long row, int* height)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/***************************************************
OnChangingCellHeight
	This notification is sent to all visible
	cells is affected row while the user is 
	changing height of a row.
Params
	col - column that the mouse is over
	row - row that the mouse is over
	height - pointer to new row height
Return
	<none>
****************************************************/
void CUGCellType::OnChangingCellHeight(int col, long row, int* height) 
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/***************************************************
SetOption
	This virtual function, although not implemented
	by default by any of cell types that are
	shipped with Ultimate Grid, was added
	to provide standard way to set additional
	properies of a cell.
Params:
	option - option ID number
	param - value for the specified option
Return:
	UG_NA - not implemented
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGCellType::SetOption(long option,long param)
{
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(param);
	return UG_NA;
}
/***************************************************
GetOption
	This virtual function, although not implemented
	by default by any of cell types that are
	shipped with Ultimate Grid, was added
	to provide standard way to get additional
	properies of a cell.
Params:
	option - option ID number
	param - value for the specified option
Return:
	UG_NA - not implemented
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGCellType::GetOption(long option,long* param)
{
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(*param);
	return UG_NA;
}
/***************************************************
OnSetFocus
	This function is called when a cell of this
	type receives focus.
Params
	col - column that just received focus
	row - row that just received focus
	cell - pointer to the cell object located at col/row
Return
	<none>
****************************************************/
void CUGCellType::OnSetFocus(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}
/***************************************************
OnKillFocus
	This function is called when a cell of this
	type loses focus.
Params
	col - column that just lost focus
	row - row that just lost focus
	cell - pointer to the cell object located at col/row
Return
	<none>
****************************************************/
void CUGCellType::OnKillFocus(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/***************************************************
OnDraw
	The Ultimate Grid calls this vistual function
	every time it is drawing a cell.  It is upto
	the individual cell type to properly draw itself.
Params:
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	col		- column that is being drawn
	row		- row that is being drawn
	cell	- cell that is being drawn
	selected- TRUE if the cell is selected, otherwise FALSE
	current - TRUE if the cell is the current cell, otherwise FALSE
Return
	<none>
****************************************************/
void CUGCellType::OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,
						int selected,int current)
{
	if (!m_drawThemesSet)
		m_useThemes = cell->UseThemes();

#ifdef UG_ENABLE_PRINTING
	// If it is printing, call OnDrawPrint to draw the cell 
	if(dc->IsPrinting()){
		OnDrawPrint( dc, rect, col, row, cell, selected, current);
		return;
	}
#endif

	if(m_ctrl->m_GI->m_enableCellOverLap){
		if(CanOverLap(cell)){
			if(cell->GetTextLength() == 0){
				int overlapCol,offset;
				CUGCell overlapCell;
				offset = GetCellOverlapInfo(dc,col,row,&overlapCol,&overlapCell);
				if(offset != 0){
					DrawBorder(dc,rect,rect,cell);
					DrawText(dc,rect,offset,col,row,&overlapCell,selected,current);
					m_ctrl->m_CUGGrid->m_drawHint.AddHint(col+1,row);
					return;
				}
			}
			else{
				m_ctrl->m_CUGGrid->m_drawHint.AddHint(col+1,row);
			}
		}
	}

	DrawBorder(dc,rect,rect,cell);
	DrawText(dc,rect,0,col,row,cell,selected,current);
}

/***************************************************
OnDrawPrint
	The OnDrawPrint virtual function is identical
	in function to the OnDraw function.  However,
	this version of the OnDraw is called when the
	cell is drawn to printer device.
Params:
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	col		- column that is being drawn
	row		- row that is being drawn
	cell	- cell that is being drawn
	selected- TRUE if the cell is selected, otherwise FALSE
	current - TRUE if the cell is the current cell, otherwise FALSE
Return
	<none>
****************************************************/
#ifdef UG_ENABLE_PRINTING
void CUGCellType::OnDrawPrint(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,
							int selected,int current)
{
	CSize   size;
	RECT    rectOut;
	CRect   rectTmp(rect);
	int     nHeadColCount = 0;	
	float   fScale = m_ctrl->GetUGPrint()->GetPrintVScale(dc);
	int     nColCount = (int)m_ctrl->GetUGPrint()->m_ColRects.GetSize();
	CUGCell cellRight, cellLeft;

	if( m_ctrl->GetUGPrint()->PrintSideHeading() )
		nHeadColCount = m_ctrl->m_GI->m_numberSideHdgCols;

	// If the column is not overlapped by left cell
	// draw the left cell using new rect.

	if(m_ctrl->m_GI->m_enableCellOverLap){
		if(CanOverLap(cell)){
			if(cell->GetTextLength() == 0){
				int     overlapCol;
				CUGCell overlapCell;
				if( GetCellOverlapInfo(dc,col,row,&overlapCol,&overlapCell) != 0 )
					return;
			}
		}
	}

	// If the column is not wider enought to draw the text 
	// and the right cell can be overlapped.

	if( cell->IsPropertySet(UGCELL_FONT_SET) )
		dc->SelectObject(cell->GetFont());
	size = dc->GetOutputTextExtent(cell->GetText());
	if (size.cx > 0)
		size.cx += (int) (m_ctrl->m_GI->m_margin * fScale);

	// Caculate the right position of the cell rect.

	if( ( size.cx > (int) (m_ctrl->GetColWidth(col) * fScale) ) &&
		m_ctrl->GetCellType(cell->GetCellType())->CanOverLap(cell) ){
		for( int i = col + 1; i + nHeadColCount < nColCount; i++ ){
			m_ctrl->GetCellIndirect( i, row, &cellRight );
			if( NULL != cellRight.GetText() ) break;
			rectTmp.right = m_ctrl->GetUGPrint()->
				m_ColRects[i + nHeadColCount].right;
			if( rectTmp.right - rectTmp.left > size.cx ) break;
		}
	}

	// Draw the border and the text

	DrawBorder(dc,rectTmp,&rectOut,cell);
	DrawText(dc,&rectOut,0,col,row,cell,selected,current);
}
#endif
/***************************************************
OnDrawFocusRect
	In the default implementation of the 
	CUGCtrl::OnDrawFocusRect the Ultimate Grid provides
	the cell type of the current cell with a chance
	to draw its own focus rect.  If a cell type draws
	the focus rectangle, than it should return TRUE
	to inform the control class.
Params:
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	col		- column that is being drawn
	row		- row that is being drawn
Return:
	TRUE	- if cell type processed this event
	FALSE	- if grid should 
****************************************************/
BOOL CUGCellType::OnDrawFocusRect(CDC *dc,RECT *rect,int col,int row)
{
	UNREFERENCED_PARAMETER(dc);
	UNREFERENCED_PARAMETER(rect);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return FALSE;
}

/***************************************************
DrawText
	This function is the standard text drawing routine
	used by this cell type and used by many others.
Params:
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	col		- column that is being drawn
	row		- row that is being drawn
	cell	- cell that is being drawn
	selected- TRUE if the cell is selected, otherwise FALSE
	current - TRUE if the cell is the current cell, otherwise FALSE
Return:
	<none>
****************************************************/
void CUGCellType::DrawText(CDC *dc,RECT *rect,int offset,int col,long row,CUGCell *cell,int selected,int current)
{
	int			left,top;
	SIZE		size;
	COLORREF	backcolor;
	LPCTSTR		string;
	int			stringLen;
	long		cellTypeEx;
	short		alignment;
	int			oldleft	= rect->left;
	long		properties = cell->GetPropertyFlags();
//	int			overLapCol = col;
	CFont		*pOldFont = NULL;

	// This is used by the theme drawing code.
	RECT offsetRect = *rect;
	offsetRect.left += offset;
	offsetRect.right -= offset;
	offsetRect.top += offset;
	offsetRect.bottom -= offset;

	UGXPThemeState state = UGXPThemes::GetState(selected>0, current>0);

	//get the extended style
	if(properties&UGCELL_CELLTYPEEX_SET){
		cellTypeEx = cell->GetCellTypeEx();
	}
	else
		cellTypeEx = 0;

	//get the string to draw
	if(!m_drawLabelText && !(cellTypeEx & UGCT_NORMALLABELTEXT) ){
		if(properties&UGCELL_STRING_SET){
			string  = cell->GetText();
			stringLen = cell->GetTextLength();
		}
		else{
			string = _T("");
			stringLen = 0;
		}
	}
	else{
		if(properties&UGCELL_LABEL_SET){
			string  = cell->GetLabelText();
			stringLen = lstrlen(string);
		}
		else{
			string = _T("");
			stringLen = 0;
		}
	}	

	//check for overlapping
/*	if(overLapCol != col)
	{
		cell->SetAlignment(olCell.GetAlignment());
		string  = olCell.GetText();
		stringLen = olCell.GetTextLength();
	}*/

	if(properties&UGCELL_ALIGNMENT_SET)
		alignment = cell->GetAlignment();
	else
		alignment = 0;

	
	//select the font
	if(properties&UGCELL_FONT_SET )
		pOldFont = dc->SelectObject(cell->GetFont());
	
	
	int style = 0;
	bool isDisabled = false;
	if( cell->IsPropertySet( UGCELL_CELLTYPEEX_SET ))
	{
		style = cell->GetCellTypeEx();
		isDisabled = (style & UGCT_CHECKBOXDISABLED) > 0;
	}

	if (isDisabled)
	{
		dc->SetTextColor(RGB(140, 140, 140));
		backcolor = cell->GetBackColor();
	}
	else
	//check the selected and current states
	if(selected || (current && m_ctrl->m_GI->m_currentCellMode&2))
	{
		dc->SetTextColor(cell->GetHTextColor());
		backcolor = cell->GetHBackColor();
	}
	else
	{
		dc->SetTextColor(cell->GetTextColor());
		backcolor = cell->GetBackColor();
	}

	// The button draws it's own background, so it can draw a pressed state
	if (cell->GetXPStyle() != XPCellTypeButton || !UGXPThemes::IsThemed())
		DrawBackground( dc, rect, backcolor, row, col, cell, (current != 0), (selected != 0) );
	
	//check for bitmaps
	if(properties&UGCELL_BITMAP_SET){
		oldleft = rect->left;
		if(offset != 0){
			int x;
			rect->left += offset;
			x= DrawBitmap(dc,cell->GetBitmap(),rect,backcolor);
			rect->left -= offset;
			offset += x;
			offsetRect.left += x;
		}
		else
		{
			int n = DrawBitmap(dc,cell->GetBitmap(),rect,backcolor);
			rect->left += n;
			offsetRect.left += n;
		}
	}

	GetTextExtentPoint(dc->m_hDC,string,stringLen,&size);

	// horizontal text alignment
	if(alignment&UG_ALIGNCENTER)
	{
		left = rect->left + (rect->right - rect->left - size.cx) /2;
	}
	else if(alignment&UG_ALIGNRIGHT)
	{
		left = rect->right - size.cx - m_ctrl->m_GI->m_margin;
	}
	else
	{
		left = rect->left + m_ctrl->m_GI->m_margin + offset;
	}	

	// vertical text alignment
	if(alignment & UG_ALIGNVCENTER)
	{
		GetTextExtentPoint(dc->m_hDC,string,stringLen,&size);
		top = rect->top + (rect->bottom - rect->top - size.cy) /2;
	}
	else if(alignment & UG_ALIGNBOTTOM)
	{
		top = rect->bottom - size.cy - 1;
	}
	else
	{
		top = rect->top + 1;
	}

	if(offset < 0)
		rect->left -= 1;

	// draw the text
	if(cellTypeEx&(UGCT_NORMALMULTILINE|UGCT_NORMALELLIPSIS))
	{
		if(cellTypeEx&UGCT_NORMALMULTILINE)
		{ // multiline

			CRect tempRect(rect);

			// set up a default format
			UINT format = DT_WORDBREAK | DT_NOPREFIX;

			// check alignment - multiline
			if(alignment) {
				if(alignment & UG_ALIGNCENTER) {
					format |= DT_CENTER;
				}
				else if(alignment & UG_ALIGNRIGHT) {
					format |= DT_RIGHT;
					tempRect.right -= m_ctrl->m_GI->m_margin + offset;
					offsetRect.right -= m_ctrl->m_GI->m_margin + offset;
				}
				else if(alignment & UG_ALIGNLEFT) {
					format |= DT_LEFT;
					tempRect.left += m_ctrl->m_GI->m_margin + offset;
					offsetRect.left += m_ctrl->m_GI->m_margin + offset;
				}
			}
			// if no alignment has been specified, then default to left justified
			else
			{
				format |= DT_LEFT;
				tempRect.left += m_ctrl->m_GI->m_margin + offset;
				offsetRect.left += m_ctrl->m_GI->m_margin + offset;
			}
			
			if (!m_useThemes || !DrawThemedText(*dc, tempRect.left, tempRect.top, &tempRect, string, stringLen, format, cell->GetXPStyle(), state))
			{
				dc->DrawText(string,-1,tempRect,format );			//draw the text			
			}
		}
		else
		{ // ellipsis

			rect->left += 3; //set margins
			rect->top = top;
 
			if (!m_useThemes || !DrawThemedText(*dc, left, top, &offsetRect, string, stringLen, DT_NOPREFIX | DT_END_ELLIPSIS | DT_SINGLELINE, cell->GetXPStyle(), state))
			{
				dc->DrawText(string, rect, DT_NOPREFIX | DT_END_ELLIPSIS | DT_SINGLELINE);
			}

			// NOTE: To get the ellipsis type to display maximum text len, even
			//		 if a letter must be cut to do so.  Use following code.
			// CSize size = dc->GetTextExtent(string,stringLen);
			// if(size.cx + 6 > (rect->right - rect->left)){
			// 	size = dc->GetTextExtent(_T("..."),3);
			// 	rect->right -= size.cx + 3;
			// 	if(rect->right > rect->left)
			// 		dc->ExtTextOut(rect->left + 3,top,ETO_OPAQUE|ETO_CLIPPED,rect,string,stringLen,NULL);
			// 	rect->left = rect->right;
			// 	if(rect->left <= oldleft)
			// 		rect->left = oldleft + 1;
			// 	rect->right += size.cx + 3;
			// 	dc->ExtTextOut(rect->left+1,top,ETO_OPAQUE|ETO_CLIPPED,rect,_T("..."),3,NULL);
			// }
			// else{
			// 	dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,rect,string,stringLen,NULL);
			// }
		}
	}
	else
	{
/*		if(overLapCol != col)
		{
			//get the offset
			for( int loop = col -1;loop >= overLapCol; loop --)
			{
				left -= m_ctrl->GetColWidth(loop);
			}
			left += 1;
		}
*/
//		DrawBackground( dc, rect, backcolor, row, col, cell, (current != 0), (selected != 0));

		int mode = DT_LEFT | DT_NOPREFIX;
		int orgTop = top;

		if(alignment & UG_ALIGNBOTTOM)
		{
			top = offsetRect.top;
			mode |= DT_SINGLELINE | DT_BOTTOM;
		}

		if (!m_useThemes || !DrawThemedText(*dc, left, top, &offsetRect, string, stringLen, mode, cell->GetXPStyle(), state))
		{
			dc->ExtTextOut(left,orgTop,ETO_CLIPPED,rect,string,stringLen,NULL);
		}
	}

	//reset the rect
	rect->left = oldleft;

	if ( pOldFont )
		dc->SelectObject( pOldFont );
}

bool CUGCellType::DrawThemedText(HDC dc, int left, int top,  RECT* rect, LPCTSTR string, int stringLen, DWORD textFormat, UGXPCellType cellType, UGXPThemeState state)
{
	bool success = false;

	if (m_useThemes && UGXPThemes::IsThemed())
	{
		RECT rcDraw = *rect;
		rcDraw.left = left;
		rcDraw.top = top;

		success = UGXPThemes::WriteText(NULL, dc, cellType, state, string, stringLen, textFormat, &rcDraw);
	}

	return success;
}

/***************************************************
DrawBackground
Params:
Return:
	<none>
****************************************************/
void CUGCellType::DrawBackground(CDC *dc,RECT *rect,COLORREF backcolor, int row, int col, CUGCell * cell, bool current, bool selected)
{
	bool isOwnerDraw = false;

	if (cell != NULL)
	{
		UGXPThemeState state = UGXPThemes::GetState(selected, current);

		if (m_useThemes)
		{
			RECT rcTheme = *rect;

			if (row != -1 && col != -1)
			{
				int overLapCol = 0;
				CUGCell olCell;
				int width = GetCellOverlapInfo(dc, col, row, &overLapCol, &olCell);

				// For some reason, GetCellOverlapInfo always returns 0 if the cell is on the left.
				if (overLapCol != col && width != 0)// && abs(overLapCol - col) == 1)
				{
					int step = 1;

					if (overLapCol < col) step = -1;

					for(int i = col + step; i != overLapCol + step; i += step)
					{
						RECT rcOverlap;

						m_ctrl->GetCellRect(i, row, &rcOverlap);

						int owidth = rcOverlap.right - rcOverlap.left;

						if (overLapCol > col)
						{
							rcTheme.right += owidth;
						}
						else
						{
							rcTheme.left -= owidth;
						}
					}
				}
			}

			isOwnerDraw = UGXPThemes::DrawBackground(NULL, *dc, cell->GetXPStyle(), state, &rcTheme, NULL);
	
			// If GetDrawBorderEdges returns true, we manually draw borders around the edge cells.
			if ((cell->GetXPStyle() == XPCellTypeTopCol ||
				cell->GetXPStyle() == XPCellTypeLeftCol ||
				cell->GetXPStyle() == XPCellTypeBorder) &&
				UGXPThemes::DrawBorderEdges())
			{
				isOwnerDraw &= (dc->DrawEdge(&rcTheme, EDGE_BUMP, BF_TOPRIGHT) > 0);
			}
			else
			{
				// DrawEdge fails for some cell types, but this is acceptable.
				UGXPThemes::DrawEdge(NULL, *dc, cell->GetXPStyle(), state, &rcTheme, cell->GetBorder(), 0, NULL);
			}
		}
	}

	if (!isOwnerDraw)
	{
		CBrush brush( backcolor );
		dc->FillRect( rect, &brush );
		dc->SetBkMode( TRANSPARENT );
	}
}

/***************************************************
OnPrint
	This function is called when the grid is going
	to be printed on a printer. This function should
	perform any setup required then call the celltypes
	OnDraw member
Params:
	dc		- device context to draw the cell with
	rect	- rectangle to draw the cell in
	bdrRect - outer retangle of the cells border
	col		- column that is being drawn
	row		- row that is being drawn
	cell	- cell that is being drawn
	overLapCol - column number of the cell to the left that
			is overlapping this cell. If this value is the same
			as 'col' then there is no overlapping
Return:
	<none>
****************************************************/
#ifdef UG_ENABLE_PRINTING
void CUGCellType::OnPrint(CDC *dc,RECT *rect,int col,long row,CUGCell *cell)
{
	int height, numRows, col1, col2;
	long row1, row2;
	CFont * font;
	
	col1 = col;
	row1 = row;

	if(row >=0)
		height = m_ctrl->GetRowHeight(row);
	else
		height = m_ctrl->m_GI->m_topHdgHeights[((row * -1) -1)];

	m_dScaleFactor = ((double)(rect->bottom - rect->top)) / ((double)height);
	
	if (cell->IsPropertySet (UGCELL_JOIN_SET)){
		m_ctrl->GetJoinRange (&col1, &row1, &col2, &row2);
		numRows = (row2 - row1) +1;
	
		if (numRows > 1)
			m_dScaleFactor = (((double)(rect->bottom - rect->top))/numRows)/((double)height);
	}
	else if ( col < 0 && row < 0 )
	{	// calculate the font scale factor when printing the corner button
		m_dScaleFactor = (((double)(rect->bottom - rect->top))/m_ctrl->m_GI->m_numberTopHdgRows)/((double)height);
	}

	if(cell->GetFont() != NULL)
		font = cell->GetFont();
	else
		font = m_ctrl->m_GI->m_defFont;
	LOGFONT lf;
	GetObject(font->m_hObject,sizeof(lf),&lf);

	CFont scaleFont;

	lf.lfHeight = (int)((double)lf.lfHeight * m_dScaleFactor);
	scaleFont.CreateFontIndirect(&lf);

	CFont * oldFont = dc->SelectObject(&scaleFont);
	cell->SetFont(&scaleFont);

	//TEST CODE
	CRect tempRect(rect);
	RECT tempRect2;

	OnDraw(dc,rect,col,row,cell,0,0);

	//TEST CODE
	DrawBorder(dc,tempRect,&tempRect2,cell);

	dc->SelectObject(oldFont);
}
#endif
/****************************************************
BitmapDisplay
	Draws a bitmap within the given rectangle. The bitmap
	is automatically resized to fit within rect. The bitmap
	is placed on the left side of the rect the with of the
	sized bitmap plus  a margin is returned
Params:
	hdc 		device context handle
	hbitmap 	bitmap handle
	rect 		bounding rectangle
	Margin 	top and bottom margins
Returns:
	returns the width of the area drawn in
*****************************************************/
int CUGCellType::DrawBitmap(CDC *dc,CBitmap * bitmap,RECT *rect,COLORREF backcolor)
{
	BITMAP bm;
	CBitmap * bmpOld;
	CDC dcMemory;
	long xin,yin,xout,yout;
	long ratio;
	int t;
	int resize = TRUE;
	int Margin = 2;

	//get the bitmaps co-ords
	bitmap->GetObject(sizeof(BITMAP), &bm);

	//calc the output co-ords , and maintain aspect ratio
	yout = (rect->bottom - rect->top - Margin*2)*100;
	yin =  bm.bmHeight;
	xin =  bm.bmWidth;
	ratio = yout /yin;
	xout = xin *ratio /100;
	yout /= 100;

	// printing the grid's content
#ifdef UG_ENABLE_PRINTING
	if( dc->IsPrinting())
	{	// adjust sizes of the bitmap and the margin to scale properly on the page
		Margin = (int)(Margin * m_dScaleFactor);
		yin = (int)(yin * m_dScaleFactor);
		xin = (int)(xin * m_dScaleFactor);
	}
#endif

	//if the height of the bitmap is less than the height of a row
	//then dont resize it
	if(yin <= (rect->bottom - rect->top - Margin *2) ){
		xout = xin;
		yout = yin;
		resize = FALSE;
	}
	if(xout >= rect->right - rect->left - Margin *2)
		xout = rect->right - rect->left - Margin *2;

	//create a compatible memory context
	dcMemory.CreateCompatibleDC(dc);
	bmpOld = (CBitmap *)dcMemory.SelectObject(bitmap);

	// If we're using themes, we draw the theme behind the bitmap - we can't do that here because we don't have a cell to pass in.
	if (!UGXPThemes::IsThemed())
	{
		//fill in background first
		t =rect->right;
		rect->right = rect->left+(int)xout+Margin*2;
		dc->SetBkColor(backcolor);
		DrawBackground( dc, rect, backcolor, NULL, false, false );
		rect->right = t;
	}

	//Draw the bitmap --- If the DC is printer, we have to 
	//convert the bitmap to DIB first, then print the DIB.
	if( dc->IsPrinting() )	// Print the bitmap	
	{
#ifdef UG_ENABLE_PRINTING
		LPSTR  pBuf;
		HANDLE hDib;
		LPBITMAPINFOHEADER lpbi;

		hDib = BitmapToDIB((HBITMAP) (bitmap->m_hObject), NULL);
	    lpbi = (LPBITMAPINFOHEADER) GlobalLock(hDib);

	    if( lpbi )
		{
			pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi);
			t= (int)((rect->bottom - rect->top - yout)/2);
			StretchDIBits (dc->m_hDC,
				rect->left + Margin, rect->top + t,
				(int)xout,(int)yout, 0, 0,
				bm.bmWidth, bm.bmHeight,
				pBuf, (LPBITMAPINFO)lpbi,
                DIB_RGB_COLORS, SRCCOPY);
		}
		GlobalUnlock (hDib);
		GlobalFree (hDib);
#endif
	}

	else
	{	// Display the bitmap on screen
		if(resize)
		{
			dc->StretchBlt(rect->left+Margin,rect->top+Margin,(int)xout,(int)yout,
				&dcMemory,0,0, bm.bmWidth, bm.bmHeight,SRCCOPY);
		}
		else
		{
			t= (int)((rect->bottom - rect->top - yout)/2);
			dc->BitBlt(rect->left+Margin,rect->top+t,(int)xout,(int)yout,
				&dcMemory,0,0,SRCCOPY);
		}
	}

	//remove the compatible memory context
	dcMemory.SelectObject(bmpOld);

	return (Margin*2+(int)xout);
}

/***************************************************
Draw Border
	Draws a border using the style set, possible
	styles are:
					   Left			   Top			   Right		   Bottom
				   |---------------|---------------|---------------|---------------
		Thin:		UG_BDR_LTHIN	UG_BDR_TTHIN	UG_BDR_RTHIN	UG_BDR_BTHIN
		Medium:		UG_BDR_LMEDIUM	UG_BDR_TMEDIUM	UG_BDR_RMEDIUM	UG_BDR_BMEDIUM
		Thick:		UG_BDR_LTHICK	UG_BDR_TTHICK	UG_BDR_RTHICK	UG_BDR_BTHICK
		3DRecess:	UG_BDR_RECESSED
		3DRaised:	UG_BDR_RAISED
Params:
	dc		- device context to draw on
	rect	- is the area to draw the border in
	rectout	- returns the area inside the border
	cell	- cell for which to draw the border for.
Returns:
	<none>
****************************************************/
void CUGCellType::DrawBorder(CDC *dc,RECT *rect,RECT *rectout,CUGCell * cell)
{
	if (!m_useThemes || !UGXPThemes::DrawEdge(NULL, *dc, cell->GetXPStyle(), ThemeStateNormal, rect, cell->GetBorder(), BF_RECT, rectout))
	{
		long props = cell->GetPropertyFlags();
		BOOL excelBdr = m_ctrl->m_GI->m_enableExcelBorders;

		if(( props & UGCELL_BORDERSTYLE_SET ) == 0 && !excelBdr)
		{
			CopyRect(rectout,rect);
			return;
		}

		int style = cell->GetBorder();
		CPen *pen = cell->GetBorderColor();
		CPen *origPen = NULL;

		if(style&UG_BDR_RAISED || style&UG_BDR_RECESSED)
			excelBdr = 0;

		int left=0,top=0,right=0,bottom=0;
		if(pen != NULL)
			dc->SelectObject(pen);
		else
			dc->SelectObject(m_ctrl->m_GI->m_defBorderPen);


		if(style&15 || excelBdr){ //thin lines
			
			if(style & 1){ //left
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->left,rect->bottom);
				left=1;
			}
			if(style & 2){ //top
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->right,rect->top);
				top=1;
			}
			if(style & 4 || excelBdr){ //right
				if((style&4) ==0)
					origPen = (CPen*)dc->SelectObject(m_ctrl->m_GI->m_defBorderPen);

				dc->MoveTo(rect->right-1,rect->top);
				dc->LineTo(rect->right-1,rect->bottom);
				right=-1;

				if((style&4) ==0)
					dc->SelectObject(origPen);
			}
			if(style & 8 || excelBdr){ //bottom
				if((style&8) ==0)
					origPen = (CPen*)dc->SelectObject(m_ctrl->m_GI->m_defBorderPen);

				dc->MoveTo(rect->left,rect->bottom-1);
				dc->LineTo(rect->right,rect->bottom-1);
				bottom=-1;

				if( origPen != NULL )
					// deselect the pen
					dc->SelectObject(origPen);
			}
		}
		if(style &240){ //medium lines
			if(style & 16){	//left
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->left,rect->bottom);
				dc->MoveTo(rect->left+1,rect->top);
				dc->LineTo(rect->left+1,rect->bottom);
				left=2;
			}
			if(style & 32){	//top
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->right,rect->top);
				dc->MoveTo(rect->left,rect->top+1);
				dc->LineTo(rect->right,rect->top+1);
				top=2;
			}
			if(style & 64){ //right
				dc->MoveTo(rect->right-1,rect->top);
				dc->LineTo(rect->right-1,rect->bottom);
				dc->MoveTo(rect->right-2,rect->top);
				dc->LineTo(rect->right-2,rect->bottom);
				right=-2;
			}
			if(style & 128){ //bottom
				dc->MoveTo(rect->left,rect->bottom-1);
				dc->LineTo(rect->right,rect->bottom-1);
				dc->MoveTo(rect->left,rect->bottom-2);
				dc->LineTo(rect->right,rect->bottom-2);
				bottom=-2;
			}
		}
		if(style &3840){ //thick lines
			if(style & 256){ //left
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->left,rect->bottom);
				dc->MoveTo(rect->left+1,rect->top);
				dc->LineTo(rect->left+1,rect->bottom);
				dc->MoveTo(rect->left+2,rect->top);
				dc->LineTo(rect->left+2,rect->bottom);
				left=3;
			}
			if(style & 512){ //top
				dc->MoveTo(rect->left,rect->top);
				dc->LineTo(rect->right,rect->top);
				dc->MoveTo(rect->left,rect->top+1);
				dc->LineTo(rect->right,rect->top+1);
				dc->MoveTo(rect->left,rect->top+2);
				dc->LineTo(rect->right,rect->top+2);
				top=3;
			}
			if(style & 1024){ //right
				dc->MoveTo(rect->right-1,rect->top);
				dc->LineTo(rect->right-1,rect->bottom);
				dc->MoveTo(rect->right-2,rect->top);
				dc->LineTo(rect->right-2,rect->bottom);
				dc->MoveTo(rect->right-3,rect->top);
				dc->LineTo(rect->right-3,rect->bottom);
				right=-3;
			}
			if(style & 2048){ //bottom
				dc->MoveTo(rect->left,rect->bottom-1);
				dc->LineTo(rect->right,rect->bottom-1);
				dc->MoveTo(rect->left,rect->bottom-2);
				dc->LineTo(rect->right,rect->bottom-2);
				dc->MoveTo(rect->left,rect->bottom-3);
				dc->LineTo(rect->right,rect->bottom-3);
				bottom=-3;
			}
		}
		if(style &4096){ //3D recessed

			int loop;
			//dark color
			dc->SelectObject((CPen *)&m_ctrl->m_threeDDarkPen);
			for(loop=0;loop<m_ctrl->m_GI->m_threeDHeight;loop++){
				dc->MoveTo(rect->left+loop,rect->bottom-loop-1);
				dc->LineTo(rect->left+loop,rect->top+loop);
				dc->LineTo(rect->right-loop-1,rect->top+loop);
			}
			//light color
			dc->SelectObject((CPen *)&m_ctrl->m_threeDLightPen);
			for(loop=0;loop<m_ctrl->m_GI->m_threeDHeight;loop++){
				dc->MoveTo(rect->right-loop-1,rect->top+loop);
				dc->LineTo(rect->right-loop-1,rect->bottom-loop-1);
				dc->LineTo(rect->left+loop,rect->bottom-loop-1);
			}
			left = m_ctrl->m_GI->m_threeDHeight;
			top = m_ctrl->m_GI->m_threeDHeight;
			right = -m_ctrl->m_GI->m_threeDHeight;
			bottom = -m_ctrl->m_GI->m_threeDHeight;

		}	

		if(style &8192){ //3D raised

			int loop;
			//light color
			dc->SelectObject((CPen *)&m_ctrl->m_threeDLightPen);
			for(loop=0;loop<m_ctrl->m_GI->m_threeDHeight;loop++){
				dc->MoveTo(rect->left+loop,rect->bottom-loop-1);
				dc->LineTo(rect->left+loop,rect->top+loop);
				dc->LineTo(rect->right-loop-1,rect->top+loop);
			}
			//dark color
			dc->SelectObject((CPen *)&m_ctrl->m_threeDDarkPen);
			for(loop=0;loop<m_ctrl->m_GI->m_threeDHeight;loop++){
				dc->MoveTo(rect->right-loop-1,rect->top+loop);
				dc->LineTo(rect->right-loop-1,rect->bottom-loop-1);
				dc->LineTo(rect->left+loop,rect->bottom-loop-1);
			}
			left = m_ctrl->m_GI->m_threeDHeight;
			top = m_ctrl->m_GI->m_threeDHeight;
			right = -m_ctrl->m_GI->m_threeDHeight;
			bottom = -m_ctrl->m_GI->m_threeDHeight;
		}	

		rectout->left	= rect->left + left;
		rectout->top	= rect->top + top;
		rectout->right	= rect->right + right;
		rectout->bottom = rect->bottom + bottom;
	}

	return;
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
void CUGCellType::GetBestSize(CDC *dc,CSize *size,CUGCell *cell)
{
	//select the font
	CFont * oldFont = NULL;
	if(cell->IsPropertySet(UGCELL_FONT_SET))
		oldFont = (CFont *)dc->SelectObject(cell->GetFont());
	else if(m_ctrl->m_GI->m_defFont != NULL)
		oldFont = (CFont *)dc->SelectObject(m_ctrl->m_GI->m_defFont);

	//get the best size
	CSize s(0,0);
	int bitmapWidth = 0;
	double yin = 0, ratio = 0;

	if ( cell->IsPropertySet( UGCELL_BITMAP_SET ))
	{
		BITMAP bitmap;
		cell->GetBitmap()->GetBitmap( &bitmap );
		// calculate the size of the bitmap
		yin = bitmap.bmHeight;
		if ( yin > 0 )
			ratio = size->cy / yin;
		else
			ratio = 0;

		bitmapWidth = (int)( bitmap.bmWidth * ratio );
	}

	if(cell->IsPropertySet(UGCELL_TEXT_SET))
	{
		CRect rect(0,0,0,0);
		UINT style = DT_CALCRECT;

		if ( cell->GetCellTypeEx() == 0 )
			style |= DT_SINGLELINE;
		if ( cell->GetCellTypeEx() & UGCT_NORMALSINGLELINE )
			style |= DT_SINGLELINE;
		if ( cell->GetCellTypeEx() & UGCT_NORMALELLIPSIS )
			style |= DT_END_ELLIPSIS;

		dc->DrawText( cell->GetText(), rect, style );
		s.cx = rect.Width();
		s.cy = rect.Height();
	}

	//use margins
	s.cx += 6 + bitmapWidth;
	s.cy += 2;

	//select back the original font
	if(oldFont != NULL)
		dc->SelectObject(oldFont);
	
	*size = s;
}

/****************************************************
GetCellOverlapInfo
	Calculates cell overlap size and offset for the
	given cell.  This function will only work if
	the cell overlap is enabled.
Params:
	dc			- device context to use to perform calculations
	col, row	- coordinates of a cell to check
	overlapCol	- column at which the overlap started
	cell		- pointer to a cell that is overlapping
Return:
	offset for drawing the overlapped cell
*****************************************************/
int CUGCellType::GetCellOverlapInfo(CDC* dc,int col,long row,int *overlapCol,CUGCell *cell)
{
	int			overLappedCol;			//used by cell-overlapping calculations
	int			loop,prevCol,width;
	CSize		size;

	//if cell overlapping is enabled check to see if the
	//cell is overlapped
	overLappedCol = col;
	if(m_ctrl->m_GI->m_enableCellOverLap){
		//check to see if there is a prev cell in the col that is not blank
		prevCol = col;
		if(m_ctrl->m_GI->m_defDataSource->GetPrevNonBlankCol(&prevCol,row) == UG_SUCCESS){
	
			//check to see if the text in the cell that was found
			//is wider than the column that it is in
			m_ctrl->GetCellIndirect(prevCol,row,cell);
			// call GetBestSize for cell type set to this cell
			size.cx = m_ctrl->GetColWidth( prevCol );
			size.cy = m_ctrl->GetRowHeight( row );
			(m_ctrl->GetCellType( cell->GetCellType()))->GetBestSize( dc, &size, cell );

			//if the prev cell is wider than its column width
			//and the cell allows overlapping

			if(size.cx > m_ctrl->GetColWidth(prevCol) &&
				m_ctrl->GetCellType(cell->GetCellType())->
				CanOverLap(cell) != FALSE){

				//check to see how many columns wide it is
				width = 0;
				for(loop = prevCol;loop <col;loop++){
					width += m_ctrl->GetColWidth(loop);
					if(width > size.cx){
						break;
					}
				}
				//check to see if the current cell is within the string width
				if(col == loop){
					*overlapCol = prevCol;
					//if we're printing then use the font scale factor otherwise make it 1.0
					if( !dc->IsPrinting() )
						m_dScaleFactor = 1.0;
					return (int)( - (double)(width) * m_dScaleFactor );
				}
			}					
		}
	}
	return 0;
}

/****************************************************
OnScrolled
	This event is called for all celltypes currently added
	to the grid when the view area is scrolled.
Params:
	col, row	- cell coordinates identifying current cell
	cell		- pointer to current cell
Return:
	<none>
*****************************************************/
void CUGCellType::OnScrolled(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/****************************************************************************
BitmapToDIB
	This function creates a DIB from a bitmap using the specified palette. 
Params:
	HBITMAP hBitmap  - specifies the bitmap to convert 
	HPALETTE hPal    - specifies the palette to use with the bitmap 
Return:
	HANDLE            - identifies the device-dependent bitmap 
*****************************************************************************/
HANDLE CUGCellType::BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal) 
{ 
    BITMAP              bm;         // bitmap structure 
    BITMAPINFOHEADER    bi;         // bitmap header 
    LPBITMAPINFOHEADER  lpbi;       // pointer to BITMAPINFOHEADER 
    DWORD               dwLen;      // size of memory block 
    HANDLE              hDIB,		// handle to DIB
						tempHandle;	// temp handle 
    HDC                 hDC;        // handle to DC 
    DWORD               biBits;     // bits per pixel 
 
    // check if bitmap handle is valid  
    if (!hBitmap) return NULL; 
 
    // fill in BITMAP structure, return NULL if it didn't work 
    if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm)) 
        return NULL; 
 
    // if no palette is specified, use default palette 
    if (hPal == NULL) 
        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE); 
 
    // calculate bits per pixel 
    biBits = bm.bmPlanes * bm.bmBitsPixel; 
 
    // make sure bits per pixel is valid  
    if (biBits <= 1) biBits = 1; 
    else if (biBits <= 4) biBits = 4; 
    else if (biBits <= 8) biBits = 8; 
    else // if greater than 8-bit, force to 24-bit 
        biBits = 24; 
 
    // initialize BITMAPINFOHEADER 
 
    bi.biSize = sizeof(BITMAPINFOHEADER); 
    bi.biWidth = bm.bmWidth; 
    bi.biHeight = bm.bmHeight; 
    bi.biPlanes = 1; 
    bi.biBitCount = (WORD)biBits; 
    bi.biCompression = BI_RGB; 
    bi.biSizeImage = 0; 
    bi.biXPelsPerMeter = 0; 
    bi.biYPelsPerMeter = 0; 
    bi.biClrUsed = 0; 
    bi.biClrImportant = 0; 
 
    // calculate size of memory block required to store BITMAPINFO 
    dwLen = bi.biSize + PaletteSize((LPSTR)&bi); 
 
    // get a DC 
    hDC = GetDC(NULL); 
 
    // select and realize our palette  
    hPal = SelectPalette(hDC, hPal, FALSE); 
    RealizePalette(hDC); 
 
    // alloc memory block to store our bitmap  
    hDIB = GlobalAlloc(GHND, dwLen); 
 
    // if we couldn't get memory block 
    if (!hDIB) 
    { 
      // clean up and return NULL  
      SelectPalette(hDC, hPal, TRUE); 
      RealizePalette(hDC); 
      ReleaseDC(NULL, hDC); 
      return NULL; 
    } 
 
    // lock memory and get pointer to it  
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    /// use our bitmap info. to fill BITMAPINFOHEADER  
    *lpbi = bi; 
 
    // call GetDIBits with a NULL lpBits param, so it will calculate the 
    // biSizeImage field for us     
    GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, (LPBITMAPINFO)lpbi, 
        DIB_RGB_COLORS); 
 
    // get the info. returned by GetDIBits and unlock memory block  
    bi = *lpbi; 
    GlobalUnlock(hDIB); 
 
    // if the driver did not fill in the biSizeImage field, make one up  
    if (bi.biSizeImage == 0) 
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight; 
 
    // realloc the buffer big enough to hold all the bits  
    dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage; 
 
	tempHandle = GlobalReAlloc(hDIB, dwLen, 0);
    if (tempHandle != NULL ) 
        hDIB = tempHandle; 
    else 
    { 
        // clean up and return NULL  
        GlobalFree(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    } 
 
    // lock memory block and get pointer to it 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    // call GetDIBits with a NON-NULL lpBits param, and actualy get the 
    // bits this time 
 
    if (GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, (LPSTR)lpbi + 
            (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi), (LPBITMAPINFO)lpbi, 
            DIB_RGB_COLORS) == 0) 
    { 
        // clean up and return NULL 
 
        GlobalUnlock(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    } 
 
    bi = *lpbi; 
 
    // clean up  
    GlobalUnlock(hDIB); 
    SelectPalette(hDC, hPal, TRUE); 
    RealizePalette(hDC); 
    ReleaseDC(NULL, hDC); 
 
    // return handle to the DIB 
    return hDIB; 
} 

/****************************************************************************
PaletteSize
	This function gets the size required to store the DIB's palette by 
	multiplying the number of colors by the size of an RGBQUAD (for a 
	Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an OS/2- 
	style DIB).
Params:
	LPSTR lpDIB      - pointer to packed-DIB memory block 
Return:
	WORD             - size of the color palette of the DIB 
*****************************************************************************/
DWORD CUGCellType::PaletteSize(LPSTR lpDIB) 
{ 
    // calculate the size required by the palette 
    if (IS_WIN30_DIB (lpDIB)) 
        return (DIBNumColors(lpDIB) * sizeof(RGBQUAD)); 
    else 
        return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
}

/****************************************************************************
DIBNumColors
	This function calculates the number of colors in the DIB's color table 
	by finding the bits per pixel for the DIB (whether Win3.0 or OS/2-style 
	DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256, 
	if 24, no colors in color table. 
Params:
	LPSTR lpDIB      - pointer to packed-DIB memory block 
Return:
	WORD             - number of colors in the color table 
*****************************************************************************/
WORD CUGCellType::DIBNumColors(LPSTR lpDIB) 
{ 
    WORD wBitCount;  // DIB bit count 
 
    // If this is a Windows-style DIB, the number of colors in the 
    // color table can be less than the number of bits per pixel 
    // allows for (i.e. lpbi->biClrUsed can be set to some value). 
    // If this is the case, return the appropriate value.  
 
    if (IS_WIN30_DIB(lpDIB)) 
    { 
        DWORD dwClrUsed; 
         dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed; 
        if (dwClrUsed) return (WORD)dwClrUsed; 
    } 
 
    // Calculate the number of colors in the color table based on 
    // the number of bits per pixel for the DIB. 
     
    if (IS_WIN30_DIB(lpDIB)) 
        wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
    else 
        wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
 
    // return number of colors based on bits per pixel 
 
    switch (wBitCount) 
    { 
        case 1: return 2; 
        case 4: return 16; 
        case 8: return 256; 
        default:return 0; 
    } 
} 
