/*************************************************************************
				Class Implementation : CUGMultiSelect
**************************************************************************
	Source file : UGMultiS.cpp
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
	Standard construction/desrtuction
***************************************************/
CUGMultiSelect::CUGMultiSelect()
{
	//set up the variables
	m_list				= NULL;
	m_blockInProgress	= FALSE;
	m_mode				= UG_MULTISELECT_OFF;
	m_enumInProgress	= FALSE;
	m_numberBlocks		= 0;
	m_blockJustStarted	= FALSE;
	m_lastCol			= -1;
	m_lastRow			= -1;
}

CUGMultiSelect::~CUGMultiSelect()
{	
	m_numberBlocks = 0;
	// remove all selection entries
	ClearAll();
}

/***************************************************
ClearAll
	function is used to empty out the selected cells
	list.
Params:
	<none>
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGMultiSelect::ClearAll()
{
	m_blockInProgress = FALSE;	
	m_enumInProgress	= FALSE;
	m_numberBlocks		= 0;
	m_lastCol			= -1;
	m_lastRow			= -1;

	//delete all items in the list
	UG_MSList *next;
	while( m_list != NULL )
	{
		next = m_list->next;
		delete m_list;
		m_list = next;
	}

	return UG_SUCCESS;
}

/***************************************************
StartBlock
	function is called whenever the user decided to
	select new range of cells either by mouse or
	by calling CUGCtrl::Select.
Params:
	col, row	- coordinates of the cell that was
				  selected.
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGMultiSelect::StartBlock(int col,long row)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_SUCCESS;

	//if the first select block
	m_origCol = col;
	m_origRow = row;
	
	//update the start variables
	m_startCol = col;
	m_startRow = row;
	m_blockInProgress = TRUE;

	//create a new item
	m_currentItem = new UG_MSList;
	m_currentItem->startCol =	col;
	m_currentItem->endCol   =	col;
	m_currentItem->loCol	=	col;
	m_currentItem->hiCol	=	col;
	m_currentItem->startRow =	row;
	m_currentItem->endRow   =	row;
	m_currentItem->loRow	=	row;
	m_currentItem->hiRow	=	row;

	m_currentItem->selected =	TRUE;
	m_currentItem->next		=	NULL;

	if(m_mode&UG_MULTISELECT_ROW)
	{
		// in the case when the row selection is enabled
		// the beggining column should be set to zero and
		// ending column to number of columns available
		m_currentItem->loCol	=	0;
		m_currentItem->hiCol	=	m_ctrl->GetNumberCols();
	}

	// add the item to the list
	if(m_list != NULL)
	{
		UG_MSList * next = m_list;
		// find the last item in the list
		while(next->next != NULL)
			next = next->next;
		// then append the new one
		next->next = m_currentItem;
	}
	else
	{	// make this item first
		m_list = m_currentItem;
	}

	m_blockJustStarted = TRUE;
	// increase number of blocks selected
	m_numberBlocks++;
	// notify the user that the selection was changed
	m_ctrl->OnSelectionChanged( m_currentItem->hiCol, m_currentItem->loRow, 
								m_currentItem->loCol, m_currentItem->hiRow,
								m_numberBlocks );

	return UG_SUCCESS;
}

/***************************************************
GetOrigCell
	function is called to retrieve information about the currect
	cell.  It is called by the CUGCtrl class to get coordinates
	of the current cell, as it is shown to the user.
Params:
	col		- pointer to integer that indicates original column.
	row		- pointer to integer that indicates original row.
Returns:
	UG_ERROR	- if there are no selections
*****************************************************/
int CUGMultiSelect::GetOrigCell(int *col,long *row)
{
	if(m_blockInProgress == FALSE)
		return UG_ERROR;

	*col = m_origCol;
	*row = m_origRow;

	return UG_SUCCESS;
}

