/*************************************************************************
				Class Implementation : CUGCheckBoxType
**************************************************************************
	Source file : UGCBType.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/

#include "stdafx.h"
#include "UGCtrl.h"
//#include "UGCBType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
CUGCheckBoxType - Constructor
		Initialize variables, and create commonly
		used pens
***************************************************/
CUGCheckBoxType::CUGCheckBoxType()
{
	// make sure that user cannot start edit this cell type
	m_canTextEdit	= FALSE;
	// also, this cell type cannot overlap
	m_canOverLap	= FALSE;
	// create pen objects, later used for drawing
	m_lightPen.CreatePen(PS_SOLID,1,RGB(225,225,225));
	m_darkPen.CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	m_facePen.CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNFACE));

	m_hbrDither     = NULL;
	HBITMAP hbmGray = CreateDitherBitmap();
	if( hbmGray != NULL )
	{
		ASSERT( m_hbrDither == NULL );
		m_hbrDither = ::CreatePatternBrush( hbmGray );
		::DeleteObject( (HGDIOBJ)hbmGray );
	}
}

/***************************************************
~CUGCheckBoxType - Destructor
		Clean up all allocated resources
***************************************************/
CUGCheckBoxType::~CUGCheckBoxType()
{
	if( m_hbrDither != NULL )
	{
		::DeleteObject( (HGDIOBJ)m_hbrDither );
	}
}

/***************************************************
GetName  - overloaded CUGCellType::GetName
		Returns a readable name for the cell type.

    **See CUGCellType::GetName for more details
	about this function

Params:
	<none>
Return:
	LPCTSTR representation of the user friendly
			cell type name
***************************************************/
LPCTSTR CUGCheckBoxType::GetName()
{
	return _T("CheckBox");
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
***************************************************/
LPCUGID CUGCheckBoxType::GetUGID()
{
	static const UGID ugid = { 0x93aab8d0, 0xf749, 0x11d0, 
							{ 0x9c, 0x7f, 0x0, 0x80, 0xc8, 
							0x3f, 0x71, 0x2f } };

	return &ugid;
}

/***************************************************
OnDClicked  - overloaded CUGCellType::OnDClicked
	The check box does not have a special behaviour
	when it comes to the double click event.  It does
	however pass handling of this event to the
	OnLClicked virtual function.

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
***************************************************/
BOOL CUGCheckBoxType::OnDClicked(int col,long row,RECT *rect,POINT *point)
{
	return OnLClicked(col,row,1,rect,point);
}

/***************************************************
OnLClicked  - overloaded CUGCellType::OnLClicked
	The handling of the OnLClicked event in the check
	box cell type is the most important for this cell 
	type.  Here is where the desired user actions are
	evaluated and if necessary the check state is
	updated.
	This function determines if the user clicked the
	left mouse button over the check area on the cell
	if he did, than the state of the check is changed.

    **See CUGCellType::OnLClicked for more details
	about this function

Params:
	col - column that was clicked in
	row - row that was clicked in
	rect - rectangle of the cell that was clicked in
	point - point where the mouse was clicked
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
***************************************************/
BOOL CUGCheckBoxType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point)
{
	int top, checkSize;
	CRect tempRect(0,0,0,0);
	m_ctrl->GetCellIndirect( col, row, &m_cell );

	int style = 0;
	bool isDisabled = false;
	if( m_cell.IsPropertySet( UGCELL_CELLTYPEEX_SET ))
	{
		style = m_cell.GetCellTypeEx();
		isDisabled = (style & UGCT_CHECKBOXDISABLED) > 0;
	}

	// If the cell is read only, user should not be
	// allowed to change the state of the check.
	if ( m_cell.GetReadOnly() || isDisabled)
	{	
		return FALSE;
	}

	if( updn )
	{
		// calculate height of the cell, based on
		// given rect parameter.
		checkSize = rect->bottom - rect->top - (UGCT_CHECKSIZE / 2);
		// the check box will never be smaller than
		// UGCT_CHECKSIZE
		if( checkSize > UGCT_CHECKSIZE )
			checkSize = UGCT_CHECKSIZE;
		top = (rect->bottom - rect->top - checkSize) / 2;

		tempRect = *rect;
		AdjustRect ( &tempRect, m_cell.GetAlignment(), m_cell.GetCellTypeEx(), top, checkSize );

		// determine if the user clicked within the check box area
		if(!tempRect.PtInRect ( *point ))
			return FALSE;

		int style = 0;
		if(m_cell.IsPropertySet(UGCELL_CELLTYPEEX_SET))
			style = m_cell.GetCellTypeEx();

		int val;
		val = (int)m_cell.GetNumber();
		m_cell.SetNumber((val + 1) % ((style & UGCT_CHECKBOX3STATE) ? 3 : 2));
	
		// set the new check state to the data source
		m_ctrl->SetCell(col,row,&m_cell);
		// notify the CUGCtrl derived class that the check state was changed
		OnCellTypeNotify(m_ID,col,row,UGCT_CHECKBOXSET,(long)m_cell.GetNumber());
		// redraw the cell to reflect the changes.
		m_ctrl->RedrawCell(col,row);
		return TRUE;
	}

	return FALSE;
}

