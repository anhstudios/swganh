/*************************************************************************
				Class Implementation : CUGMem
**************************************************************************
	Source file : UGMemMan.cpp
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
CUGMem::CUGMem()
{
	//set up the variables
	m_currentRow	= 0;
	m_rowInfo		= NULL;
	m_currentCol	= 0;
	m_colInfo		= NULL;
	m_findRow		= 0;
}

CUGMem::~CUGMem()
{
	//clear the variables
	Empty();
}

/***************************************************
************ row and col info **********************
****************************************************/

/***************************************************
GotoRow - Private
	Moves the internal pointers to the specified row
	The column will automatically be reset to 0 when
	this function is called. So generally GotoCol is
	required after this call.
Params
	row - the row to go to.
Return
	UG_SUCCESS	success
	UG_ERROR	fail
****************************************************/
int CUGMem::GotoRow(long row)
{
	if(m_rowInfo == NULL)
		return UG_ERROR;

	//find the correct row
	if(row != m_currentRow)
	{
		//if the row is greater than the current, then move forward
		while(row > m_currentRow)
		{
			if(m_rowInfo->next != NULL)
			{
				m_rowInfo = m_rowInfo->next;
				m_currentRow++;
			}
			else
			{
				//update the current column pointer
				m_colInfo		= m_rowInfo->col;
				m_currentCol	= 0; 
				return UG_ERROR;
			}
		}
		//if the row is less than the current, then move back
		while(row < m_currentRow)
		{
			if(m_rowInfo->prev != NULL)
			{
				m_rowInfo = m_rowInfo->prev;
				m_currentRow--;
			}
			else
			{
				//update the current column pointer
				m_colInfo		= m_rowInfo->col;
				m_currentCol	= 0; 
				return UG_ERROR;
			}
		}

		//update the current column pointer
		m_colInfo		= m_rowInfo->col;
		m_currentCol	= 0; 
	}

	return UG_SUCCESS;
}

/***************************************************
GotoCol - Private
	Moves the internal pointers to the specified
	column (for the current row)
Params
	col - the column to go to.
Return
	UG_SUCCESS	success - the column exists
	UG_ERROR	fail - the column does not exist
****************************************************/
int CUGMem::GotoCol(int col)
{
	if(m_colInfo==NULL)
		return UG_ERROR;

	//find the correct column
	if(col != m_currentCol)
	{
		while(col > m_currentCol)
		{
			if(m_colInfo->next != NULL)
			{
				m_colInfo = m_colInfo->next;
				m_currentCol++;
			}
			else
				return UG_ERROR;
		}

		while(col < m_currentCol)
		{
			if(m_colInfo->prev != NULL)
			{
				m_colInfo = m_colInfo->prev;
				m_currentCol--;
			}
			else
				return UG_ERROR;
		}
	}
	if(m_colInfo != NULL)
		return UG_SUCCESS;
	return UG_ERROR;
}