/***************************************************
EndBlock
	is called to complete the selection block, usually
	called while the user is selecting range of cells
	using mouse dragging ability.
Params:
	col, row	- coordinates of where the selected
				  block ends.
Returns:
	UG_ERROR	- if the selection was not started
	UG_SUCCESS	- on success
*****************************************************/
int CUGMultiSelect::EndBlock(int col,long row)
{	
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_SUCCESS;

	if(	m_blockInProgress == FALSE)
		return UG_ERROR;

	int maxLoCol   = m_currentItem->loCol;
	int maxHiCol   = m_currentItem->hiCol;
	long maxLoRow  = m_currentItem->loRow;
	long maxHiRow  = m_currentItem->hiRow;
	
	int oldEndCol = m_currentItem->endCol;
	int oldEndRow = m_currentItem->endRow;

	m_currentItem->endCol   =	col;
	m_currentItem->endRow   =	row;

	if(m_currentItem->endCol > m_currentItem->startCol){
		m_currentItem->loCol = m_currentItem->startCol;
		m_currentItem->hiCol = m_currentItem->endCol;
	}
	else{
		m_currentItem->hiCol = m_currentItem->startCol;
		m_currentItem->loCol = m_currentItem->endCol;
	}
	if(m_currentItem->endRow > m_currentItem->startRow){
		m_currentItem->loRow = m_currentItem->startRow;
		m_currentItem->hiRow = m_currentItem->endRow;
	}
	else{
		m_currentItem->hiRow = m_currentItem->startRow;
		m_currentItem->loRow = m_currentItem->endRow;
	}

	if(m_mode&UG_MULTISELECT_ROW){
		m_currentItem->loCol	=	0;
		m_currentItem->hiCol	=	m_ctrl->GetNumberCols();
	}

	m_currentItem->selected = TRUE;

	if(m_blockJustStarted){
		m_blockJustStarted = FALSE;
	}
	// notify the user that the selection was changed
	m_ctrl->OnSelectionChanged( m_currentItem->hiCol, m_currentItem->loRow, 
								m_currentItem->loCol, m_currentItem->hiRow,
								m_numberBlocks );

	if(maxLoCol > m_currentItem->loCol)
		maxLoCol = m_currentItem->loCol;
	if(maxHiCol < m_currentItem->hiCol)
		maxHiCol = m_currentItem->hiCol;
	if(maxLoRow > m_currentItem->loRow)
		maxLoRow = m_currentItem->loRow;
	if(maxHiRow < m_currentItem->hiRow)
		maxHiRow = m_currentItem->hiRow;
	
	//add draw hints for the grid
	if(m_currentItem->endRow > oldEndRow){
		m_ctrl->m_CUGGrid->m_drawHint.AddHint(maxLoCol,
			oldEndRow,maxHiCol,m_currentItem->endRow);
	}
	else if(m_currentItem->endRow < oldEndRow){
		m_ctrl->m_CUGGrid->m_drawHint.AddHint(maxLoCol,
			m_currentItem->endRow,maxHiCol,oldEndRow);
	}
	if(m_currentItem->endCol > oldEndCol){
		m_ctrl->m_CUGGrid->m_drawHint.AddHint(oldEndCol,
			maxLoRow,m_currentItem->endCol,maxHiRow);
	}
	else if(m_currentItem->endCol < oldEndCol){
		m_ctrl->m_CUGGrid->m_drawHint.AddHint(m_currentItem->endCol,
			maxLoRow,oldEndCol,maxHiRow);
	}

	return UG_SUCCESS;
}

