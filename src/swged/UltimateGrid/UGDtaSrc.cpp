/*************************************************************************
				Class Implementation : CUGDataSource
**************************************************************************
	Source file : UGDtaSrc.cpp
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
CUGDataSource::CUGDataSource()
{
	m_ctrl = NULL;
	m_ID = -1;
}

CUGDataSource::~CUGDataSource()
{
}

/***************************************************
SetID
	Called by the framework to inform the data source
	which index value it is assigned.  This same index
	value is also returned by the AddDataSource function.
Params:
	ID	- index value assigned to the datasource
Return:
	<none>
***************************************************/
void CUGDataSource::SetID(long ID)
{
	m_ID = ID;
}

/***************************************************
GetID
	Is used to determine which index value is assigned
	to given data source class.
Params:
	<none>
Return:
	long	- the index assigned.
***************************************************/
long CUGDataSource::GetID()
{
	return m_ID;
}

/////////////////////////////////////////////////////////////////////////////
//	Virtual Functions

/***************************************************
Open
	A virtual function that provides standard interface
	for openning of the data source.  It is most often
	used to open database files, connections, etc.
Params:
	name
	option
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::Open(LPCTSTR name,LPCTSTR option)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(option);
	return UG_NA;
}

/***************************************************
IsOpen
	virtual function is most commonly used by datasources
	that bind to a database or some form of an external data.
	It is used to provide feedback to the developer who requires
	to know if a connection to the database is currently open
	or closed.

	This virtual function is not applicable to memory based
	datasources (ie. grid's default CUGMem).
Params:
	<none>
Return:
	FALSE if the datasource is closed and TRUE if it is open.
***************************************************/
BOOL CUGDataSource::IsOpen()
{
	return FALSE;
}

/***************************************************
SetPassword
	A virtual function that provides standard interface
	to set user name and password used to open the data
	source.
Params:
	user	- user name to use
	pass	- password
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SetPassword(LPCTSTR user,LPCTSTR pass)
{
	UNREFERENCED_PARAMETER(user);
	UNREFERENCED_PARAMETER(pass);
	return UG_NA;
}

/***************************************************
Close
	A virtual function that provides standard interface
	to close the data source file or connection.  
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::Close()
{
	return UG_NA;
}

/***************************************************
Save
	A virtual function that provides standard interface
	that allows the data source to be saved to file.
	This function is mostly needed with datasources that
	either do not update the data source as user makes
	changes (just like Excel), or allow for the gird's
	bound data to be saved to a file.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::Save()
{
	return UG_NA;
}

/***************************************************
SaveAs
	A virtual function that provides standard interface
	that allows the data source to be saved to file.
	This function is mostly needed with datasources that
	either do not update the data source as user makes
	changes (just like Excel), or allow for the gird's
	bound data to be saved to a file.
Params:
	name	- file or connection name
	option	- save options
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SaveAs(LPCTSTR name,LPCTSTR option)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(option);
	return UG_NA;
}

/***************************************************
GetNumRows
	A virtual function that provides standard interface
	for the grid to find out how many rows are in the
	data source.  If the data source is not able to 
	determine how many rows there are, than it should 
	only return a value greater than zero defining how
	many rows it is aware of.  The grid then will use
	the OnHitBottom notification to check if there are
	additional rows.
Params:
	<none>
Return:
	UG_NA		not available
	long		number of rows
****************************************************/
long CUGDataSource::GetNumRows()
{
	return UG_NA;
}

/***************************************************
GetNumCols
	A virtual function that provides standard interface
	for the grid to find out how many columns are in the
	data source.
Params:
	<none>
Return:
	UG_NA		not available
	long		number of cols
****************************************************/
int CUGDataSource::GetNumCols()
{
	return UG_NA;
}

