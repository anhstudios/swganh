/*************************************************************************
				Class Declaration : CUGTopHdg
**************************************************************************
	Source file : UGTopHdg.cpp
	Header file : UGTopHdg.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The top heading (CUGTopHdg) object/window
		is responsible to draw cells and handle
		user's actions on the column heading.

	Keay features:
		- This class provides ability to resize
		  column width with the mouse.
		- as well as the height of the entire
		  top heading and rows it contains
		- mouse and keyboard messages are forwarded
		  to the CUGCtrl class as notifications.
		  OnTH_...

*************************************************************************/
#ifndef _UGTopHdg_H_
#define _UGTopHdg_H_

class UG_CLASS_DECL CUGTopHdg : public CWnd
{
// Construction
public:
	CUGTopHdg();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGTopHdg)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGTopHdg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGTopHdg)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ToolTipNeedText( UINT id, NMHDR* pTTTStruct, LRESULT* pResult );
	virtual int OnToolHitTest( CPoint point, TOOLINFO *pTI ) const;

protected:
	
	friend CUGCtrl;
	CUGCtrl *		m_ctrl;
	CUGGridInfo *	m_GI;		//pointer to the grid information

	CUGCell			m_cell;		//general purpose cell class


	BOOL			m_isSizing;			//sizing flag
	BOOL			m_canSize;			//sizing flag
	BOOL			m_colOrRowSizing;	// 0-col 1-row
	int				m_sizingColRow;		//column/row being sized
	int				m_sizingStartSize;	//original size
	int				m_sizingStartPos;	//original start pos
	int				m_sizingStartHeight;//original top heading total height

	RECT			m_focusRect;		//focus rect for column sizing option

	CUGDrawHint		m_drawHint;		//cell drawing hints

	int				m_swapStartCol;
	int				m_swapEndCol;

	int GetCellRect(int *col,long *row,RECT *rect);
	int GetCellRect(int col,long row,RECT *rect);
	int GetCellFromPoint(CPoint *point,int *col,int *row,RECT *rect);
	int GetJoinRange(int *col,int *row,int *endCol,int *endRow);

	void DrawCellsIntern(CDC *dc);

	void CheckForUserResize(CPoint *point);

public:

	int GetTHRowHeight(int row);

	void Update();
	void Moved();
};

#endif // _UGTopHdg_H_