/***************************************************
OnCharDown  - overloaded CUGCellType::OnCharDown
	This function is called when a cell of this type
	has focus and a printable key is pressed.(WM_CHARDOWN)
	If the user has pressed SPACE bar than the check box
	cell type will interpret it as if the user wanted to
	change the state of the check box.

    **See CUGCellType::OnCharDown for more details
	about this function

Params:
	col - column that has focus
	row - row that has focus
	vcKey - pointer to the virtual key code,
			of the key that was pressed.
Return:
	TRUE - if the event was processed
	FALSE - if the event was not
***************************************************/
BOOL CUGCheckBoxType::OnCharDown(int col,long row,UINT *vcKey)
{
	m_ctrl->GetCellIndirect( col, row, &m_cell );

	if( m_cell.IsPropertySet( UGCELL_CELLTYPEEX_SET ))
	{
		int style = m_cell.GetCellTypeEx();
		if ((style & UGCT_CHECKBOXDISABLED) > 0)
		{
			return FALSE;
		}

	}

	if ( *vcKey == VK_SPACE && m_cell.GetReadOnly() == FALSE )
	{
		int style = 0;
		if(m_cell.IsPropertySet(UGCELL_CELLTYPEEX_SET))
			style = m_cell.GetCellTypeEx();

		int val;
		val = (int)m_cell.GetNumber();
		m_cell.SetNumber((val = (val + 1) % ((style & UGCT_CHECKBOX3STATE) ? 3 : 2)));

		m_ctrl->SetCell(col,row,&m_cell);
		m_ctrl->RedrawCell(col,row);

		//notify the user that the checkbox was checked
		OnCellTypeNotify(m_ID,col,row,UGCT_CHECKBOXSET,(long)val);
				
		return TRUE;
	}
	return FALSE;
}

