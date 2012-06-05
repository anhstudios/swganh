/*************************************************************************
				Class Declaration : CUGMem
**************************************************************************
	Source file : UGMemMan.cpp
	Header file : UGMemMan.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This class is derived from the grid's 
		base datasource class (CUGDataSource). This
		class is the default datasource and it 
		allows information to be pre-loaded into
		the grid. By default an Instance of this 
		datasource is created for each sheet in 
		the grid. Since this is the default data
		source for a grid, it makes it easy to
		create and pre-load a grid control with
		data.

	Details
		-This class only stores cells that have
		 information set in them, which reduces
		 the amount of memory required for large
		 grids.
		-This class works as a two dimensional 
		 linked list, with its optimizations being
		 row oriented.
		-Support for sorting, finding, inserting
		 and deleting are fully supported.
		-cells are stored in native CUGCell format
		 internally.
		-since this datasource is automatically created
		 for each grid, it is always given an index
		 number of 0 (see CUGCtrl::SetDefDataSource).
		-Standard return values for functions are: 
			UG_SUCCESS	- success (0)
			UG_NA		- not available (-1)
			UG_ERROR	- basic error code (1)
			2 and up	- extended error codes
*************************************************************************/
#ifndef _UGMemMan_H_
#define _UGMemMan_H_

#pragma warning (disable: 4786)

//linked list structures that make up the row/col matrix
typedef struct UGMemCITag{
	CUGCell *		cell;
	UGMemCITag *	next;
	UGMemCITag *	prev;
}UGMemCI;

typedef struct UGMemRITag{
	UGMemCI	*		col;	
	UGMemRITag *	next;
	UGMemRITag *	prev;
	CUGCell **		cellLookup;
}UGMemRI;



/************************************************
CUGMem - memory based data source
*************************************************/
class UG_CLASS_DECL CUGMem: public CUGDataSource
{
private:
	
	long		m_currentRow;	//current row
	UGMemRI	*	m_rowInfo;		//current row information pointer
	int			m_currentCol;	//current column
	UGMemCI	*	m_colInfo;		//current column information pointer

	int			m_findCol;	//current col for the findnext member
	long		m_findRow;	//current row for the findnext member

	int GotoRow(long row);	//moves the internal memory pointers to the row
	int GotoCol(int col);	//moves the internal memory pointers to the col
	int PrevRow();			//moves the internal memory pointers to the prev row
	int NextRow();			//moves the internal memory pointers to the next row
	int PrevCol();			//moves the internal memory pointers to the prev col
	int NextCol();			//moves the internal memory pointers to the next col

	UGMemCI * GetCol(UGMemRI * ri,int col);

public:

	CUGMem();
	virtual ~CUGMem();

	//row and col info
	virtual long GetNumRows();

	//add-update-clear
	virtual int AppendRow();
	virtual int InsertRow(long row);
	virtual int AppendCol();
	virtual int InsertCol(int col);
	virtual int DeleteRow(long row);
	virtual int DeleteCol(int col);
	virtual int Empty();
	virtual int DeleteCell(int col,long row);

	//cell info
	virtual int	GetCell(int col,long row,CUGCell *cell);
	virtual int	SetCell(int col,long row,CUGCell *cell);

	//finding sorting
	virtual int FindFirst(CString *string,int *col,long *row,long flags);
	virtual int FindNext(CString *string,int *col,long *row,int flags);
	virtual int SortBy(int col,int flags);
	virtual int SortBy(int *cols,int num,int flags);
	int SortBy(long startRow, long endRow, int *cols,int numCols,int flags);

	virtual int GetPrevNonBlankCol(int *col,long row);
};

#endif // _UGMemMan_H_