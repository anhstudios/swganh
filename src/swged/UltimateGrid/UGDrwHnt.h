/*************************************************************************
				Class Declaration : CUGDrawHint
**************************************************************************
	Source file : UGDrwHnt.cpp
	Header file : UGDrwHnt.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This class is used internally by the grid
		to keep track of which cells need redrawing
		The grid draws its cells in an extremely
		optimized manner which gives it is great
		speed.

		This is the class which helps the optimization
		process by maintaining a list of cells that
		need to be redrawn.

		When the grid is doing the drawing it calls
		IsInvalid function to see if a cell needs
		to be redrawn.
************************************************/
#ifndef _UGDrwHnt_H_
#define _UGDrwHnt_H_

//drawing hint linked list structure
typedef struct UGDrwHintListTag
{
	int startCol;
	long startRow;
	int endCol;
	long endRow;
	
	UGDrwHintListTag * next;

}UGDrwHintList;

typedef struct UGDrwHintVListTag
{
	int Col;
	long Row;
	
	UGDrwHintVListTag * next;

}UGDrwHintVList;

//drawing hint class
class UG_CLASS_DECL CUGDrawHint
{
private:
	UGDrwHintList * m_List;
	UGDrwHintVList * m_VList;

	int  m_minCol,m_maxCol;
	long m_minRow,m_maxRow;

	int  m_vMinCol,m_vMaxCol;
	long m_vMinRow,m_vMaxRow;

public:
	CUGDrawHint();
	~CUGDrawHint();

	void AddHint(int col,long row);
	void AddHint(int startCol,long startRow,int endCol,long endRow);

	void AddToHint(int col,long row);
	void AddToHint(int startCol,long startRow,int endCol,long endRow);
	void ClearHints();

	BOOL IsInvalid(int col,long row);
	BOOL IsValid(int col,long row);
	int GetTotalRange(int *startCol,long *startRow,int *endCol,long *endRow);

	void SetAsValid(int col,long row);
	void SetAsValid(int startCol,long startRow,int endCol,long endRow);
};

#endif // _UGDrwHnt_H_