/***************************************************
OnDraw  - overloaded CUGCellType::OnDraw
	This function will draw the checkbox according to
	the extended style for the checkbox.

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
	<none>
****************************************************/
void CUGCheckBoxType::OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current)
{
	double fHScale = 1.0;
	double fVScale = 1.0;
	
	if (dc->IsPrinting())
	{
		fHScale = m_ctrl->GetUGPrint()->GetPrintHScale(dc);
		fVScale = m_ctrl->GetUGPrint()->GetPrintVScale(dc);
	}

	if (!m_drawThemesSet)
		m_useThemes = cell->UseThemes();

	// draw border of the cell using build-in routine
	DrawBorder( dc, rect, rect, cell );

	int style = 0;
	bool isDisabled = false;
	if( cell->IsPropertySet( UGCELL_CELLTYPEEX_SET ))
	{
		style = cell->GetCellTypeEx();
		isDisabled = (style & UGCT_CHECKBOXDISABLED) > 0;
	}

	UGXPCellType ct = (style & UGCT_CHECKBOXROUND) ? XPCellTypeRadio : XPCellTypeCheck;

	if (cell->GetNumber() > 0)
	{
		if (style&UGCT_CHECKBOXCHECKMARK)
		{
			ct = (style & UGCT_CHECKBOXROUND) ? XPCellTypeRadioYes : XPCellTypeCheckYes;
		}
		else
		{
			ct = (style & UGCT_CHECKBOXROUND) ? XPCellTypeRadioNo : XPCellTypeCheckNo;
		}
	}

	UGXPThemeState ts = UGXPThemes::GetState(selected>0, current>0);

	if (isDisabled) ts = ThemeStateTriState;

	if (cell->GetNumber() > 1)
	{
		ts = ThemeStateTriState;
	}

	int right = rect->right,
		left = rect->left,
		top = 0,
		checkSize = 0;
	CRect checkrect(0,0,0,0);
	CPen * oldpen;

	checkSize = rect->bottom - rect->top - (int)( UGCT_CHECKMARGIN * 2 * fHScale );
	if( checkSize > UGCT_CHECKSIZE * fHScale )
		checkSize = (int)(UGCT_CHECKSIZE * fHScale);
	top = ( rect->bottom - rect->top - checkSize ) / 2;

	checkrect.CopyRect(rect);
	// calculate position and set of the check box
	AdjustRect( &checkrect, cell->GetAlignment(), cell->GetCellTypeEx(), top, checkSize );

	//*** draw the background ***
	if ( selected || ( current && m_ctrl->m_GI->m_currentCellMode & 2 ))
		DrawBackground( dc, rect, cell->GetHBackColor(), row, col, cell, (current != 0), (selected != 0));
	else
		DrawBackground( dc, rect, cell->GetBackColor(), row, col, cell, (current != 0), (selected != 0));

	if (!m_useThemes || !UGXPThemes::DrawBackground(NULL, *dc, ct, ts, &checkrect, NULL))
	{	
		//*** draw the checkbox ***
		if( checkSize >= ( UGCT_CHECKMARGIN * 2 ) )
		{
			//draw a 3D Recessed check box
			if( style & UGCT_CHECKBOX3DRECESS )
			{	
				oldpen = (CPen*)dc->SelectObject((CPen*)&m_darkPen);
				dc->MoveTo(checkrect.left,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.top);
				dc->LineTo(checkrect.right,checkrect.top);
				dc->SelectObject(&m_lightPen);
				dc->LineTo(checkrect.right,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.bottom);
				checkrect.top++;
				checkrect.left++;
				checkrect.right--;
				checkrect.bottom--;
				dc->SelectObject(&m_facePen);
				dc->MoveTo(checkrect.left,checkrect.bottom);
				dc->LineTo(checkrect.right,checkrect.bottom);
				dc->LineTo(checkrect.right,checkrect.top);
				dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN))));
				dc->LineTo(checkrect.left,checkrect.top);
				dc->LineTo(checkrect.left,checkrect.bottom);
				dc->SelectObject(oldpen);

				checkrect.top++;
				checkrect.left++;
				if( cell->GetNumber() > 1 )
					FillDitheredRect( dc, checkrect );
			}
			//draw a 3D Raised check box
			else if( style & UGCT_CHECKBOX3DRAISED )
			{	
				oldpen = (CPen*)dc->SelectObject((CPen*)&m_lightPen);
				dc->MoveTo(checkrect.left,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.top);
				dc->LineTo(checkrect.right,checkrect.top);
				dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN))));
				dc->LineTo(checkrect.right,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.bottom);
				checkrect.top++;
				checkrect.left++;
				checkrect.right--;
				checkrect.bottom--;
				dc->SelectObject(&m_darkPen);
				dc->MoveTo(checkrect.left,checkrect.bottom);
				dc->LineTo(checkrect.right,checkrect.bottom);
				dc->LineTo(checkrect.right,checkrect.top);
				dc->SelectObject(&m_facePen);
				dc->LineTo(checkrect.left,checkrect.top);
				dc->LineTo(checkrect.left,checkrect.bottom);
				dc->SelectObject(oldpen);

				checkrect.top++;
				checkrect.left++;
				if( cell->GetNumber() > 1 )
					FillDitheredRect( dc, checkrect );
			}
			// draw a flat (radio like) check box
			else if( style & UGCT_CHECKBOXROUND )
			{
				//draw the circle
				if( selected || ( current && m_ctrl->m_GI->m_currentCellMode&2 ))
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN))));
				else
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN))));

				dc->Arc( checkrect, CPoint(checkrect.left,(checkrect.bottom-checkrect.top)/2), CPoint(checkrect.left,(checkrect.bottom-checkrect.top)/2));
			}
			//draw a plain check box
			else
			{	
				if( selected || ( current && m_ctrl->m_GI->m_currentCellMode&2 ))
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN))));
				else
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN))));

				dc->MoveTo(checkrect.left,checkrect.top);
				dc->LineTo(checkrect.right,checkrect.top);
				dc->LineTo(checkrect.right,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.bottom);
				dc->LineTo(checkrect.left,checkrect.top);
				dc->SelectObject(oldpen);
					
				checkrect.left++;
				checkrect.top++;
				if( cell->GetNumber() > 1 )
					FillDitheredRect( dc, checkrect );
			}

			if ( cell->GetReadOnly() == TRUE )
			{	// cell is set to read only
				FillDitheredRect( dc, checkrect );
			}

			//draw the check
			if( cell->GetNumber() > 0 )
			{
				if( cell->GetNumber() > 1 )
					oldpen = (CPen*)dc->SelectObject((CPen*)&m_darkPen);
				else if( selected || ( current && m_ctrl->m_GI->m_currentCellMode&2 ))
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN))));
				else
					oldpen = (CPen*)dc->SelectObject((CPen*)CPen::FromHandle((HPEN)(CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN))));

				//draw a check mark
				if(style&UGCT_CHECKBOXCHECKMARK)
				{ 
					dc->MoveTo(checkrect.left+2,checkrect.bottom-4);
					dc->LineTo(checkrect.left+4,checkrect.bottom-2);
					dc->LineTo(checkrect.right+3,checkrect.top-1);
					if(checkSize > 9)
					{
						dc->MoveTo(checkrect.left+2,checkrect.bottom-5);
						dc->LineTo(checkrect.left+4,checkrect.bottom-3);
						dc->LineTo(checkrect.right+3,checkrect.top-2);
						dc->MoveTo(checkrect.left+5,checkrect.bottom-2);
						dc->LineTo(checkrect.right+4,checkrect.top-1);
						dc->MoveTo(checkrect.left+2,checkrect.bottom-6);
						dc->LineTo(checkrect.left+5,checkrect.bottom-3);
					}
					dc->SelectObject(oldpen);
				}
				// draw the radio style fill area
				else if( style & UGCT_CHECKBOXROUND )
				{
					checkrect.left += 2;
					checkrect.top += 2;
					checkrect.right -= 2;
					checkrect.bottom -= 2;

					CBrush brush;

					if( cell->GetNumber() > 1 )
					{
						LOGPEN logPen;
						m_darkPen.GetLogPen( &logPen );
						brush.CreateSolidBrush( logPen.lopnColor );
						dc->SelectObject( brush );
					}
					else if( selected || ( current && m_ctrl->m_GI->m_currentCellMode&2 ))
						dc->SelectObject((CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
					else
						dc->SelectObject((CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));

					dc->Ellipse( checkrect );
				}
				//draw the X mark
				else
				{
					checkrect.left++;
					checkrect.top++;
					checkrect.right-=2;
					checkrect.bottom-=2;
					dc->MoveTo(checkrect.left,checkrect.top);
					dc->LineTo(checkrect.right+1,checkrect.bottom+1);
					dc->MoveTo(checkrect.left,checkrect.bottom);
					dc->LineTo(checkrect.right+1,checkrect.top-1);
					if(checkSize > 9)
					{
						dc->MoveTo(checkrect.left+1,checkrect.top);
						dc->LineTo(checkrect.right+1,checkrect.bottom);
						dc->MoveTo(checkrect.left,checkrect.bottom-1);
						dc->LineTo(checkrect.right,checkrect.top-1);
						dc->MoveTo(checkrect.left,checkrect.top+1);
						dc->LineTo(checkrect.right,checkrect.bottom+1);
						dc->MoveTo(checkrect.left+1,checkrect.bottom);
						dc->LineTo(checkrect.right+1,checkrect.top);
					}
					dc->SelectObject(oldpen);
				}
			}
		}
		
	}

	if (!( style & UGCT_CHECKBOXUSEALIGN ))
	{
		// adjust text rect
		rect->left += (( UGCT_CHECKMARGIN * 2 ) + checkSize );
		rect->right = right;
		m_drawLabelText = TRUE;
		// draw the text using the default drawing routine
		CUGCellType::DrawText(dc,rect,0,col,row,cell,selected,current);
	}	

	// restore original value of the left side
	rect->left = left;
}

