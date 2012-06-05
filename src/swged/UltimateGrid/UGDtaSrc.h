/*************************************************************************
				Class Declaration : CUGDataSource
**************************************************************************
	Source file : UGDtaSrc.cpp
	Header file : UGDtaSrc.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGDataSource class is used by the grid
		as standard interface between the grid
		and its data.  The Ultimate Grid relies
		on CUGDataSource derived class to provide
		it with all of the information that needs
		to be displayed.

		Datasources can be practically anything
		i.e.	arrays
				linked lists
				databases
				flat files
				real-time feeds (sensors)
				calculations

	Details
		This is a base class which all other datasources
		must be derived from. By defining a standard
		interface to the data, an abstract layer is
		created which allows the uderlying data to
		come from any source, plus allows the datasource
		to be changed without any code re-write.

		At the minimum only ONE virtual function must
		be overwitten it is the GetCell. GetCell is
		called by the grid when it needs information
		about a particular cell.

		Even though the grid generally works on a cell by
		cell basis, many datasource (such as databases) 
		tend to work on a row by row basis. To allow data
		to be written to the datasource in this manner
		transaction writing can be used within a datasource
		by overwritting the transaction functions.

		If a derived datasource cannot return the number
		of rows that is contains, then overwrite
		the OnHitBottom virtual function. This allows for
		the grid to ask the datasource for new rows on the fly.

		Stanard return values from a datasource are
			UG_NA		- not implemented (-1)
			UG_SUCCESS	- success (0)
			1 and up	- error codes
*************************************************************************/
#ifndef _UGDtaSrc_H_
#define _UGDtaSrc_H_

//defines for the IsSupported function
#define UGDS_SUPPORT_INSERTROW		BIT0
#define UGDS_SUPPORT_APPENDROW		BIT1
#define UGDS_SUPPORT_INSERTCOL		BIT2
#define UGDS_SUPPORT_APPENDCOL		BIT3
#define UGDS_SUPPORT_DELETEROW		BIT4
#define UGDS_SUPPORT_DELETECOL		BIT5
#define UGDS_SUPPORT_EMPTY			BIT6
#define UGDS_SUPPORT_CLOSE			BIT7
#define UGDS_SUPPORT_SAVE			BIT8
#define UGDS_SUPPORT_OPEN			BIT9
#define UGDS_SUPPORT_SETPASSWORD	BIT10
#define UGDS_SUPPORT_FIND			BIT11
#define UGDS_SUPPORT_SORT			BIT12
#define UGDS_SUPPORT_GETNUMROWS		BIT13
#define UGDS_SUPPORT_GETNUMCOLS		BIT14
#define UGDS_SUPPORT_GETCOLNAME		BIT15
#define UGDS_SUPPORT_GETCOLTYPE		BIT16
#define UGDS_SUPPORT_SETCELL		BIT17
#define UGDS_SUPPORT_GETCELL		BIT18
#define UGDS_SUPPORT_TRANSACTIONS	BIT19
#define UGDS_SUPPORT_HITBOTTON		BIT20
#define UGDS_SUPPORT_HITTOP			BIT21
#define UGDS_SUPPORT_GETPREVCOL		BIT22

class UG_CLASS_DECL CUGDataSource
{
protected:
	friend CUGCtrl;
	CUGCtrl * m_ctrl;  //pointer to the main class
	
	long m_ID;

public:
	CUGDataSource();
	virtual ~CUGDataSource();

	void SetID(long ID);
	long GetID();
	
	//opening and closing
	virtual int Open(LPCTSTR name,LPCTSTR option);
	virtual BOOL IsOpen();
	virtual int SetPassword(LPCTSTR user,LPCTSTR pass);
	
	virtual int Close();
	virtual int Save();
	virtual int SaveAs(LPCTSTR name,LPCTSTR option);

	//row and col info
	virtual long GetNumRows();
	
	virtual int GetNumCols();
	virtual int GetColName(int col,CString * string);
	virtual int GetColType(int col,int *type);	//0-string 1-bool 2-short 3-long 4-float 
												//5-double 6-currency 7-data 8-time
												//8-memo 9-blob 10-ole
	virtual int GetColFromName(LPCTSTR name,int *col);
	
	//add-update-clear
	virtual int AppendRow();
	virtual int AppendRow(CUGCell **cellList,int numCells);
	virtual int InsertRow(long row);
	virtual int AppendCol();
	virtual int InsertCol(int col);
	virtual int DeleteRow(long row);
	virtual int DeleteCol(int col);
	virtual int DeleteCell(int col,long row);
	virtual int Empty();

	//cell info
	virtual int	GetCell(int col,long row,CUGCell *cell);
	virtual int	SetCell(int col,long row,CUGCell *cell);

	//finding sorting
	virtual int FindFirst(CString *string,int *col,long *row,long flags);
	virtual int FindNext(CString *string,int *col,long *row,int flags);

	virtual int SortBy(int col,int flags);
	virtual int SortBy(int *cols,int num,int flags);

	//options
	virtual int SetOption(int option,long param1,long param2);
	virtual int GetOption(int option,long& param1,long& param2);

	//movement 
	virtual int OnHitTop(long numrows,long rowspast,long *rowsfound);
	virtual int OnHitBottom(long numrows,long rowspast,long *rowsfound);

	//grid movement
	virtual void OnRowChange(long oldRow,long newRow);
	virtual void OnColChange(int oldCol,int newCol);
	virtual int OnCanMove(int oldcol,long oldrow,int newcol,long newrow);

	//used to check to see if the data source supports a standard function
	BOOL IsFunctionSupported(long type);

	//transactions
	virtual int StartTransaction();
	virtual int CancelTransaction();
	virtual int FinishTransaction();

	// editing notifications...
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	//other functions
	virtual int GetPrevNonBlankCol(int *col,long row);
};

#endif // _UGDtaSrc_H_