/***************************************************
PrevRow - Private
	Moves the current row to the previous row. If there
	are no previous rows then the function will fail
Params
	none
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure, no previous row exists
****************************************************/
int CUGMem::PrevRow()
{
	if(m_rowInfo == NULL)
		return UG_ERROR;

	if(m_rowInfo->prev != NULL)
	{
		//update the row pointer
		m_rowInfo = m_rowInfo->prev;
		m_currentRow--;
		//update the current column pointer
		m_colInfo		= m_rowInfo->col;
		m_currentCol	= 0; 

		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/***************************************************
NextRow - Private
	Moves the current row to the next row. If there
	are no more rows then the function will fail
Params
	none
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure, no more row exists
****************************************************/
int CUGMem::NextRow()
{
	if(m_rowInfo == NULL)
		return UG_ERROR;

	if(m_rowInfo->next != NULL)
	{
		//update the row pointer
		m_rowInfo = m_rowInfo->next;
		m_currentRow++;
		//update the current column pointer
		m_colInfo		= m_rowInfo->col;
		m_currentCol	= 0; 

		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/***************************************************
PrevCol - Private
	Moves the current column to the previous column.
	If there are no previous coluumns then the function 
	will fail.
Params
	none
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure, no previous column exists
****************************************************/
int CUGMem::PrevCol()
{
	if(m_colInfo==NULL)
		return UG_ERROR;

	if(m_colInfo->prev != NULL)
	{
		//update the current column pointer
		m_colInfo		= m_colInfo->prev;
		m_currentCol--; 

		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/***************************************************
NextCol - Private
	Moves the current column to the next column.
	If there are no more coluumns then the function 
	will fail.
Params
	none
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure, no more columns exists
****************************************************/
int CUGMem::NextCol()
{
	if(m_colInfo==NULL)
		return UG_ERROR;

	if(m_colInfo->next != NULL)
	{
		//update the current column pointer
		m_colInfo		= m_colInfo->next;
		m_currentCol++; 

		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/***************************************************
GetNumRows
	Returns the number of rows allocated in the 
	memory datasource. This number may not be the 
	same as the number of rows shown in the grid
	(see CUGCtrl::GetNumRows). This function only 
	returns the number of rows that have been allocated
	in the memory datasource.
Params
	none
Return
	the number of allocated rows
****************************************************/
long CUGMem::GetNumRows()
{
	//find the highest row number
	GotoRow(0x7FFFFFFF);
	return m_currentRow;
}
	
/***************************************************
************* add-update-clear *********************
****************************************************/

/***************************************************
AppendRow
	This function does not need to do anything, since
	cells are automaticly appended during the SetCell
	routine if it does not exist
Params
	none
Return
	UG_SUCCESS  - success
****************************************************/
int CUGMem::AppendRow()
{
	return UG_SUCCESS;
}

/***************************************************
InsertRow
	This function inserts a blank row at the specified
	location. The previous row in that location is 
	pushed down one row. If the specified row does not
	already exist, then the Insert will fail.
Params
	row - row number where a blank row will be inserted
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::InsertRow(long row)
{
	//adjust the linked list if the list has an item at the insertion point
	if(GotoRow(row)==UG_SUCCESS)
	{
		//create a new row information item for the list
		//then insert it 
		UGMemRI *newrow;
		newrow			= new UGMemRI;
		newrow->col		= NULL;
		newrow->next	= m_rowInfo;
		newrow->prev	= m_rowInfo->prev;
		newrow->cellLookup = NULL;

		if(newrow->prev != NULL)
			newrow->prev->next = newrow;

		m_rowInfo->prev = newrow;
		m_currentRow++;
		return UG_SUCCESS;
	}
	else
	{
		return UG_ERROR;
	}
}

/***************************************************
AppendRow
	This function does not need to do anything, since
	cells are automaticly appended during the SetCell
	routine if it does not exist
Params
	none
Return
	UG_SUCCESS  - success
****************************************************/
int CUGMem::AppendCol()
{
	return UG_SUCCESS;
}

/***************************************************
InsertCol
	This function inserts a blank column at the specified
	location. The previous column in that location is 
	pushed right one column. If the specified column does not
	already exist for a given row, then that row will be
	skipped and the insert will continue.
Params
	col - column number where a blank column will be inserted
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure (no rows in grid)
****************************************************/
int CUGMem::InsertCol(int col)
{
	//goto the first row
	while(PrevRow()==UG_SUCCESS);

	//check to see if there is a first row
	if(m_rowInfo == NULL)
		return UG_ERROR;

	//get the screen position column number
	int numCols = m_ctrl->GetNumberCols();
	if(numCols > 0)
	{
		for(int loop = 0 ; loop < numCols;loop++)
		{
			if(m_ctrl->m_GI->m_colInfo[loop].colTranslation == col)
			{
				col = loop;
				break;
			}
		}
	}

	//move the 'col' column over 
	UGMemCI *newcol;
	do
	{
		if(GotoCol(col) ==UG_SUCCESS)
		{
			newcol			= new UGMemCI;
			newcol->cell	= NULL;
			newcol->next	= m_colInfo;
			newcol->prev	= m_colInfo->prev;
			
			if(newcol->prev != NULL)
				newcol->prev->next = newcol;

			if(m_rowInfo->col == m_colInfo)
				m_rowInfo->col = newcol;

			m_colInfo->prev = newcol;
			m_currentCol ++;

		}
	}while(NextRow()==UG_SUCCESS);

	//adjust the column translations
	if(numCols > 0)
	{
		for(int loop = 0 ; loop < numCols;loop++)
		{
			if(m_ctrl->m_GI->m_colInfo[loop].colTranslation >= col)
				m_ctrl->m_GI->m_colInfo[loop].colTranslation++;
		}
	}

	return UG_SUCCESS;
}

/***************************************************
DeleteRow
	This function deletes a row at the specified 
	position. Even if the row does not exist the 
	function will return success.
Params
	row - row number to delete
Return
	UG_SUCCESS  - success
****************************************************/
int CUGMem::DeleteRow(long row)
{
	//adjust the linked list if the list has an item at the insertion point
	if(GotoRow(row)==UG_SUCCESS)
	{
		//delete the columns for this row		
		UGMemCI *next;
		while(PrevCol()==UG_SUCCESS);  //find the first col
		while(m_colInfo != NULL)       //delete the cols to the end
		{
			next = m_colInfo->next;
				
			//delete the colinfo
			if(m_colInfo->cell != NULL)
				delete m_colInfo->cell;
			delete m_colInfo;

			m_colInfo = next;
		}

		//update the row links
		UGMemRI *currentrow;
		if(m_rowInfo->prev != NULL)
			m_rowInfo->prev->next = m_rowInfo->next;
		if(m_rowInfo->next != NULL)
		{
			m_rowInfo->next->prev = m_rowInfo->prev;
			currentrow = m_rowInfo->next;
		}
		else if(m_rowInfo->prev != NULL)
		{
			currentrow = m_rowInfo->prev;
			m_currentRow--;
		}
		else
		{
			currentrow = NULL;
			m_currentRow = 0;
		}
		delete m_rowInfo;
		m_rowInfo = currentrow;

		//update the current column pointer
		if(m_rowInfo != NULL)
			m_colInfo	= m_rowInfo->col;
		else
			m_colInfo	= NULL;
		m_currentCol	= 0; 

		return UG_SUCCESS;
	}

	//return success even if there are no rows to delete
	//since the memory manager is autoupdating
	return UG_SUCCESS;	
}

/***************************************************
DeleteCol
	This function deletes a column at the specified 
	position. Even if the column does not exist the 
	function will return success. However if there
	are no rows in the grid the function will fail.
Params
	col - column number to delete
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::DeleteCol(int col)
{
	//goto the first row
	while(PrevRow()==UG_SUCCESS);

	//check to see if there is a first row
	if(m_rowInfo == NULL)
		return UG_ERROR;

	//move the 'col' column over 
	UGMemCI *colinfo;
	do{
		if(GotoCol(col) ==UG_SUCCESS)
		{
			// delete the columninfo if there are columns to the right
			if(m_colInfo->next != NULL){
				//update the links
				colinfo = m_colInfo->prev;
				if(colinfo != NULL)
					colinfo->next = m_colInfo->next;

				colinfo = m_colInfo->next;
				if(colinfo != NULL)
					colinfo->prev = m_colInfo->prev;

				if(m_rowInfo->col == m_colInfo)
					m_rowInfo->col = colinfo;
	
				//delete the colinfo
				if(m_colInfo->cell != NULL)
					delete m_colInfo->cell;
				delete m_colInfo;
			}
			// else if there are no more column to the right, then keep the
			// colinfo, but delete the cellinfo within the colinfo
			// since this may be the initial link to the rowinfo list
			else{
				delete m_colInfo->cell;
				m_colInfo->cell = NULL;
			}
		}
	}while(NextRow()==UG_SUCCESS);

	//adjust the column translations
	int numCols = m_ctrl->GetNumberCols();
	if(numCols > 0)
	{
		for(int loop = 0 ; loop < numCols;loop++)
		{
			if(m_ctrl->m_GI->m_colInfo[loop].colTranslation >= col)
				m_ctrl->m_GI->m_colInfo[loop].colTranslation--;
		}
	}

	//update the current column pointer
	m_colInfo		= m_rowInfo->col;
	m_currentCol	= 0; 

	return UG_SUCCESS;
}
/***************************************************
DeleteCol
	This function deletes a cell at the specified 
	location. Even if the cell does not exist the 
	function will return success. However if there
	are no rows in the grid the function will fail.
Params
	col - column number of cell to delete
	row - row number of cell to delete
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::DeleteCell(int col,long row)
{
	if(GotoRow(row)==UG_SUCCESS)
	{
		if(GotoCol(col)==UG_SUCCESS)
		{
			//delete the colinfo cell object
			if(m_colInfo->cell != NULL)
			{
				m_colInfo->cell->ClearMemory();
				delete m_colInfo->cell;
				m_colInfo->cell = NULL;
			}

			m_colInfo		= m_rowInfo->col;
			m_currentCol	= 0; 

			return UG_SUCCESS;
		}
	}
	return UG_ERROR;
}
/***************************************************
Empty
	Deletes the memory that was allocated to store
	the data. This does not change the number of rows
	or columns shown in the grid.
Params
	none
Return
	UG_SUCCESS  - success
****************************************************/
int CUGMem::Empty()
{
	UGMemRI *	row = m_rowInfo;
	UGMemCI	*	col;
	UGMemRI *	nextrow;
	UGMemCI	*	nextcol;

	//release the memory
	//find the first row
	while(row != NULL)
	{
		if(row->prev == NULL)
			break;
		row = row->prev;
	}

	while(row != NULL)
	{	
		//find the first col
		col = row->col;
		while(col != NULL)
		{
			if(col->prev ==NULL)
				break;
			col = col->prev;
		}

		int cols = 0;
		UGMemCI	*	firstCol = col;
		for(;col;col = col->next, ++cols);
		col = firstCol;


		//delete any columns that are attached to the row
		while(col != NULL)
		{
			nextcol = col->next;
			// Because of problems with creating a cell inside a cell,
			// and the cell class being used to create temporary copies of cells,
			// this method allows us to clean up pointers such as the initialstate cell.
			if(col->cell)
			{
		        	col->cell->ClearMemory();
		        	delete col->cell;
				col->cell = NULL;
			}
			delete col;
			col = nextcol;
		}

		//delete the row information
		nextrow = row->next;
		delete row;
		row = nextrow;
	}
	
	//reset the variables
	m_currentRow	= 0;
	m_rowInfo		= NULL;
	m_currentCol	= 0;
	m_colInfo		= NULL;

	return UG_SUCCESS;
}

/***************************************************
****************** cell info ***********************
****************************************************/

/***************************************************
GetCell
	Copies the cell data from the specified cell
	position into the given cell object. If the
	specified cell does not exist, then the function
	will fail.
Params
	col - the column number of the cell to retrieve
	row - the row number of the cell to retrieve
	cell - pointer to an existing cell object to 
		copy the given cells data into. This pointer
		must not be NULL.
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::GetCell(int col,long row,CUGCell *cell)
{
	//TODO
	/* 
	//check to see if there are no cells at all
	if(m_rowInfo == NULL)
		return UG_ERROR;

	//find the correct row
	if(GotoRow(row) == UG_SUCCESS){
		if(GotoCol(col) == UG_SUCCESS){
			// ***** copy the information over *****
			cell->AddInfoFrom(m_colInfo->cell);
			return UG_SUCCESS;
		}
	}

	return UG_ERROR;
	*/

	//check to see if there are no cells at all
	if(m_rowInfo == NULL)
		return UG_ERROR;
	

	//find the correct row
	if(row != m_currentRow)
	{
		while(row > m_currentRow)
		{
			if(m_rowInfo->next != NULL)
			{
				m_rowInfo = m_rowInfo->next;
				m_currentRow++;
			}
			else
			{
				m_colInfo		= m_rowInfo->col;
				m_currentCol	= 0; 
				return UG_ERROR;
			}
		}
		while(row < m_currentRow)
		{
			if(m_rowInfo->prev != NULL)
			{
				m_rowInfo = m_rowInfo->prev;
				m_currentRow--;
			}
			else
			{
				m_colInfo		= m_rowInfo->col;
				m_currentCol	= 0; 
				return UG_ERROR;
			}
		}
		m_colInfo = m_rowInfo->col;
		m_currentCol = 0; 
	}

	//check to see if a col info exists at all for this row
	if(m_colInfo == NULL)
		return UG_ERROR;

	//find the correct column
	if(col != m_currentCol)
	{
		while(col > m_currentCol)
		{
			if(m_colInfo->next != NULL)
			{
				m_colInfo = m_colInfo->next;
				m_currentCol++;
			}
			else
				return UG_ERROR;
		}
		while(col < m_currentCol)
		{
			if(m_colInfo->prev != NULL)
			{
				m_colInfo = m_colInfo->prev;
				m_currentCol--;
			}
			else
				return UG_ERROR;
		}
	}

	
	//if the cell does not exist then create it
	if(m_colInfo->cell == NULL)
	{
		return UG_ERROR;
	}

	// ***** copy the information over *****
	cell->AddInfoFrom(m_colInfo->cell);

	return UG_SUCCESS;
}

/***************************************************
SetCell
	Sets/stores the given cell information into the memory
	datasource. If the specified cell has not been allocated
	yet, then it will be allocated (extra rows and/or columns
	will be added if required).
Params
	col - the column number to set the cell information into
	row - the row number to set the cell information into
	cell - pointer to the cell object to store the information from
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::SetCell(int col,long row,CUGCell *cell)
{
	UGMemRI *newrow;
	UGMemCI *newcol;

	//check to see if the memory manager is empty, if
	//so then add a blank item
	if(m_rowInfo == NULL)
	{
		m_rowInfo		= new UGMemRI;
		m_rowInfo->col	= NULL;
		m_rowInfo->next	= NULL;
		m_rowInfo->prev	= NULL;
		m_rowInfo->cellLookup = NULL;

		m_currentRow =0;		// TD user JF offered a fix where this is 
								// changed to '=row' - unsure of reason for fix.
	}

	//find the correct row
	if(row != m_currentRow)
	{
		while(row > m_currentRow)
		{
			//check to see if the row exists
			if(m_rowInfo->next != NULL)
			{
				m_rowInfo = m_rowInfo->next;
				m_currentRow++;
			}
			//if the row does not then create it
			else
			{
				newrow			= new UGMemRI;
				newrow->col		= NULL;
				newrow->next	= NULL;
				newrow->prev	= m_rowInfo;
				newrow->cellLookup = NULL;
				m_rowInfo->next = newrow;

				m_rowInfo = m_rowInfo->next;
				m_currentRow++;
			}
		}
		while(row < m_currentRow)
		{
			//check to see if the row exists
			if(m_rowInfo->prev != NULL)
			{
				m_rowInfo = m_rowInfo->prev;
				m_currentRow--;
			}
			//if the row does not then create it
			else
			{
				newrow			= new UGMemRI;
				newrow->col		= NULL;
				newrow->next	= m_rowInfo;
				newrow->prev	= NULL;
				newrow->cellLookup = NULL;
				m_rowInfo->prev = newrow;

				m_rowInfo = m_rowInfo->prev;
				m_currentRow--;
			}
		}
		m_colInfo = m_rowInfo->col;
		m_currentCol = 0; 
	}

	//check to see if a colinfo exists for this row
	if(m_colInfo == NULL)
	{
		m_colInfo		= new UGMemCI;
		m_colInfo->cell	= NULL;
		m_colInfo->next	= NULL;
		m_colInfo->prev	= NULL;
		m_currentCol	= 0; 
		m_rowInfo->col	= m_colInfo;
	}

	//find the correct column
	if(col != m_currentCol)
	{
		while(col > m_currentCol)
		{
			//check to see if the col exists
			if(m_colInfo->next != NULL)
			{
				m_colInfo = m_colInfo->next;
				m_currentCol++;
			}
			//if the col does not then create it
			else
			{
				newcol			= new UGMemCI;
				newcol->cell	= NULL;
				newcol->next	= NULL;
				newcol->prev	= m_colInfo;
				m_colInfo->next = newcol;

				m_colInfo = m_colInfo->next;
				m_currentCol ++;
			}
		}
		while(col < m_currentCol)
		{
			//check to see if the col exists
			if(m_colInfo->prev != NULL)
			{
				m_colInfo = m_colInfo->prev;
				m_currentCol--;
			}
			//if the col does not then create it
			else
			{
				newcol			= new UGMemCI;
				newcol->cell	= NULL;
				newcol->next	= m_colInfo;
				newcol->prev	= NULL;
				m_colInfo->prev = newcol;

				m_colInfo = m_colInfo->prev;
				m_currentCol --;
			}
		}
	}

	//check to see if a cell info structure exists
	if(m_colInfo->cell == NULL)
		m_colInfo->cell = new CUGCell;
	
	// ***** copy the information over *****
	cell->CopyInfoTo(m_colInfo->cell);
	
	return UG_SUCCESS;
}

/***************************************************
****************  finding sorting ******************
****************************************************/

/***************************************************
FindFirst
	Returns the column and row numbers of the first
	cell that matches the given string. How the match
	is performed is dependant on the specified flags.

Params
	string	- string to find
	col		- [in] column to perform the search on
			  [out] pointer to the column number of the cell that matches
	row		- [out] pointer to the row number of the cell that matches
	flags	- flags that modify how the find is performed
			  UG_FIND_CASEINSENSITIVE - all matching is insensitive
			  UG_FIND_PARTIAL - matches all cells that contains 
				the search string as a substring, otherwise the
				strings must match exactly
			  UG_FIND_ALLCOLUMNS - searches all columns in the grid
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::FindFirst(CString *string,int *col,long *row,long flags){

	if(flags&UG_FIND_ALLCOLUMNS)
	{
		*col = -1;
		*row = 0;
	}
	else
	{
		*row = -1;
	}

	return FindNext(string,col,row,flags);

}
/***************************************************
FindNext
	Returns the column and row numbers of the next
	cell that matches the given string. How the match
	is performed is dependant on the specified flags.

Params
	string	- string to find
	col		- [in] column to start to perform the search on. If searching
			  all columns, then this specifies the column to continue the
			  search from.
			  [out] pointer to the column number of the cell that matches
	row		- [in] row number to start searching from
			  [out] pointer to the row number of the cell that matches
	flags	- flags that modify how the find is performed
			  UG_FIND_CASEINSENSITIVE - all matching is insensitive
			  UG_FIND_PARTIAL - matches all cells that contains 
				the search string as a substring, otherwise the
				strings must match exactly
			  UG_FIND_ALLCOLUMNS - searches all columns in the grid
			  UG_FIND_UP - search upwards from the specified row, the
				default search is down.
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure
****************************************************/
int CUGMem::FindNext(CString *string,int *col,long *row,int flags)
{
	BOOL success = FALSE;
	CString tempStr;

	//goto the first col/row
	m_findRow = *row;
	m_findCol = *col;

	long lastRow = m_ctrl->GetNumberRows()-1;
	int  lastCol = m_ctrl->GetNumberCols()-1;
	long searchLimit = lastRow+2;		// search with wrap
	long rowCount = 0;
	BOOL colsDone = FALSE;

	// note: there could conceivably be a mismatch
	// here.  Not all columns of the grid are necessarily
	// bound to this datasource.  On the other hand, this
	// does limit us to searching that subset of the 
	// the ds shown on the grid.  (assuming col translation
	// has not been applied to intevening rows...)


	if(flags&UG_FIND_CASEINSENSITIVE)
		string->MakeUpper();	// do this once
	
	// initial positioning 
	if(flags&UG_FIND_ALLCOLUMNS)
	{
		if((flags&UG_FIND_UP)) 
		{
			m_findCol--;
			if(m_findCol < 0) 
			{
				m_findCol = lastCol;
				m_findRow--;
				if(m_findRow < 0)
					m_findRow = lastRow;
			}
		}
		else
		{
			m_findCol++;
			if(m_findCol > lastCol) 
			{
				m_findCol = 0;
				m_findRow++;
				if(m_findRow > lastRow)
					m_findRow = 0;
			}
		}
	}
	else
	{
		if(!(flags&UG_FIND_UP)) 
		{
			m_findRow++;
			if(m_findRow > lastRow)
				m_findRow = 0;
		}
		else 
		{
			m_findRow--;
			if(m_findRow < 0) 
				m_findRow = lastRow;
		}
	}
	// end initial positioning

	// loop through all rows plus wrap one row
	while(1)
	{
		if(rowCount >= searchLimit)
			return UG_ERROR;
		
		if(GotoRow(m_findRow) != UG_SUCCESS) 
		{
			// goto next row
			if(!(flags&UG_FIND_UP)) 
			{
				m_findRow++;
				if(m_findRow > lastRow)
					m_findRow = 0;
			}
			else 
			{
				m_findRow--;
				if(m_findRow < 0) 
					m_findRow = lastRow;
			}
			// increment count
			rowCount++;
			continue;
		}

		// ok - if we get here, the row is valid.  Now, find a valid col
		colsDone = FALSE;
		while(!colsDone) 
		{
			if(GotoCol(m_findCol) == UG_SUCCESS) 
			{
				if(NULL != m_colInfo->cell) 
				{
					// check for match
					if(flags&UG_FIND_CASEINSENSITIVE)
					{
						if(flags&UG_FIND_PARTIAL)
						{
							tempStr = m_colInfo->cell->GetText();
							tempStr.MakeUpper();
							if( tempStr.Find(*string) >= 0 )
								success = TRUE;
						}
						else
						{
							tempStr = m_colInfo->cell->GetText();
							if( tempStr.IsEmpty() == 0 )
							{
								if( string->CompareNoCase( tempStr ) == 0 )
									success = TRUE;
							}
						}
					}
					else
					{
						if(flags&UG_FIND_PARTIAL)
						{
							tempStr = m_colInfo->cell->GetText();
							if( tempStr.Find(*string) >= 0 )
								success = TRUE;
						}
						else
						{
							tempStr = m_colInfo->cell->GetText();
							if( tempStr.IsEmpty() == 0 )
							{
								if( string->Compare( tempStr ) == 0 )
									success = TRUE;
							}
						}
					}

					if(success)
					{
						*col = m_findCol;
						*row = m_findRow;
						return UG_SUCCESS;
					}
				}
			}
			// move to next col and/or row
			if(flags&UG_FIND_ALLCOLUMNS) 
			{
				if((flags&UG_FIND_UP)) 
				{
					m_findCol--;
					if(m_findCol < 0) 
					{
						colsDone = TRUE;
						m_findCol = lastCol;
						m_findRow--;
						rowCount++;
						if(m_findRow < 0)
							m_findRow = lastRow;
					}
				}
				else
				{
					m_findCol++;
					if(m_findCol > lastCol) 
					{
						colsDone = TRUE;
						m_findCol = 0;
						m_findRow++;
						rowCount++;
						if(m_findRow > lastRow) 
							m_findRow = 0;
					}
				}
			}
			else 
			{
				colsDone = TRUE;
				if(flags&UG_FIND_UP) 
				{
					m_findRow--;
					rowCount++;
					if(m_findRow < 0)
						m_findRow = lastRow;
				}
				else 
				{
					m_findRow++;
					rowCount++;
					if(m_findRow > lastRow)
						m_findRow = 0;
				}

			}
					
		}   // end while(!colsDone)	

	}		// end while(1)

	// failed to find match...
	return UG_ERROR;
}

/***************************************************
SortBy
	Sorts the rows in the grid based on the given column
	the order of the sort depends on the given flags.
	Note: This function will not redraw the grid after
	the sort is performed. Once a redraw is required call
	CUGCtrl::RedrawAll()
Params
	col - column to sort the grid by
	flags - UG_SORT_ASCENDING or UG_SORT_DESCENDING
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure	
****************************************************/
int CUGMem::SortBy(int col,int flags)
{
	return SortBy(&col,1,flags);
}

/***************************************************
SortBy
	Sorts the rows in the grid based on the given array
	of columns. The order in which the column numbers
	appear in the array determine how the sub-sorts are
	performed, with [0] being the primary sort.
	The order of the sort depends on the given flags.
	Note: This function will not redraw the grid after
	the sort is performed. Once a redraw is required call
	CUGCtrl::RedrawAll()

	new sort features
	 - halfway point checking
	 - row skipping

Params
	cols - column number array to sort the grid by
	numCols - number of column numbers in the array
	flags - UG_SORT_ASCENDING or UG_SORT_DESCENDING
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure	
****************************************************/
int CUGMem::SortBy(int *cols,int numCols,int flags)
{
	return SortBy(0,0x7FFFFFFF,cols,numCols,flags);
}

int CUGMem::SortBy(long startRow, long endRow, int *cols,int numCols,int flags)
{
	if(m_rowInfo == NULL)
		return UG_ERROR;

	if(startRow >= endRow)
		return UG_ERROR;


	UGMemRI *OldListRowInfo;
	UGMemRI *OldListNextRowInfo;
	UGMemRI *NewListRowInfo;
	UGMemRI *NewListStartRowInfo;

	UGMemRI *NewListRowInfo_HalfPt;
	
	CUGCell* cell1;
	CUGCell* cell2;
	CUGCell	 blankCell;

	int		index;
	int		rt;
	BOOL    bInserted = FALSE;
	int		halfPtOffset = 0;

	BOOL	bSkip;
	UGMemRI* lastSkipRow;

	long	rowCount = startRow;
		
	//goto the first row, and copy the first row over to the sorted list
	GotoRow(startRow);
	NewListRowInfo		= m_rowInfo;
	NewListStartRowInfo = m_rowInfo;
	NewListRowInfo_HalfPt = NewListRowInfo;
	//goto the second row and make that the unsorted list
	OldListRowInfo = m_rowInfo->next;	
	m_rowInfo->next = NULL;

	//copy the (pointers to) cells from the columns to be sorted into the lookup list
	NewListRowInfo->cellLookup = new CUGCell*[numCols];
	m_colInfo = NewListRowInfo->col;
	m_currentCol = 0;	// TD JF added (for init as below) 
	for(index =0; index < numCols;index++){
		if(GotoCol(cols[index])==UG_SUCCESS)
			NewListRowInfo->cellLookup[index] = m_colInfo->cell;
		else
			NewListRowInfo->cellLookup[index] = NULL;
	}

	
	//main row sort loop - while there are items in the unsorted list
	while(OldListRowInfo != NULL && (rowCount < endRow) ){
		
		//save the next row pointer
		OldListNextRowInfo = OldListRowInfo->next;

		//copy the (pointers to) cells from the columns to be sorted into the lookup list
		OldListRowInfo->cellLookup = new CUGCell*[numCols];
		m_colInfo = OldListRowInfo->col;
		m_currentCol = 0;
		for(index =0; index < numCols;index++){
			if(GotoCol(cols[index])==UG_SUCCESS)
			{
				OldListRowInfo->cellLookup[index] = m_colInfo->cell;
			}
			else
			{
				OldListRowInfo->cellLookup[index] = NULL;
		}
		}

		//check the half point 
		//================================================
		//get a cell from the unsorted list
		if(OldListRowInfo->cellLookup[0] != NULL)
			cell1 = OldListRowInfo->cellLookup[0];
		else{
			blankCell.ClearAll();
			blankCell.ClearMemory();
			cell1 = &blankCell;
		}

		//get a cell from the sorted list
		if(NewListRowInfo_HalfPt->cellLookup[0] != NULL)
			cell2 = NewListRowInfo_HalfPt->cellLookup[0];
		else{
			blankCell.ClearAll();
			blankCell.ClearMemory();
			cell2 = &blankCell;
		}
		
		//call the evaluation function
		rt = m_ctrl->OnSortEvaluate(cell1,cell2,flags);
		if(rt <= 0){
			NewListRowInfo = NewListStartRowInfo;
			halfPtOffset -=1;
		}
		else{
			NewListRowInfo = NewListRowInfo_HalfPt;
			halfPtOffset +=1;
		}
		if(halfPtOffset == -2){
			halfPtOffset = 0;
			if(NewListRowInfo_HalfPt->prev != NULL)
				if(NewListRowInfo_HalfPt->prev->cellLookup != NULL)
					NewListRowInfo_HalfPt = NewListRowInfo_HalfPt->prev;
		}
		if(halfPtOffset == 2){
			halfPtOffset = 0;
			if(NewListRowInfo_HalfPt->next != NULL)
				NewListRowInfo_HalfPt = NewListRowInfo_HalfPt->next;
		}
		//================================================

		bSkip = TRUE;
		lastSkipRow = NULL;

		//main loop to check an unsorted item against the sorted list
		//NewListRowInfo = NewListStartRowInfo;
		bInserted = FALSE;
		while(NewListRowInfo != NULL){

			//sort by the columns in their given order
			for(index = 0; index < numCols; index++){

				//get a cell from the unsorted list
				if(OldListRowInfo->cellLookup[index] != NULL)
					cell1 = OldListRowInfo->cellLookup[index];
				else{
					blankCell.ClearAll();
					blankCell.ClearMemory();
					cell1 = &blankCell;
				}

				//get a cell from the sorted list
				if(NewListRowInfo->cellLookup[index] != NULL)
					cell2 = NewListRowInfo->cellLookup[index];
				else{
					blankCell.ClearAll();
					blankCell.ClearMemory();
					cell2 = &blankCell;
				}
				
				//call the evaluation function
				rt = m_ctrl->OnSortEvaluate(cell1,cell2,flags);
				if(rt < 0){					
					if(bSkip && lastSkipRow != NULL){
						NewListRowInfo = lastSkipRow;
						bSkip = FALSE;
						break;
					}
					OldListRowInfo->next = NewListRowInfo;
					OldListRowInfo->prev = NewListRowInfo->prev;
					NewListRowInfo->prev = OldListRowInfo;
					if(OldListRowInfo->prev !=NULL)
						OldListRowInfo->prev->next = OldListRowInfo;
					if(NewListRowInfo == NewListStartRowInfo)
						NewListStartRowInfo = OldListRowInfo;
					bInserted = TRUE;
					rowCount++;
					break;
				}
				//if equal then check the next column to sort by
				else if(rt == 0){
				}
				else{
					break;
				}
			}
		
			//if the item was inserted into the sorted then then break
			if(bInserted == TRUE)
				break;

			//if at the end of the sorted list then add the item
			if(NewListRowInfo->next == NULL){
				NewListRowInfo->next = OldListRowInfo;
				OldListRowInfo->prev = NewListRowInfo;
				OldListRowInfo->next = NULL;
				rowCount++;
				break;
			}

			//perform row skips
			if(bSkip){
				lastSkipRow = NewListRowInfo;
				for(int loop = 0; loop< 100; loop++){
					if(NewListRowInfo->next != NULL){
						NewListRowInfo = NewListRowInfo->next;
					}
					else{
						NewListRowInfo = lastSkipRow;
						bSkip = FALSE;
						break;
					}
				}
			}
			else{
				NewListRowInfo = NewListRowInfo->next;
			}
		}

		OldListRowInfo = OldListNextRowInfo;
	}

	//reset the linked list positioning pointers
	m_currentRow = startRow;
	m_currentCol = 0;
	m_rowInfo = NewListStartRowInfo;
	m_colInfo = NewListStartRowInfo->col;

	//add the rest of the rows, if they were not part of the sort
	if(rowCount == endRow && OldListRowInfo != NULL){
		GotoRow(endRow);
		m_rowInfo->next = OldListRowInfo;
		OldListRowInfo->prev = m_rowInfo;
		
		m_currentRow = startRow;
		m_rowInfo = NewListStartRowInfo;
	}

	//delete all of the cell lookups
	GotoRow(0);
	do
	{
		if(m_rowInfo->cellLookup != NULL)
		{
			delete[] m_rowInfo->cellLookup;
		}
		m_rowInfo->cellLookup = NULL;
	}while(NextRow() == UG_SUCCESS);
	GotoRow(0);

	return UG_SUCCESS;
}

/***************************************************
GetPrevNonBlankCol
	Returns the previous column (relative to the given
	column) that is not a blank cell. This function is
	usally used to determine how cell overlapping is 
	performed.
Params
	col - [in] column number to start the search from
		  [out] column number of the previous non-blank
		  column
	row - row number to perform the search on
Return
	UG_SUCCESS  - success
	UG_ERROR	- failure	
****************************************************/
int CUGMem::GetPrevNonBlankCol(int *col,long row)
{
	//move to the row if not already there
	if(m_currentRow != row)
	{
		if(GotoRow(row) != UG_SUCCESS) //no cells on the specified row
		{
			return UG_ERROR;
		}
	}
	
	//move to the col if not already there
	if(m_currentCol != *col)
	{
		//if the specified col was not found then 1 is returned by
		//GotoCol, but it will find the closest col
		if(GotoCol(*col) != UG_SUCCESS)
		{	
			//if the first cell found is blank then there is no prev
			if(m_colInfo == NULL)
			{
				return UG_ERROR;
			}
			if(m_colInfo->cell == NULL)
			{
				return UG_ERROR;
			}
		}
	}

	//if there is no col information for the row then there is no prev col
	if(m_colInfo == NULL)
		return UG_ERROR;
	
	// if the specified cell (or a prev cell in the same row was found 
	// then find the prev non-blank cell
	UGMemCI * tempCI	= m_colInfo;
	int tempCol			= m_currentCol;

	//main prev search loop
	while(tempCol >= 0)
	{	
		if(tempCI->cell != NULL && *col != tempCol){
			if(tempCI->cell->GetTextLength() > 0){
				*col = tempCol;
				return UG_SUCCESS;
			}		
		}
		tempCI = tempCI->prev;
		tempCol --;
	}

	return UG_ERROR;
}

/***************************************************
GetCol - Private
	Moves the internal pointers to the specified
	column, and returns a pointer to the column object
Params
	ri - pointer to the current row information structure
	col - column to return the column information for
  return
	pointer to the column information structure, or
	NULL upon error
****************************************************/
UGMemCI * CUGMem::GetCol(UGMemRI * ri,int col)
{
	if(ri == NULL)
		return NULL;
	
	int count		= 0;
	BOOL isNULL		= FALSE;
	UGMemCI * ci	= ri->col;
	
	if(ci == NULL)
		return NULL;

	while(count < col)
	{
		if(ci->next != NULL)
		{
			ci = ci->next;
			count++;
		}
		else
		{
			isNULL = TRUE;
			break;
		}
	}
	while(count > col)
	{
		if(ci->prev != NULL)
		{
			ci = ci->prev;
			count--;
		}
		else
		{
			isNULL = TRUE;
			break;
		}
	}
	
	if(isNULL)
		return NULL;

	return ci;
}