/***************************************************
AdjustRect
	This internal function is used to determine where
	the check box is loacated within the cell.

Params:
	rect		- rect of the cell that we are looking at, this
				  parameter is also used to return the rect
				  of the check box within the cell.
	alignment	- the alignment set to the cell
	top			- specifies y coordinate of the check box
	squareSize	- size if the check square
Return:
	<none>
***************************************************/
void CUGCheckBoxType::AdjustRect( RECT *rect, int alignment, int style, int top, int squareSize /*= UGCT_CHECKSIZE*/ )
{
	if ( style & UGCT_CHECKBOXUSEALIGN )
	{
		if( alignment & UG_ALIGNCENTER )
		{
			rect->left += (int)(( rect->right - rect->left ) / 2 - ( squareSize / 2 ));
		}
		else if( alignment & UG_ALIGNRIGHT )
		{
			rect->left = rect->right - squareSize - ( UGCT_CHECKMARGIN * 2 );
		}
		else// if( alignment & UG_ALIGNLEFT )
		{
			rect->left = rect->left + UGCT_CHECKMARGIN;
		}

		if( alignment & UG_ALIGNTOP )
		{
			rect->top = rect->top + UGCT_CHECKMARGIN;
		}
		else if( alignment & UG_ALIGNBOTTOM )
		{
			rect->top = rect->bottom - squareSize - ( UGCT_CHECKMARGIN * 2 );
		}
		else// if( alignment & UG_ALIGNVCENTER )
		{
			rect->top = rect->top + top;
		}

		// set remaining co-ordinates of the check box
		rect->right = rect->left + squareSize;
		rect->bottom= rect->top + squareSize;
	}
	else if( style & UGCT_CHECKBOXROUND )
	{
		rect->left	= rect->left + UGCT_CHECKMARGIN;
		rect->top	= rect->top + top;
		rect->bottom = rect->top + squareSize;
		rect->right = rect->left + squareSize + 1;
	}
	else
	{
		rect->left	= rect->left + UGCT_CHECKMARGIN;
		rect->top	= rect->top + top;
		rect->right	= rect->left + squareSize;
		rect->bottom= rect->top + squareSize;
	}
}