/***************************************************
ToggleCell
	function can be used to toggle cell's selected status.
	Although this function is not used by any of the grid's
	classes it can be called directly to (de)select cells.
Params:
	col, row	- coordinates of the cell to select/deselect
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGMultiSelect::ToggleCell(int col,long row)
{	
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_SUCCESS;

	//find the block
	int found = FALSE;
	UG_MSList * next = m_list;

	while(next != NULL)
	{
		if(col == next->loCol && col == next->hiCol)
		{
			if(row == next->loRow && row == next->hiRow)
			{
				if(next->selected)
					next->selected = FALSE;
				else
					next->selected = TRUE;

				found = TRUE;
				break;
			}
		}
		next = next->next;
	}
	//if there is no entry for this cell then add an entry
	if(!found)
	{
		//check to see if this cell is in a block	
		int selected = IsSelected(col,row);
		//add the cell
		StartBlock(col,row);
		if(selected)
			m_currentItem->selected =	FALSE;
		else
			m_currentItem->selected =	TRUE;
	}

	return UG_SUCCESS;
}

/***************************************************
GetCurrentBlock
	function returs coordinates of the block that user
	is currently working with (selecting).
Params:
	startCol,	- pointers to integers that can store
	startRow,	  information about selected range.
	endCol,
	endRow
Returns:
	UG_ERROR	- if no selection is currently under way
*****************************************************/
int CUGMultiSelect::GetCurrentBlock(int *startCol,long *startRow,int *endCol,long *endRow)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_SUCCESS;

	if(m_blockInProgress == FALSE)
		return UG_ERROR;

	*startCol =	m_currentItem->loCol;
	*endCol =	m_currentItem->hiCol;
	*startRow =	m_currentItem->loRow;
	*endRow =	m_currentItem->hiRow;

	return UG_SUCCESS;
}

/***************************************************
IsSelected
	is a function which answers a question: Is given
	cell currently selected?  The answer can be either
	TRUE (yes, it is selected) or FALSE.
Params:
	col, row	- coordinates of cell we are interested in
	block		- (optional) a pointer to an integer which
				  identified in which block this cell is 
				  selected.
Returns:
	TRUE, FALSE indentifying if given cell is selected.
*****************************************************/
int CUGMultiSelect::IsSelected(int col,long row,int *block)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return FALSE;

	if(m_list == NULL)
		return FALSE;

	int blockNum = 0;
	int blockCount = 0;

	//go through the list and check to see if the given
	//cell is in it
	int selected = FALSE;
	UG_MSList * next = m_list;

	while(next != NULL)
	{
		if(col >= next->loCol && col <= next->hiCol)
		{
			if(row >= next->loRow && row <= next->hiRow)
			{
				selected = next->selected;
				blockNum = blockCount;
				if(m_mode&UG_MULTISELECT_NODESELECT) //if no deselect
					break;
			}
		}

		blockCount++;
		
		next = next->next;
	}

	if(block != NULL)
		*block = blockNum;
	
	return selected;
}

/***************************************************
IsCellInColSelected
	function can be called to get information if
	at least one cell in given column is selected.
Params:
	col		- identifies column in question
Returns:
	TRUE, FALSE indicating success (cell found) or
			failure.
*****************************************************/
int CUGMultiSelect::IsCellInColSelected(int col)
{	
	if(m_mode&UG_MULTISELECT_OFF)
		return FALSE;

	if(m_list == NULL)
		return FALSE;

	if(m_mode&UG_MULTISELECT_ROW)
		col = 1;

	//go through the list and check to see if the given
	//cell is in it
	int selected = FALSE;
	UG_MSList * next = m_list;

	while(next != NULL)
	{
		if(col >= next->loCol && col <= next->hiCol)
		{
			selected = next->selected;
			if(m_mode&UG_MULTISELECT_NODESELECT) //if no deselect
				break;
		}

		next = next->next;
	}

	return selected;
}

/***************************************************
IsCellInRowSelected
	function can be called to get information if
	at least one cell in given row is selected.
Params:
	row		- indicating the row in question.
Returns:
	TRUE, FALSE indicating success (cell found) or
			failure.
*****************************************************/
int CUGMultiSelect::IsCellInRowSelected(long row)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return FALSE;

	if(m_list == NULL)
		return FALSE;

	//go through the list and check to see if the given
	//cell is in it
	int selected = FALSE;
	UG_MSList * next = m_list;

	while(next != NULL)
	{
		if(row >= next->loRow && row <= next->hiRow)
		{
			selected = next->selected;
			if(m_mode&UG_MULTISELECT_NODESELECT) //if no deselect
				break;
		}

		next = next->next;
	}

	return selected;
}

