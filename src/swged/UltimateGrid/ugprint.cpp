/*************************************************************************
				Class Implementation : CUGPrint
**************************************************************************
	Source file : ugprint.cpp
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

#ifdef UG_ENABLE_PRINTING

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGPrint::CUGPrint()
{
	m_pagesWide = 0;
	m_pagesHigh = 0;
	m_printList = NULL;

	m_printLeftMargin	= 25;	//margin in mm
	m_printTopMargin	= 25;	
	m_printRightMargin	= 25;
	m_printBottomMargin = 25;

	m_printFrame = TRUE;
	m_printCenter = FALSE;
	m_fitToPage = FALSE;
	m_printTopHeading = FALSE;
	m_printSideHeading = FALSE;

	m_virPixWidth = 7;
	m_bIsPreview = FALSE;

	m_printScale = 100;
}

/***************************************************
~CUGPrint- Destructor
****************************************************/
CUGPrint::~CUGPrint(){
	ClearPrintList();
}

/***************************************************
ClearPrintList - Private
	Clears the list of pages that are to be printed
Params
	<none>
Return
	<none>
****************************************************/
void CUGPrint::ClearPrintList()
{
	CUGPrintList* next;
	
	//delete all the items in the linked list
	while(m_printList != NULL)
	{
		next = m_printList->next;
		delete m_printList;
		m_printList = next;
	}
}
/***************************************************
AddPageToList - Private
	Adds a page to the print list, including the
	col/row range of the grid cells to print.
Params
	page	- page number
	startCol- leftmost column to print on the page
	endCol	- rightmost column to print on the page
	startRow- topmost row to print on the page
	endRow	- bottom most row to print on the page
Return
	UG_SUCCESS - success
	UG_ERROR - failure
****************************************************/
int	CUGPrint::AddPageToList(int page,int startCol,int endCol,long startRow,long endRow)
{
	CUGPrintList * item = NULL;	//new print list item

	//if the list is already empty then just return
	if(m_printList == NULL)
	{
		m_printList = new CUGPrintList;
		item = m_printList;
		item->next = NULL;
	}
	else
	{
		//find the last item in the list
		CUGPrintList* current = m_printList;
		while(current != NULL)
		{
			if(current->page == page)
			{
				item = current;
				break;
			}
			if(current->next==NULL)
			{
				item = new CUGPrintList;
				current->next = item;
				item->next = NULL;
				break;
			}
			current = current->next;
		}
	}

	if ( item != NULL )
	{
		item->page		= page;
		item->startCol	= startCol;
		item->endCol	= endCol;
		item->startRow	= startRow;
		item->endRow	= endRow;

		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/***************************************************
GetPageFromList - Private
	Retrives the col/row range to be printed on a
	given page. The information is retrieved by
	page number
Params
	page	- page number to retrive the information for
	startCol- [out] leftmost column to print on the page
	endCol	- [out] rightmost column to print on the page
	startRow- [out] topmost row to print on the page
	endRow	- [out] bottom most row to print on the page
Return
	UG_SUCCESS - success
	UG_ERROR - failure
****************************************************/
int	CUGPrint::GetPageFromList(int page,int *startCol,int *endCol,long *startRow,long *endRow)
{
	//find the page
	CUGPrintList* next = m_printList;
	while(next != NULL)
	{
		if(next->page ==page)
			break;
		next = next->next;
	}

	//if the page was not found then return FALSE
	if(next == NULL)
		return UG_ERROR;
	
	//return the page information
	*startCol	= next->startCol;
	*endCol		= next->endCol;
	*startRow	= next->startRow;
	*endRow		= next->endRow;
	
	return UG_SUCCESS;
}
/***************************************************
PrintInit
	Initializes and sets up the data that needs to
	be calculated to perform the actual print. Such
	tasks include calcualating the number of pages
	required to print the given range, and the scale.
Params
	pDC - device context for the printer (or print preview)
	pPD - pointer to the print dialog information
	startCol- leftmost column to print on the page
	endCol	- rightmost column to print on the page
	startRow- topmost row to print on the page
	endRow	- bottom most row to print on the page
Return
	number of pages that will be printed
****************************************************/
int CUGPrint::PrintInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
						int endCol,long endRow)
{
	UNREFERENCED_PARAMETER(pPD);

	ClearPrintList();

	//get a screen device context
	CDC	sDC;
	sDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	
	//check to see if the request if for printing or print preview
	m_bIsPreview = FALSE;
	SIZE size;
	pDC->SetMapMode(MM_ANISOTROPIC);
	SetWindowExtEx(pDC->m_hDC,1,1,&size);
	if(size.cx != 1){
		m_bIsPreview = TRUE;
		SetWindowExtEx(pDC->m_hDC,size.cx,size.cy,&size);
	}
	
	//get the metrics for the printer
	float paperWidthPX		= (float)pDC->GetDeviceCaps(HORZRES);
	float paperHeightPX		= (float)pDC->GetDeviceCaps(VERTRES);
	float paperWidthMM		= (float)pDC->GetDeviceCaps(HORZSIZE);
	float paperHeightMM		= (float)pDC->GetDeviceCaps(VERTSIZE);
	int virtualWidthPX = (int)( 3.79 * (paperWidthMM - m_printLeftMargin - m_printRightMargin));
	int virtualHeightPX = (int)( 3.78 * (paperHeightMM - m_printTopMargin - m_printBottomMargin));

	//save the scaling values
	m_printVScale = ( (paperHeightPX/paperHeightMM) *
					(paperHeightMM - m_printTopMargin - m_printBottomMargin) ) /
					virtualHeightPX;
	m_printHScale = ( (paperWidthPX/paperWidthMM) *
					(paperWidthMM - m_printLeftMargin - m_printRightMargin) ) /
					virtualWidthPX;

	//save the margins
	m_printLeftMarginPX = (int)((paperWidthPX/paperWidthMM) * m_printLeftMargin);
	m_printTopMarginPX = (int)((paperHeightPX/paperHeightMM) * m_printTopMargin);
	m_printRightMarginPX = (int)(paperWidthPX - (paperWidthPX/paperWidthMM) * m_printRightMargin);
	m_printBottomMarginPX = (int)(paperHeightPX - (paperHeightPX/paperHeightMM) * m_printBottomMargin);
	m_paperWidthPX = (int)paperWidthPX;
	m_paperHeightPX = (int)paperHeightPX;

	//if not print preview, then addjust values by the virtual pixel width
	if(m_bIsPreview == FALSE)
	{
		m_virPixWidth = (int)paperWidthPX / 450;  //300
		m_printVScale /= m_virPixWidth;
		m_printHScale /= m_virPixWidth;
		m_printLeftMarginPX /= m_virPixWidth;
		m_printTopMarginPX /= m_virPixWidth;
		m_printRightMarginPX /= m_virPixWidth;
		m_printBottomMarginPX /= m_virPixWidth;
		m_paperWidthPX /= m_virPixWidth;
		m_paperHeightPX /= m_virPixWidth;
	}

	//get the number of pages wide
	int col,width=0,w;
	int scol = startCol;		//start col for a page
	m_pagesWide = 0;
	
	if(m_printSideHeading)
		width = m_ctrl->GetSH_Width();
	//find the number of pages wide and the number of columns
	//on each page
	for(col =startCol;col <= endCol;col++)
	{
		m_ctrl->GetColWidth(col,&w);		
		width += w;
	
		if(width > virtualWidthPX)
		{
			m_pagesWide++;
			if(m_printSideHeading)
				width = m_ctrl->GetSH_Width();
			else
				width = 0;

			if(w < virtualWidthPX)
				col--;

			//add the column info to the list
			AddPageToList(m_pagesWide,scol,col,0,0);
			scol = col+1;
		}
	}
	
	if(width >0)
	{
		m_pagesWide++;
		//add the column info to the list
		if(col > endCol)
			col = endCol;
		AddPageToList(m_pagesWide,scol,col,0,0);
	}

	//get the number of pages high
	long row;
	long srow = startRow;		//start row for page
	int height=0,h;
	m_pagesHigh = 0;
	
	if(m_printTopHeading)
		height = m_ctrl->GetTH_Height();

	//find the number of pages high and the number of rows
	//on each page
	for(row = startRow;row <=endRow;row++)
	{
		m_ctrl->GetRowHeight(row,&h);
		height += h;
		if(height > virtualHeightPX)
		{
			m_pagesHigh ++;
			if(m_printTopHeading)
				height = m_ctrl->GetTH_Height();
			else 
				height = 0;

			if(h < virtualHeightPX)
				row--;
			
			//add the page information to the list
			int c1,c2;
			long r1,r2;
			int page;
			for(col =1;col<=m_pagesWide;col++)
			{
				//get the col info
				GetPageFromList(col,&c1,&c2,&r1,&r2);
				//update the page info
				page = (m_pagesHigh-1)*m_pagesWide +col;
				AddPageToList(page,c1,c2,srow,row);
			}
			srow = row+1;
		}
	}
	if(height >0)
	{
		m_pagesHigh++;
		//add the page information to the list
		int c1,c2;
		long r1,r2;
		int page;
		for(col =1;col<=m_pagesWide;col++)
		{
			//get the col info
			GetPageFromList(col,&c1,&c2,&r1,&r2);
			//update the page info
			page = (m_pagesHigh-1)*m_pagesWide +col;
			if(row > endRow)
				row = endRow;
			AddPageToList(page,c1,c2,srow,row);
		}
	}

	//return the number of pages in total
	return (m_pagesWide * m_pagesHigh);
}
/***************************************************
PrintPage
	Performs the actual printing of the grid cells.
	PrintInit must be called first to setup the
	printing criteria.
Params
	pDC - device context of the printer (or print preview)
	pageNum - page number to print
Return
	UG_SUCCESS - success
	UG_ERROR - failure
****************************************************/
int CUGPrint::PrintPage(CDC * pDC, int pageNum)
{
	//TEST
	if(m_bIsPreview == FALSE)
	{
		SIZE size;
		pDC->SetMapMode(MM_ANISOTROPIC);
		SetWindowExtEx(pDC->m_hDC,1,1,&size);
		SetViewportExtEx(pDC->m_hDC,m_virPixWidth,m_virPixWidth,&size);
	}

	int  x,h,w,col,xx;
	long y,row,yy;
	int startCol,endCol;
	long startRow,endRow;
	RECT cellrect={0,0,0,0};
	RECT origCellRect;
	int leftMargin = m_printLeftMarginPX;
	int topMargin =  m_printTopMarginPX;

	//get the co-ords to print
	if(GetPageFromList(pageNum,&startCol,&endCol,&startRow,&endRow) != UG_SUCCESS)
		return UG_ERROR;

	//***********************************************************
	// Caculate the column width that will be used in drawing cells.
	//***********************************************************

	CRect ColRect;
	int   nColWidth, nFirstCol; 

	m_ColRects.RemoveAll();
	ColRect.left  = leftMargin;
	ColRect.right = ColRect.left;
	nFirstCol = startCol;
	if( m_printSideHeading )
		nFirstCol = 0 - m_GI->m_numberSideHdgCols;
	for( int i = nFirstCol; i <= endCol; i++ )
	{
		if( i < 0 )
		{
			//get the side heading co-ords
			ColRect.left = ColRect.right;
			ColRect.right += (int) (m_ctrl->m_CUGSideHdg->GetSHColWidth(i) * m_printHScale);
		}
		else 
		{
			//get the left,right co-ords
			ColRect.left = ColRect.right;
			m_ctrl->GetColWidth( i, &nColWidth );
			ColRect.right += (int) (nColWidth  * m_printHScale);
		}
		m_ColRects.Add(ColRect);
	}

	//*********************
	// draw the page
	//*********************

	//draw the grid
	cellrect.top = topMargin;
	cellrect.bottom = topMargin;

	//main grid row drawing loop
	yy = startRow;
	if(m_printTopHeading)
		yy = 0 - m_GI->m_numberTopHdgRows;

	for(y = yy; y <= endRow ; y++)
	{	
		//set up the rectangle for the row
		if(y < 0 )
		{
			//set up the rect for the top heading
			cellrect.top	= cellrect.bottom;
			cellrect.bottom+= (int)(m_ctrl->m_CUGTopHdg->GetTHRowHeight(y) * m_printVScale);
			cellrect.left	= leftMargin;
			cellrect.right  = cellrect.left;
		}
		else
		{
			if(yy < 0)
			{
				yy=0;
				y = startRow;
			}
			//set up the rect for the row to be drawn
			cellrect.top	= cellrect.bottom;
			m_ctrl->GetRowHeight(y,&h);		
			cellrect.bottom += (int)(h * m_printVScale);
			cellrect.left	= leftMargin ;
			cellrect.right  = cellrect.left;
		}	

		//dont draw the row if it is 0 pixels high		
		if(cellrect.top == cellrect.bottom)
			continue;

		//main column drawing loop
		xx =startCol;
		if(m_printSideHeading)
			xx = 0 - m_GI->m_numberSideHdgCols;
		for(x = xx; x <= endCol; x++)
		{
			if( x < 0 )
			{
				//get the side heading co-ords
				cellrect.left = cellrect.right;
				cellrect.right += (int)(m_ctrl->m_CUGSideHdg->GetSHColWidth(x) * m_printHScale);
			}
			else
			{
				if(xx < 0)
				{
					xx=0;
					x = startCol;
				}
				//get the left,right co-ords
				cellrect.left = cellrect.right;
				m_ctrl->GetColWidth(x,&w);		
				cellrect.right += (int)(w * m_printHScale);
			}
		
			//dont draw the column if it is 0 pixels wide
			if(cellrect.left == cellrect.right)
				continue;

			//store the original cell rect, incase it is modified below
			CopyRect(&origCellRect,&cellrect);
	
			col = x;
			row = y;

			//check to see if the cell is a joined cell
			if(m_ctrl->GetJoinStartCell(&col,&row)==UG_SUCCESS)
			{
				RECT tempRect;
				CUGCell sourceCell, targetCell;
				int  col2, tempCol, tempCol2;
				long row2, tempRow, tempRow2;

				tempCol  = col;
				tempRow  = row;
				m_ctrl->GetJoinRange(&col,&row,&col2,&row2);
				tempCol2 = col2;
				tempRow2 = row2;

				if ((y == startRow && x == col) || (x == startCol && y == row)
					|| (y == startRow && x == startCol))
				{
					m_ctrl->GetCell (col,row,&sourceCell);
					m_ctrl->SetCell (x,y,&sourceCell);
					m_ctrl->JoinCells (x,y,col2,row2);
					col = x;
					row = y;
					m_ctrl->GetJoinRange(&col,&row,&col2,&row2);
				}
				
				if(col == x && row == y)
				{
					CopyRect(&tempRect,&cellrect);

					if (col2 > endCol || row2 > endRow)
					{
						if (col2 > endCol)
							col2 = endCol;
						if (row2 > endRow)
							row2 = endRow;
						m_ctrl->JoinCells (col,row,col2,row2);
					}
					while(col2 > col)
					{
						if (col2 <= endCol)
						{
							m_ctrl->GetColWidth(col2,&w);		
							tempRect.right+=(int)(w* m_printHScale);
						}
						col2--;
					}

					while(row2 > row)
					{
						if (row2 <= endRow)
						{
							m_ctrl->GetRowHeight(row2,&h);		
							tempRect.bottom+=(int)(h* m_printVScale);
						}
						row2--;
					}
					InternPrintCell(pDC,&tempRect,col,row);
					m_ctrl->JoinCells (tempCol,tempRow,tempCol2,tempRow2);
				}
			}
			else
			{
				InternPrintCell(pDC,&cellrect,x,y);
			}
			
			//draw a section of the print frame if selected
			CopyRect(&cellrect,&origCellRect);
		}
	}

	if ( m_printFrame )
	{	// draw left border and top border.
		if ((( m_printTopHeading ) && ( m_GI->m_numberTopHdgRows > 0 )) ||
			( startRow <= endRow))
		{
			pDC->MoveTo(leftMargin,topMargin);
			pDC->LineTo(leftMargin,cellrect.bottom-1);
			pDC->MoveTo(leftMargin,topMargin);
			pDC->LineTo(cellrect.right-1, topMargin);
		}
	}

	return UG_SUCCESS;
}
/***************************************************
PrintSetMargin
	Sets the margin in mm (milli-meters)
Params
	whichMargin	- one of the following
					UG_PRINT_LEFTMARGIN
					UG_PRINT_RIGHTMARGIN
					UG_PRINT_TOPMARGIN
					UG_PRINT_BOTTOMMARGIN
	size - size of the margin in mm
return
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGPrint::PrintSetMargin(int whichMargin,int size)
{
	return PrintSetOption(whichMargin,(long)size);
}
/***************************************************
PrintSetScale
	Sets the overall printing scale, as a percentage. 
	The value defaults to 100% and can range from
	1 to 5000 percent
Params
	scalePercent - percentage to set the scale to
Return
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGPrint::PrintSetScale(int scalePercent)
{
	if(scalePercent < 1 || scalePercent > 5000)
		return UG_ERROR;

	m_printScale = scalePercent;

	return UG_SUCCESS;
}
/***************************************************
PrintSetOption
	Sets the up the printing options.
	
	 Note:When setting margins the param value is 
	 in mm (milli-meters)
Params
	option	- one of the following
				UG_PRINT_LEFTMARGIN
				UG_PRINT_RIGHTMARGIN
				UG_PRINT_TOPMARGIN
				UG_PRINT_BOTTOMMARGIN
				UG_PRINT_FRAME
				UG_PRINT_TOPHEADING
				UG_PRINT_SIDEHEADING
	param - if setting a margin:  size in mm
			if setting the frame: TRUE or FALSE
			if setting a heading: TRUE or FALSE
return
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGPrint::PrintSetOption(int option,long param)
{
	if(option == UG_PRINT_LEFTMARGIN)
		m_printLeftMargin = (int)param;
	if(option == UG_PRINT_RIGHTMARGIN)
		m_printRightMargin =  (int)param;
	if(option == UG_PRINT_TOPMARGIN)
		m_printTopMargin =  (int)param;
	if(option == UG_PRINT_BOTTOMMARGIN)
		m_printBottomMargin =  (int)param;
	if(option == UG_PRINT_FRAME)
	{
		if(param == FALSE)
			m_printFrame =  FALSE;
		else
			m_printFrame =  TRUE;
	}
	if(option == UG_PRINT_TOPHEADING)
	{
		m_printTopHeading = param;
	}
	if(option == UG_PRINT_SIDEHEADING)
	{
		m_printSideHeading = param;
	}
	
	return UG_SUCCESS;
}

/***************************************************
PrintGetOption
	Retrieves the margin in mm (milli-meters)
Params
	option	- one of the following
				UG_PRINT_LEFTMARGIN
				UG_PRINT_RIGHTMARGIN
				UG_PRINT_TOPMARGIN
				UG_PRINT_BOTTOMMARGIN
				UG_PRINT_FRAME
				UG_PRINT_TOPHEADING
				UG_PRINT_SIDEHEADING
	param - [out] if retrieving a margin:  size in mm
			[out] if retrieving the frame: TRUE or FALSE
			[out] if retrieving a heading: TRUE or FALSE
return
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGPrint::PrintGetOption(int option,long *param)
{
	int retval = UG_SUCCESS;

	switch (option) 
	{
	case UG_PRINT_LEFTMARGIN:
		*param = m_printLeftMargin;
		break;
	case UG_PRINT_RIGHTMARGIN:
		*param = m_printRightMargin;
		break;
	case UG_PRINT_TOPMARGIN:
		*param = m_printTopMargin;
		break;
	case UG_PRINT_BOTTOMMARGIN:
		*param = m_printBottomMargin;
		break;
	case UG_PRINT_FRAME:
		*param = m_printFrame;
		break;
	case UG_PRINT_TOPHEADING:
		*param =  m_printTopHeading;
		break;
	case UG_PRINT_SIDEHEADING:
		*param =  m_printSideHeading;
		break;
	default:
		*param = 0;
		retval = UG_ERROR;
	}
	
	return retval;
}

/***************************************************
InternPrintCell - private
	This function is called by the PrintPage function
	to setup the drawing for an individual cell.
	Once setup then the cells specified celltype is
	called to perform the actual drawing
Params
	dc - device context to draw to
	rect - rectangle to draw the cell within
	col - column of the cell to draw
	row - row of the cell to draw
****************************************************/
void CUGPrint::InternPrintCell(CDC *dc,RECT * rect,int col,long row)
{
	CUGCellType * cellType;
	CUGCell cell,cell2;
	int leftMargin = m_printLeftMarginPX;
	int topMargin =  m_printTopMarginPX;

	//TEST
	CRgn rgn;
	if(m_bIsPreview == FALSE){
		rgn.CreateRectRgn((rect->left-0) * m_virPixWidth,
			(rect->top-0) * m_virPixWidth,
			(rect->right-0) * m_virPixWidth,
			(rect->bottom-0) * m_virPixWidth);

		dc->SelectClipRgn(NULL);
		dc->SelectClipRgn(&rgn);
	}

	if ( col < 0 && row < 0 )
	{
		rect->top = topMargin;
		rect->left = leftMargin;
		rect->right = (int)(leftMargin + (m_GI->m_sideHdgWidth * m_printHScale));
		rect->bottom = (int)(topMargin + (m_GI->m_topHdgHeight * m_printVScale)) - 1;

		col = -1;
		row = -1;
	}

	//get the cell information
	m_ctrl->GetCellIndirect(col,row,&cell);

	//get the cell type to draw the cell
	if(cell.IsPropertySet(UGCELL_CELLTYPE_SET))
		cellType = m_ctrl->GetCellType(cell.GetCellType());
	else
		cellType = m_ctrl->GetCellType(-1);

	//paint the cell
	int saveID = dc->SaveDC();
	cellType->OnPrint(dc,rect,col,row,&cell);
	dc->RestoreDC(saveID);

	//TEST
	if(m_bIsPreview == FALSE){
		dc->SelectClipRgn(NULL);
	}
}

#endif