/***************************************************
CreateDitherBitmap
	Generates a dithered black and white bitmap.
Params:
	<none>
Return:
	HBITMAP representation of the dither bitmap
***************************************************/
HBITMAP CUGCheckBoxType::CreateDitherBitmap()
{
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD      bmiColors[16];
	}bmi;
	memset( &bmi, 0, sizeof( bmi ) );

	bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth = 8;
	bmi.bmiHeader.biHeight = 8;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	COLORREF clr = ::GetSysColor( COLOR_BTNFACE );
	bmi.bmiColors[0].rgbBlue = GetBValue( clr );
	bmi.bmiColors[0].rgbGreen = GetGValue( clr );
	bmi.bmiColors[0].rgbRed = GetRValue( clr );

	clr = ::GetSysColor( COLOR_BTNHIGHLIGHT );
	bmi.bmiColors[1].rgbBlue = GetBValue( clr );
	bmi.bmiColors[1].rgbGreen = GetGValue( clr );
	bmi.bmiColors[1].rgbRed = GetRValue( clr );

	// initialize the brushes
	long patGray[8];
	for( int i = 0 ; i < 8 ; i++ )
	{
	   patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;
	}

	HDC hDC = ::GetDC( NULL );
	HBITMAP hbm = ::CreateDIBitmap( hDC, &bmi.bmiHeader, CBM_INIT,
		(LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS );
	::ReleaseDC( NULL, hDC );

	return hbm;
}