/***************************************************
GetColName
	A virtual function that provides standard interface
	to provide the grid with the name of a column.
Params:
	col		- column number for which to return name
	string	- pointer to a string which should be populated
			  with the column name.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetColName(int col,CString * string)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*string);
	return UG_NA;
}

/***************************************************
GetColFromName
	A virtual function that provides standard interface
	to return a column number based on the column name.
Params:
	name	- name of the column to look for
	col		- ponter to an integer value representing
			  column location.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetColFromName(LPCTSTR name,int *col)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(*col);
	return UG_NA;
}

/***************************************************
GetColType
	A virtual function that provides standard interface
	to return the default data type set to a column.
Params:
	col		- column of interest
	type	- ponter to an integer that stores information
			  about the data type.  Possible data types can be:
				UGCELLDATA_STRING	(1)	string
				UGCELLDATA_NUMBER	(2)	number
				UGCELLDATA_BOOL		(3)	booliean
				UGCELLDATA_TIME		(4)	date/time
				UGCELLDATA_CURRENCY	(5)	currency
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetColType(int col,int *type)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*type);
	return UG_NA;
}

/***************************************************
AppendRow
	A virtual function that provides standard interface
	to append a new row at the end of the current data
	in the data source.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::AppendRow()
{
	return UG_NA;
}

/***************************************************
AppendRow
	This version of the AppendRow function allows
	to append new row with pre-populated cell objects.
	This is mostly used when adding new records to
	tables that require for some entries to have
	a value (ie. secondary keys, IDs, etc).
Params:
	cellList	- pointer to array of cell objects
	numCells	- integer value indicating number of
				  elements in the array.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::AppendRow(CUGCell **cellList,int numCells)
{
	UNREFERENCED_PARAMETER(**cellList);
	UNREFERENCED_PARAMETER(numCells);
	return UG_NA;
}

/***************************************************
InsertRow
	A virtual function that provides standard interface
	to inserts a row at specified location in the
	current data.
Params:
	row			- position at which to insert new row
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::InsertRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return UG_NA;
}

/***************************************************
AppendCol
	A virtual function that provides standard interface
	to append a new column at the end of the current data
	in the data source.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::AppendCol()
{
	return UG_NA;
}

/***************************************************
InsertCol
	A virtual function that provides standard interface
	to inserts a column at specified location in the
	current data.
Params:
	col			- column at which to insert new column
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::InsertCol(int col)
{
	UNREFERENCED_PARAMETER(col);
	return UG_NA;
}

/***************************************************
DeleteRow
	A virtual function that provides standard interface
	to delete specified row from the data source.
Params:
	row			- indicates the row number to delete
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::DeleteRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return UG_NA;
}

/***************************************************
DeleteCol
	A virtual function that provides standard interface
	to delete specified column from the data source.
Params:
	col			- indicates the column number to delete
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::DeleteCol(int col)
{
	UNREFERENCED_PARAMETER(col);
	return UG_NA;
}

/***************************************************
Empty 
	A virtual function that provides standard interface
	to delete everything that is storred in the data source.
	This function should be accompanied by functions that
	set number of columnd and rows to zero.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::Empty()
{
	return UG_NA;
}

/***************************************************
Delete 
	A virtual function that provides standard interface
	to delete a single cell from the data source.
	Deleting a single cell from the data source usually
	means that cell's value should be cleared (deleted).
Params:
	col, row	- coordinates of the cell to delete.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::DeleteCell(int col,long row)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return UG_NA;
}

/***************************************************
GetCell
	A virtual function that provides standard way
	for the grid to populate a cell object.  This
	function is called as a result of the 
	CUGCtrl::GetCell being called.
Params:
	col, row	- coordinates of the cell to retrieve
				  information on.
	cell		- pointer to CUGCell object to populate
				  with the information found.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
	return UG_NA;
}

/***************************************************
SetCell
	This virtual function is called as a result of
	a call to CUGCtrl::SetCell in attempts to set
	new value to a cell in the data source.
Params:
	col, row	- coordinates of the cell to set new
				  information to.
	cell		- pointer to CUGCell object to that
				  contains new cell's value.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
	return UG_NA;
}

/***************************************************
FindFirst
	A virtual function that provides standard interface
	to find first occurence of specified value (string)
	in a column based on the flags passed in.
Params:
	string		- string to look for
	col			- pointer to integer value identifying
				  which column contains the found value
	row			- pointer to a long integer value that
				  identifys the row which contains the
				  value found
	flags		- find flags
					(1) UG_FIND_PARTIAL
					(2) UG_FIND_CASEINSENSITIVE
					(4) UG_FIND_UP
					(8) UG_FIND_ALLCOLUMNS
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::FindFirst(CString *string,int *col,long *row,long flags)
{
	UNREFERENCED_PARAMETER(*string);
	UNREFERENCED_PARAMETER(*col);
	UNREFERENCED_PARAMETER(*row);
	UNREFERENCED_PARAMETER(flags);
	return UG_NA;
}

/***************************************************
FindFirst
	A virtual function that provides standard interface
	to find next occurence of specified value (string)
	in a column based on the flags passed in.
Params:
	string		- string to look for
	col			- pointer to integer value identifying
				  which column contains the found value
	row			- pointer to a long integer value that
				  identifys the row which contains the
				  value found
	flags		- find flags
					(1) UG_FIND_PARTIAL
					(2) UG_FIND_CASEINSENSITIVE
					(4) UG_FIND_UP
					(8) UG_FIND_ALLCOLUMNS
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::FindNext(CString *string,int *col,long *row,int flags)
{
	UNREFERENCED_PARAMETER(*string);
	UNREFERENCED_PARAMETER(*col);
	UNREFERENCED_PARAMETER(*row);
	UNREFERENCED_PARAMETER(flags);
	return UG_NA;
}

/***************************************************
SortBy
	A virtual function that provides standard interface
	to sort data in the data source.  This function is
	never called by the grid, but it can be called
	directly.
Params:
	col			- the column to sort
	flags		- sort flag identifying the sort direction
					UG_SORT_ASCENDING
					UG_SORT_DESCENDING
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SortBy(int col,int flags)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(flags);
	return UG_NA;
}

/***************************************************
SortBy
	A virtual function that provides standard interface
	to sort data in the data source.  This function is
	called when user calls CUGCtrl::SortBy
Params:
	cols		- array of columns to be sorted, in the
				  sort order.
	num			- number of elements in the array
	flags		- sort flag identifying the sort direction
					UG_SORT_ASCENDING
					UG_SORT_DESCENDING
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SortBy(int *cols,int num,int flags)
{
	UNREFERENCED_PARAMETER(*cols);
	UNREFERENCED_PARAMETER(num);
	UNREFERENCED_PARAMETER(flags);
	return UG_NA;
}

/***************************************************
SetOption
	Datasource dependant function. Used to set data source
	specific information and modes of operation
Params:
	option		- integer identifying the option to set
	param1		- option depanded parameter
	param2		- option depanded parameter
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::SetOption(int option,long param1,long param2)
{
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	return UG_NA;
}

/***************************************************
GetOption
	Datasource dependant function. Used to get data source
	specific information and modes of operation
Params:
	option		- integer identifying the option to set
	param1		- option depanded parameter
	param2		- option depanded parameter
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetOption(int option,long& param1,long& param2)
{
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	return UG_NA;
}

/****************************************************
GetPrevNonBlankCol
	A virtual function that provides standard interface
	for the cell type to complete the cell over lap
	functionality.
Params:
	col			- pointer to an integer value used
				  identify which column the cell type
				  is working with, and to return column 
				  number that contains a value. 
	row			- row the cell type is working with.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::GetPrevNonBlankCol(int *col,long row)
{
	UNREFERENCED_PARAMETER(*col);
	UNREFERENCED_PARAMETER(row);
	return UG_NA;
}

/****************************************************
StartTransaction
	A virtual function that provides standard method
	to start a transaction.  Very important when
	working with databases.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::StartTransaction()
{
	return UG_NA;
}

/****************************************************
CancelTransaction
	A virtual function that provides standard method
	to cancel (undo) changes that were made after
	last call to the StartTransaction.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::CancelTransaction()
{
	return UG_NA;
}

/****************************************************
FinishTransaction
	A virtual function that provides standard method
	to make permanent the changes that were made
	after the last call to the StartTransaction.
Params:
	<none>
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::FinishTransaction()
{
	return UG_NA;
}

/***************************************************
OnHitBottom
	This notification allows for dynamic row loading, 
	it will be called when the grid's drawing function 
	has hit the last row.  It allows the grid to ask 
	the datasource/developer if there are additional 
	rows to be displayed.
Params:
	numrows		- known number of rows in the grid
	rowspast	- number of extra rows that the grid 
				  is looking for in the datasource
	rowsfound	- number of rows actually found, 
				  usually equal to rowspast or zero.
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::OnHitBottom(long numrows,long rowspast,long *rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(*rowsfound);
	return UG_NA;
}

/***************************************************
OnHitTop
	Is called when the user has scrolled all the way
	to the top of the grid.
Params:
	numrows		- known number of rows in the grid
	rowspast	- number of extra rows that the grid
				  is looking for in the datasource
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
int CUGDataSource::OnHitTop(long numrows,long rowspast,long *rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(*rowsfound);
	return UG_NA;
}

/***************************************************
OnRowChange
	Sent whenever the current row changes
Params:
	oldrow		- row that is loosing the locus
	newrow		- row that user moved into
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
void CUGDataSource::OnRowChange(long oldRow,long newRow)
{
	UNREFERENCED_PARAMETER(oldRow);
	UNREFERENCED_PARAMETER(newRow);
}

/***************************************************
OnColChange
	Sent whenever the current column changes
Params:
	oldcol		- column that is loosing the focus
	newcol		- column that the user move into
Return:
	UG_NA		not available
	UG_SUCCESS	success
	1...		error codes (data source dependant)
****************************************************/
void CUGDataSource::OnColChange(int oldCol,int newCol)
{
	UNREFERENCED_PARAMETER(oldCol);
	UNREFERENCED_PARAMETER(newCol);
}

