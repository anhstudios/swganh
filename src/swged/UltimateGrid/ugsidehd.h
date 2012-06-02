/*************************************************************************
				Class Declaration : CUGSideHdg
**************************************************************************
	Source file : ugsidehd.cpp
	Header file : ugsidehd.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The side heading (CUGSideHdg) object/window
		is responsible to draw cells and handle
		user's actions on the rows heading.

	Keay features:
		- This class provides ability to resize
		  rows height with the mouse.
		- as well as the width of the entire
		  side heading and columns it contains
		- mouse and keyboard messages are forwarded
		  to the CUGCtrl class as notifications.
		  OnSH_...

*************************************************************************/
#ifndef _ugsidehd_H_
#define _ugsidehd_H_

class UG_CLASS_DECL CUGSideHdg : public CWnd
{
// Construction
public:
	CUGSideHdg();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGSideHdg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGSideHdg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGSideHdg)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
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
	CUGGridInfo *	m_GI;			//pointer to the grid information

	CUGCell			m_cell;			//general purpose cell class

	CUGDrawHint		m_drawHint;		//cell drawing hints

	long			m_bottomRow;	//the row drawn at the bottom

	int				m_isSizing;			//sizing flag
	int				m_canSize;			//sizing flag
	int				m_colOrRowSizing;	// 0 - col 1- row
	long			m_sizingColRow;		// column or row number being sized
	int				m_sizingStartSize;	// original col/row width/height
	int				m_sizingStartPos;	// original starting mouse position
	int				m_sizingStartWidth;//original side heading total width
	int				m_sizingNumRowsDown; //number of rows from top when size started

	RECT			m_focusRect;		//focus rect for column sizing option

	int GetCellRect(int *col,long *row,RECT *rect);
	int GetCellRect(int col,long row,RECT *rect);
	int GetCellFromPoint(CPoint *point,int *col,long *row,RECT *rect);

	void DrawCellsIntern(CDC *dc);

public:

	int GetSHColWidth(int col);

	//internal functions
	void Update();
	void Moved();
};

#endif // _ugsidehd_H_