/***************************************************
SelectMode
	function is called to set new multiselection mode,
	this mode can be one of following:
		UG_MULTISELECT_OFF (0)	- sindle selection mode
		UG_MULTISELECT_CELL(1)	- cell multiselection mode enabled
		UG_MULTISELECT_ROW (2)	- row multiselection mode enabled

	The SelectMode can be called directly or as a result
	of a call to the CUGCtrl::SetMultiSelectMode.
Params:
	mode	- indicates the new selection mode to set.
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGMultiSelect::SelectMode(int mode)
{	
	ClearAll();

	m_mode = mode;
	
	return UG_SUCCESS;
}

/***************************************************
GetSelectMode
	returns current selection mode, as set in the SelectMode.
Params:
	<none>
Returns:
	int	- current state of the m_mode flag.
*****************************************************/
int CUGMultiSelect::GetSelectMode()
{
	return m_mode;
}

/***************************************************
GetNumberBlocks
	function can be used to retrieve number of currently
	selected blocks.  A block is a group of selected cells.
Params:
	<none>
Returns:
	integer value representing current number of blocks
*****************************************************/
int CUGMultiSelect::GetNumberBlocks()
{
	return m_numberBlocks;
}

/***************************************************
EnumFirstSelected
	is the first function to call when enumerating 
	through selected cells.  This function sets up
	all of the variables needed later when the
	EnumNextSelected is called.
Params:
	col, row	- pointers to integer variables, which
				  will be set to contain coordinates
				  of first selected cell.
Returns:
	UG_SUCCESS, this function will never fail, but
	UG_ERROR can be returned from EnumNextSelected
	upon failure.
*****************************************************/
int CUGMultiSelect::EnumFirstSelected(int *col,long *row)
{
	//multiselect off
	if(m_mode&UG_MULTISELECT_OFF)
	{
		*col = m_ctrl->GetCurrentCol();
		*row = m_ctrl->GetCurrentRow();
		return UG_SUCCESS;
	}

	//no items selected
	if(m_list == NULL)
	{
		*col = m_ctrl->GetCurrentCol();
		*row = m_ctrl->GetCurrentRow();
		return UG_SUCCESS;
	}

	//set up the vars
	m_enumInProgress= TRUE;
	m_enumStartCol	= m_ctrl->GetCurrentCol();
	m_enumStartRow	= m_ctrl->GetCurrentRow();
	m_enumEndCol	= m_ctrl->GetCurrentCol();
	m_enumEndRow	= m_ctrl->GetCurrentRow();
	
	//find the total selected region
	UG_MSList * next = m_list;

	while(next != NULL)
	{
		if(m_enumStartCol > next->loCol)
			m_enumStartCol = next->loCol;
		if(m_enumEndCol < next->hiCol)
			m_enumEndCol = next->hiCol;
		if(m_enumStartRow > next->loRow)
			m_enumStartRow = next->loRow;
		if(m_enumEndRow < next->hiRow)
			m_enumEndRow = next->hiRow;

		next = next->next;
	}
	
	m_enumRow = m_enumStartRow;
	m_enumCol = m_enumStartCol;

	return EnumNextSelected( col, row );
}

