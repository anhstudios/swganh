/*************************************************************************
				Class Declaration : CUGGrid
**************************************************************************
	Source file : UGGrid.cpp
	Header file : UGGrid.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGGrid takes care of the main grid area,
		this is where all of the cells are drawn and
		most user interaction takes place.
	Details
		- This class handles drawing of all cells,
		  excluding cells on headings.
		- All of the mouse and keyboard events
		  are routed to the CUGCtrl derived class
		  in form of virtual functions (notifications).
*************************************************************************/
#ifndef _UGGrid_H_
#define _UGGrid_H_

#ifndef WM_MOUSEWHEEL
#define ON_WM_MOUSEWHEEL
#endif

#pragma warning (disable: 4786)

class UG_CLASS_DECL CUGGrid : public CWnd
{
// Construction
public:
	CUGGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGGrid)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGGrid)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	
	#ifdef WM_MOUSEWHEEL
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	#endif
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ToolTipNeedText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

	void DrawCellsIntern(CDC *dc,CDC *db_dc);

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// This protected member is used to prevent the grid from un-matched
	// mouse button events.  This member was added in attempt to work around
	// a Windows bug, when user double clicks within a window 4 messages are
	// sent: WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, WM_LBUTTONUP
	// On many occasions popup windows are closed on the double click event
	// causing the last mouse up message be sent to any window under current
	// mouse cursor.
	BOOL m_bHandledMouseDown;

public:
	
	//internal data
	CUGCtrl		*	m_ctrl;			//pointer to the main class
	CUGGridInfo *	m_GI;			//pointer to the grid information

	CUGDrawHint		m_drawHint;		//cell drawing hints

	CBitmap *		m_bitmap;		//double buffering
	int				m_doubleBufferMode;

	CUGCell			m_cell;			//general purpose cell object

	long			m_keyRepeatCount; //key ballistics repeat counter

	int				m_hasFocus;

	BOOL			m_cellTypeCapture;

	BOOL			m_tempDisableFocusRect;

public:

	void Update();
	void Moved();

	void PaintDrawHintsNow(LPCRECT invalidateRect= NULL);

	void RedrawCell(int col,long row);
	void RedrawRow(long row);
	void RedrawCol(int col);
	void RedrawRange(int startCol,long startRow,int endCol,long endRow);
	void TempDisableFocusRect();

	int SetDoubleBufferMode(int mode);
};

#endif // _UGGrid_H_