/***************************************************
OnCanMove
	is sent when a cell change action was instigated
	( user clicked on another cell, used keyboard arrows,
	or Goto[...] function was called ).
Params:
	oldcol, oldrow - 
	newcol, newrow - cell that is gaining focus
Return:
	TRUE - to allow the move
	FALSE - to prevent new cell from gaining focus
****************************************************/
int CUGDataSource::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/***************************************************
OnEditStart
	This message is sent whenever the grid is ready to
	start editing a cell
Params:
	col, row - location of the cell that edit was requested over
	edit -	pointer to a pointer to the edit control,
			allows for swap of edit control if edit 
			control is swapped permanently (for the
			whole grid) is it better to use 'SetNewEditClass'
			function.
Return:
	TRUE - to allow the edit to start
	FALSE - to prevent the edit from starting
****************************************************/
int CUGDataSource::OnEditStart(int col, long row,CWnd **edit)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(**edit);
	return TRUE;
}
/***************************************************
OnEditVerify
	This notification is sent every time the user hits
	a key while in edit mode.  It is mostly used to create
	custom behavior of the edit contol, because it is
	so eazy to allow or disallow keys hit.
Params:
	col, row	- location of the edit cell
	edit		-	pointer to the edit control
	vcKey		- virtual key code of the pressed key
Return:
	TRUE - to accept pressed key
	FALSE - to do not accept the key
****************************************************/
int CUGDataSource::OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(*vcKey);
	return TRUE;
}
/***************************************************
OnEditFinish
	This notification is sent when the edit is being finised
Params:
	col, row	- coordinates of the edit cell
	edit		- pointer to the edit control
	string		- actual string that user typed in
	cancelFlag	- indicates if the edit is being cancelled
Return:
	TRUE - to allow the edit it proceede
	FALSE - to force the user back to editing of that same cell
****************************************************/
int CUGDataSource::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(string);
	UNREFERENCED_PARAMETER(cancelFlag);
	return TRUE;
}
/***************************************************
OnEditContinue
	This notification is called when the user pressed 
	'tab' or 'enter' keys Here you have a chance
	to modify the destination cell
Params:
	oldcol, oldrow - edit cell that is loosing edit focus
	newcol, newrow - cell that the edit is going into, 
					 by changing their values you are able
					 to change where to edit next
Return:
	TRUE - allow the edit to continue
	FALSE - to prevent the move, the edit will be stopped
****************************************************/
int CUGDataSource::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(*newcol);
	UNREFERENCED_PARAMETER(*newrow);
	return TRUE;
}
