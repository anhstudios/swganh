/*************************************************************************
				Class Declaration : CUGMultiSelect
**************************************************************************
	Source file : UGMultiS.cpp
	Header file : UGMultiS.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This class maintains the multiple selection
		list for the grid.
	Details
		The CUGMultiSelect is responsible for figuring
		out what gets selected and what does not.  It
		can do this because the Mouse and Keyboard
		messages are passed here by the control class.

		It also has responsibilities to return information
		on selected cells/blocks, to the control class.
*************************************************************************/
#ifndef _UGMultiS_H_
#define _UGMultiS_H_

typedef struct UG_MSListTag
{
	int		startCol,endCol;
	long	startRow,endRow;
	int		loCol,hiCol;
	long	loRow,hiRow;
	int		selected;
	UG_MSListTag * next;
}UG_MSList;

#define UG_MULTISELECT_OFF	0
#define UG_MULTISELECT_CELL	1
#define UG_MULTISELECT_ROW	2
#define UG_MULTISELECT_HDGS	4
#define UG_MULTISELECT_DONTMOVE	8
#define UG_MULTISELECT_NODESELECT	16

class UG_CLASS_DECL CUGMultiSelect
{
protected:
	UG_MSList *	m_list;		//list of selected regions
	int			m_startCol;	//start region co-ords
	long		m_startRow;
	int			m_endCol;	//end region co-ords
	long		m_endRow;
	int			m_blockInProgress;
	UG_MSList *	m_currentItem;
	int			m_mode;	//selection mode, see UG_MULTISELECT_[...]
	int			m_numberBlocks;
	BOOL		m_blockJustStarted;

	//enum info
	int			m_enumStartCol;
	long		m_enumStartRow;
	int			m_enumEndCol;
	long		m_enumEndRow;
	int			m_enumCol;
	long		m_enumRow;
	int			m_enumInProgress;

	int			m_enumBlockNumber;

	friend		CUGCtrl;
	CUGCtrl *	m_ctrl;
	CUGGridInfo	*m_GI;

	int			m_lastCol;
	long		m_lastRow;

	int			m_origCol;
	int			m_origRow;

public:
	CUGMultiSelect();
	virtual ~CUGMultiSelect();

	virtual int ClearAll();

	virtual int StartBlock(int col,long row);
	virtual int EndBlock(int col,long row);

	virtual void OnLClick(int col,long row, UINT nFlags);
	virtual void OnRClick(int col,long row, UINT nFlags);
	virtual void OnKeyMove(int col,long row);
	virtual void OnMouseMove(int col,long row, UINT nFlags);
	
	virtual int ToggleCell(int col,long row);

	virtual int GetOrigCell(int *col,long *row);
	virtual int GetCurrentBlock(int *startCol,long *startRow,int *endCol,long *endRow);
	virtual int GetTotalRange(int *startCol,long *startRow,int *endCol,long *endRow);

	virtual int GetNumberBlocks();

	virtual int IsSelected(int col,long row,int *block = NULL);
	virtual int IsCellInColSelected(int col);
	virtual int IsCellInRowSelected(long row);

	virtual int SelectMode(int mode);
	virtual int GetSelectMode();

	virtual int EnumFirstSelected(int *col,long *row);
	virtual int EnumNextSelected(int *col,long *row);

	virtual int EnumFirstBlock(int *startCol,long *startRow,int *endCol,long *endRow);
	virtual int EnumNextBlock(int *startCol,long *startRow,int *endCol,long *endRow);

	void AddTotalRangeToDrawHints(CUGDrawHint * hint);
};

#endif // _UGMultiS_H_