/***************************************************
FillDitheredRect
	Is used to fill the back ground area of the check box
	portion of the cell.
Params:
	pDC		- current draw CDC
	rect	- rect to fill
Return:
	<none>
***************************************************/
void CUGCheckBoxType::FillDitheredRect( CDC* pDC, const CRect& rect )
{
	ASSERT_VALID(pDC);

	if( m_hbrDither == NULL )
	{
		::FillRect( pDC->GetSafeHdc (), &rect, (HBRUSH) (COLOR_WINDOW+1) );
	}
	else
	{
		::FillRect( pDC->GetSafeHdc (), &rect, m_hbrDither );
	}
}

/****************************************************
GetBestSize
	Returns the best (nominal) size for a cell using
	the checkbox celltype, with the given cell properties.
Params:
	dc		- device context to use to calc the size on	
	size	- return the best size in this param
	cell	- pointer to a cell object to use for the calc.
Return:
	<none>
*****************************************************/
void CUGCheckBoxType::GetBestSize(CDC *dc,CSize *size,CUGCell *cell)
{
	//select the font
	CFont * oldFont = NULL;
	//get the best size
	CSize tempSize(0,0);

	if (!( cell->GetCellTypeEx()&UGCT_CHECKBOXUSEALIGN ))
	{
		if(cell->IsPropertySet(UGCELL_FONT_SET))
			oldFont = (CFont *)dc->SelectObject(cell->GetFont());
		else if(m_ctrl->m_GI->m_defFont != NULL)
			oldFont = (CFont *)dc->SelectObject(m_ctrl->m_GI->m_defFont);

		if( cell->IsPropertySet(UGCELL_TEXT_SET))
		{
			CRect rect(0,0,0,0);
			UINT style = DT_CALCRECT;

			if ( cell->GetCellTypeEx() == 0 )
				style |= DT_SINGLELINE;
			if ( cell->GetCellTypeEx() & UGCT_NORMALSINGLELINE )
				style |= DT_SINGLELINE;
			if ( cell->GetCellTypeEx() & UGCT_NORMALELLIPSIS )
				style |= DT_END_ELLIPSIS;

			dc->DrawText( cell->GetLabelText(), rect, style );
			tempSize.cx = rect.Width() + ( UGCT_CHECKMARGIN * 2 );
			tempSize.cy = rect.Height();
		}
	}

	//use margins
	tempSize.cx += ( UGCT_CHECKMARGIN * 2 ) + UGCT_CHECKSIZE;
	tempSize.cy += 2;

	//select back the original font
	if(oldFont != NULL)
		dc->SelectObject(oldFont);
	
	*size = tempSize;
}