/***************************************************
EnumNextSelected
	is called to retrieve coordinates of the next
	selected cell.
Params:
	col, row	- pointers to integer variables, which
				  will be set to contain coordinates
				  of first selected cell.
Returns:
	UG_ERROR can be returned if the multiselection is not
		enabled, or if the EnumFirstSelected was not
		called first.
	UG_SUCCESS if everything went as planned.
*****************************************************/
int CUGMultiSelect::EnumNextSelected(int *col,long *row)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_ERROR;

	if(!m_enumInProgress)
		return UG_ERROR;

	//find the next item
	int xIndex;
	long yIndex;

	// start by looping through selected rows
	for( yIndex = m_enumRow; yIndex <= m_enumEndRow; yIndex++ )
	{
		if ( m_mode == UG_MULTISELECT_ROW )
		{
			if( IsSelected( m_enumStartCol, yIndex ))
			{
				m_enumCol = m_enumStartCol;
				m_enumRow = yIndex + 1;
				*col = m_enumStartCol;
				*row = yIndex;
				return UG_SUCCESS;
			}
		}
		else
		{
			for( xIndex = m_enumCol; xIndex <= m_enumEndCol; xIndex++ )
			{
				if( IsSelected( xIndex, yIndex ))
				{
					m_enumCol = xIndex + 1;
					m_enumRow = yIndex;
					*col = xIndex;
					*row = yIndex;
					return UG_SUCCESS;
				}
			}
		}
		m_enumCol = m_enumStartCol;
	}

	// end of the list was reached.
	m_enumInProgress = FALSE;

	return UG_ERROR;
}

/***************************************************
EnumFirstBlock
	is used to retrieve coordinates of the first selected
	block (continues range of cells) set by either
	SelectRange or by user's mouse selection action.
Params:
	startCol,	- pointers to integer valriables that
	startRow,	  will contain coordinate information
	endCol,		  of selected range
	endRow
Returns:
	The EnumFirstBlock does not return any error codes
	it depends on the EnumNextBlock to perform proper
	checking.
*****************************************************/
int CUGMultiSelect::EnumFirstBlock(int *startCol,long *startRow,int *endCol,long *endRow)
{
	m_enumBlockNumber = 0;
	m_enumInProgress = TRUE;

	return EnumNextBlock(startCol,startRow,endCol,endRow);
}

/***************************************************
EnumNextBlock
	retieves coordinate information on the next selected
	block.
Params:
	startCol,	- pointers to integer valriables that
	startRow,	  will contain coordinate information
	endCol,		  of selected range
	endRow
Returns:
	UG_ERROR if multiselection is off, or if user did not
	call EnumFirstBlock first, or if we have reached end 
	of selected blocks list.
*****************************************************/
int CUGMultiSelect::EnumNextBlock(int *startCol,long *startRow,int *endCol,long *endRow)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_ERROR;

	if(!m_enumInProgress)
		return UG_ERROR;

	int count			= 0;
	UG_MSList * next	= m_list;

	while(next != NULL && count < m_enumBlockNumber)
	{
		next = next->next;
		count++;
	}
	if(next != NULL)
	{
		*startCol = next->loCol;
		*startRow = next->loRow;
		*endCol = next->hiCol;
		*endRow = next->hiRow;

		m_enumBlockNumber ++;

		return UG_SUCCESS;
	}

	// end of the list was reached.
	m_enumInProgress = FALSE;
	return UG_ERROR;
}

/***************************************************
OnLClick
	is called by the grid control to inform the multislect
	class that the user has clicked a left mouse button
	over the grid area.
Params:
	col, row	- coordinates of the cell user clicked on
	nFlags		- additional information about the mouse click
Returns:
	<none>
*****************************************************/
void CUGMultiSelect::OnLClick(int col,long row, UINT nFlags)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return;

	m_lastCol = col;
	m_lastRow = row;

	if(nFlags&MK_CONTROL){
		StartBlock(col,row);
	}
	else if(nFlags&MK_SHIFT){
		EndBlock(col,row);
	}
	else{
		AddTotalRangeToDrawHints(&m_ctrl->m_CUGGrid->m_drawHint);
		ClearAll();
		StartBlock(col,row);
	}
}

