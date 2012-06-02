/*************************************************************************
				Class Implementation : CUGArrowType
**************************************************************************
	Source file : UGCTarrw.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"
//#include "UGCTarrw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
CUGArrowType - Constructor
	Initialize all member variables
***************************************************/
CUGArrowType::CUGArrowType()
{	
	// make sure that user cannot start edit this cell type
	m_canTextEdit =	FALSE;
}

/***************************************************
~CUGArrowType - Denstructor
	Clean up all allocated resources
***************************************************/
CUGArrowType::~CUGArrowType()
{
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
LPCTSTR CUGArrowType::GetName()
{
	return _T("Side Arrows");
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
LPCUGID CUGArrowType::GetUGID()
{
	static const UGID ugid = { 0xf5c12593, 0xd930, 0x11d5, 
	{ 0x9b, 0x36, 0x0, 0x50, 0xba, 0xd4, 0x4b, 0xcd } };

	return &ugid;
}


/***************************************************
OnDraw - overloaded CUGCellType::OnDraw
	The arrow cell type uses this function to
	properly draw itself within cell's area
	respecting the extended cell type styles
	specified, using text color as the color
	which should be used to draw arrows.

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
void CUGArrowType::OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);

	DrawBorder(dc,rect,rect,cell);

	CPen pen;
	CPen * oldpen;

	int style = UGCT_ARROWRIGHT;
	if(cell->IsPropertySet(UGCELL_CELLTYPEEX_SET))
		style = cell->GetCellTypeEx();

	//*** draw the background ***

	//check the selected and current states
		COLORREF	backcolor;

	if(selected || (current && m_ctrl->m_GI->m_currentCellMode&2)){
		dc->SetTextColor(cell->GetHTextColor());
		backcolor = cell->GetHBackColor();
	}
	else{
		dc->SetTextColor(cell->GetTextColor());
		backcolor = cell->GetBackColor();
	}

	DrawBackground( dc, rect, backcolor, row, col, cell, (current > 0 ), (selected != 0));

	//check the width of the drawing area
	if((rect->right - rect->left) <4)
		return;

	//set up the pen color to draw with
	pen.CreatePen(PS_SOLID,1,cell->GetTextColor());
	oldpen = (CPen *)dc->SelectObject(&pen);

	//get the arrow center co-ords
	int vcenter,hcenter;
	vcenter = rect->top + (rect->bottom - rect->top)/2;
	hcenter = rect->left + (rect->right - rect->left)/2;

	if (dc->IsPrinting())
	{
#ifdef UG_ENABLE_PRINTING
		CRgn   rgn1, rgn2;
		POINT  point[3];
		CBrush Brush;
		int	   nWidth = rect->right - rect->left;
		int	   nHeight = rect->bottom - rect->top;
		float  fScale = m_ctrl->GetUGPrint()->GetPrintVScale(dc);

		Brush.CreateSolidBrush(RGB(0, 0 , 0));

		if( style&UGCT_ARROWRIGHT || style == 0 ){  // right arrow (default)
			point[0].x = rect->left + (int) ((nWidth - UG_TRIANGLE_WIDTH * fScale) / 2); 
			point[0].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2), 
			point[1].x = point[0].x;
			point[1].y = point[0].y + (int) (UG_TRIANGLE_HEIGHT * fScale);
			point[2].x = point[0].x + (int) (UG_TRIANGLE_WIDTH * fScale);
			point[2].y = rect->top + (int) (nHeight / 2), 

			rgn1.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn1, &Brush);
		}
		else if( style&UGCT_ARROWLEFT ){  //left arrow
			point[0].x = rect->left + (int) ((nWidth - UG_TRIANGLE_WIDTH * fScale) / 2); 
			point[0].y = rect->top + (int) (nHeight / 2), 
			point[1].x = point[0].x + (int) (UG_TRIANGLE_WIDTH * fScale);
			point[1].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2), 
			point[2].x = point[1].x;
			point[2].y = point[1].y + (int) (UG_TRIANGLE_HEIGHT * fScale);

			rgn1.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn1, &Brush);
		}
		else if( style&UGCT_ARROWDRIGHT ){  //double right arrow
			point[0].x = rect->left + nWidth / 2 - (int) ((UG_TRIANGLE_WIDTH + 1) * fScale);
			point[0].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2), 
			point[1].x = point[0].x;
			point[1].y = point[0].y + (int) (UG_TRIANGLE_HEIGHT * fScale); 
			point[2].x = rect->left + nWidth / 2 - (int) (fScale);
			point[2].y = rect->top + nHeight / 2;

			rgn1.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn1, &Brush);

			point[0].x = rect->left + nWidth / 2 + (int) (fScale);
			point[0].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2); 
			point[1].x = point[0].x;
			point[1].y = point[0].y + (int) (UG_TRIANGLE_HEIGHT * fScale); 
			point[2].x = rect->left + nWidth / 2 + (int) ((UG_TRIANGLE_WIDTH + 1) * fScale);
			point[2].y = rect->top + nHeight / 2;

			rgn2.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn2, &Brush);
		}
		else if( style&UGCT_ARROWDLEFT ){ // double left arrow
			point[0].x = rect->left + nWidth / 2 - (int) ((UG_TRIANGLE_WIDTH + 1) * fScale);
			point[0].y = rect->top + nHeight / 2; 
			point[1].x = point[0].x + (int) (UG_TRIANGLE_WIDTH * fScale);
			point[1].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2); 
			point[2].x = point[1].x;
			point[2].y = point[1].y + (int) (UG_TRIANGLE_HEIGHT * fScale); 

			rgn1.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn1, &Brush);

			point[0].x = rect->left + nWidth / 2 + (int) (fScale);
			point[0].y = rect->top + nHeight / 2; 
			point[1].x = point[0].x + (int) (UG_TRIANGLE_WIDTH * fScale);
			point[1].y = rect->top + (int) ((nHeight - UG_TRIANGLE_HEIGHT * fScale) / 2); 
			point[2].x = point[1].x;
			point[2].y = point[1].y + (int) (UG_TRIANGLE_HEIGHT * fScale); 

			rgn2.CreatePolygonRgn(point, 3, ALTERNATE);
			dc->FillRgn(&rgn2, &Brush);
		}
#endif
	}
	else
	{
		if(style&UGCT_ARROWRIGHT || style == 0){	  // right arrow (default)
			dc->MoveTo(hcenter-1,vcenter-3);
			dc->LineTo(hcenter-1,vcenter+4);
			dc->MoveTo(hcenter,vcenter-2);
			dc->LineTo(hcenter,vcenter+2);
			dc->LineTo(hcenter+2,vcenter);
			dc->LineTo(hcenter+1,vcenter-1);
			dc->LineTo(hcenter+1,vcenter+1);
		}
		else if(style&UGCT_ARROWLEFT){  //left arrow
			dc->MoveTo(hcenter+1,vcenter-3);
			dc->LineTo(hcenter+1,vcenter+4);
			dc->MoveTo(hcenter,vcenter-2);
			dc->LineTo(hcenter,vcenter+2);
			dc->LineTo(hcenter-2,vcenter);
			dc->LineTo(hcenter-1,vcenter-1);
			dc->LineTo(hcenter-1,vcenter+1);
		}
		else if(style&UGCT_ARROWDRIGHT){	  // double right arrow
			dc->MoveTo(hcenter+2,vcenter-3);
			dc->LineTo(hcenter+2,vcenter+4);
			dc->MoveTo(hcenter+3,vcenter-2);
			dc->LineTo(hcenter+3,vcenter+2);
			dc->LineTo(hcenter+5,vcenter);
			dc->LineTo(hcenter+4,vcenter-1);
			dc->LineTo(hcenter+4,vcenter+1);

			dc->MoveTo(hcenter-4,vcenter-3);
			dc->LineTo(hcenter-4,vcenter+4);
			dc->MoveTo(hcenter-3,vcenter-2);
			dc->LineTo(hcenter-3,vcenter+2);
			dc->LineTo(hcenter-1,vcenter);
			dc->LineTo(hcenter-2,vcenter-1);
			dc->LineTo(hcenter-2,vcenter+1);
		}
		else if(style&UGCT_ARROWDLEFT){  //double left arrow
			dc->MoveTo(hcenter+4,vcenter-3);
			dc->LineTo(hcenter+4,vcenter+4);
			dc->MoveTo(hcenter+3,vcenter-2);
			dc->LineTo(hcenter+3,vcenter+2);
			dc->LineTo(hcenter+1,vcenter);
			dc->LineTo(hcenter+2,vcenter-1);
			dc->LineTo(hcenter+2,vcenter+1);

			dc->MoveTo(hcenter-2,vcenter-3);
			dc->LineTo(hcenter-2,vcenter+4);
			dc->MoveTo(hcenter-3,vcenter-2);
			dc->LineTo(hcenter-3,vcenter+2);
			dc->LineTo(hcenter-5,vcenter);
			dc->LineTo(hcenter-4,vcenter-1);
			dc->LineTo(hcenter-4,vcenter+1);
		}
	}

	dc->SelectObject(oldpen);
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
void CUGArrowType::GetBestSize(CDC *dc,CSize *size,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(dc);

	size->cx = UG_TRIANGLE_WIDTH + 6;

	if ( cell->GetPropertyFlags()&UGCELL_CELLTYPEEX_SET )
	{
		if ( cell->GetCellTypeEx() == UGCT_ARROWDRIGHT || cell->GetCellTypeEx() == UGCT_ARROWDLEFT )
			 size->cx += UG_TRIANGLE_WIDTH;
	}
}