/***************************************************
OnRClick
	is called by the grid control to inform the multislect
	class that the user has clicked a right mouse button
	over the grid area.
Params:
	col, row	- coordinates of the cell user clicked on
	nFlags		- additional information about the mouse click
Returns:
	<none>
*****************************************************/
void CUGMultiSelect::OnRClick(int col,long row, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nFlags);

	if(m_mode&UG_MULTISELECT_OFF)
		return;
	
	if(IsSelected(col,row) == FALSE)
	{
		AddTotalRangeToDrawHints(&m_ctrl->m_CUGGrid->m_drawHint);
		ClearAll();
		StartBlock(col,row);
	}
}

/***************************************************
OnKeyMove
	Is called when the user moves within the grid
	using the keyboard.
Params:
	col, row	- coordinates of of the cell that the
				  user has just mode to.
Returns:
	<none>
*****************************************************/
void CUGMultiSelect::OnKeyMove(int col,long row)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return;

	if(col == m_lastCol && row == m_lastRow)
		return;

	if(GetKeyState(VK_SHIFT) <0)
	{
		EndBlock(col,row);
	}
	else if(GetKeyState(VK_CONTROL) <0)
	{
		StartBlock(col,row);
	}
	else
	{
		AddTotalRangeToDrawHints(&m_ctrl->m_CUGGrid->m_drawHint);
		ClearAll();
		StartBlock(col,row);
	}
}

/***************************************************
OnMouseMove
	is called when the user selects cells using the mouse
	droagging functionality.
Params:
	col, row	- coordinates of the cell user moved over
	nFlags		- additional information about the mouse move
Returns:
	<none>
*****************************************************/
void CUGMultiSelect::OnMouseMove(int col,long row, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nFlags);

	if(m_mode&UG_MULTISELECT_OFF)
		return;

	if(col == m_lastCol && row == m_lastRow)
		return;
	
	EndBlock(col,row);
}

/***************************************************
GetTotalRange
	is used to calculate the total range covered by the
	selected cells.
Params:
	startCol,	- pointers variables that when successful
	startRow,	  will store coordinates of the Top/Left and
	endCol,		  Bottom/Right cells indicating the total
	endRow		  selected range.
Returns:
	UG_ERROR if the multiselection is disabled, or no
		cells are selected (m_list is NULL)
	otherwise UG_SUCCESS.
*****************************************************/
int CUGMultiSelect::GetTotalRange(int *startCol,long *startRow,int *endCol,long *endRow)
{
	if(m_mode&UG_MULTISELECT_OFF)
		return UG_ERROR;

	if(m_list == NULL)
		return UG_ERROR;

	if(m_mode&UG_MULTISELECT_ROW)
		*endCol = m_GI->m_numberCols -1;

	*startCol = m_list->loCol;
	*startRow = m_list->loRow;
	*endCol = m_list->hiCol;
	*endRow = m_list->hiRow;

	UG_MSList * next = m_list->next;

	while(next != NULL)
	{
		if(next->loCol < *startCol)
			*startCol = next->loCol;
		
		if(next->loRow < *startRow)
			*startRow = next->loRow;

		if(next->hiCol > *endCol)
			*endCol = next->hiCol;

		if(next->hiRow > *endRow)
			*endRow = next->hiRow;

		next = next->next;
	}

	return UG_SUCCESS;
}

/***************************************************
AddTotalRangeToDrawHints
	is called whenever the selected range is needed to be
	redrawn, usually when selection changed.  This will
	not cause gird's redraw, but when grid redraws next time
	range indicated here will be also redrawn.
Params:
	hint	- pointer an instance of the drawing hints class.
Returns:
	<none>
*****************************************************/
void CUGMultiSelect::AddTotalRangeToDrawHints(CUGDrawHint * hint)
{
	int startCol,endCol;
	long startRow,endRow;

	if(GetTotalRange(&startCol,&startRow,&endCol,&endRow) == UG_SUCCESS)
		hint->AddHint(startCol,startRow,endCol,endRow);
}
