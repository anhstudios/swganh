/*************************************************************************
				Class Declaration : CUGTab
**************************************************************************
	Source file : ugtab.cpp
	Header file : ugtab.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The tab control is used by the Ultimate Grid
		to provide ability to switch between grid
		sheets.  A notification is called everytime
		any tab is clicked on, this notification
		identifies which tab user clicked on and
		provides a convienient location to react
		to user's action.

		This control is located just left to the 
		horizontal scroll bar, where its size is
		dictated by the width of the scroll bar
		and grid's window.

*************************************************************************/
#ifndef _ugtab_H_
#define _ugtab_H_

#define UTABSCROLLID 100
#define UTMAXTABS	64

class UG_CLASS_DECL CUGTab : public CWnd
{
// Construction
public:
	CUGTab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGTab)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGTab();

	void ShowScrollbars(bool show) { if (m_scroll) m_scroll.ShowWindow((show) ? SW_SHOW : SW_HIDE); }

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGTab)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CScrollBar m_scroll;	
	CFont	m_defFont;
	CFont	*m_font;
	// ******************************
	CString		m_tabStrings[UTMAXTABS];
	int			m_tabWidths[UTMAXTABS];
	int			m_tabIDs[UTMAXTABS];
	COLORREF	m_tabTextColors[UTMAXTABS];
	COLORREF	m_tabBackColors[UTMAXTABS];
	COLORREF	m_tabTextHColors[UTMAXTABS];
	COLORREF	m_tabBackHColors[UTMAXTABS];
	// ******************************

	// Member vairables used to store state and limiting information
	int		m_tabCount;
	int		m_tabOffset;
	int		m_maxTabOffset;
	int		m_currentTab;
	int		m_scrollWidth;
	int		m_bestWidth;
	int		m_resizeReady;
	int		m_resizeInProgress;

	// Allow the CUGTab communicate with the CUGCtrl
	friend CUGCtrl;
	CUGCtrl* m_ctrl;
	CUGGridInfo *	m_GI;

	int GetTabItemWidth( CString string );
	int AdjustScrollBars();

public:
	// functions to add, insert and delete tabs
	int AddTab( CString string, long ID );
	int InsertTab( int pos, CString string, long ID );
	int DeleteTab( long ID );
	// functions used for user navigation
	int SetCurrentTab( long ID );
	int GetCurrentTab();
	// functions to set and retrieve information on the tabs
	long GetTabID( int nIndex );
	int GetTabIndex( long nID );
	int GetTabCount();
	int SetTabLabel( int nID, CString sLabel );
	int GetTabLabel( int nID, CString &sLabel );
	CString GetTabLabel( int nIndex );
	// functions to set tab's apperance
	int SetTabBackColor( long ID, COLORREF color );
	int SetTabTextColor( long ID, COLORREF color );
	int SetTabHBackColor( long ID, COLORREF color );
	int SetTabHTextColor( long ID, COLORREF color );
	int SetTabFont( CFont *font );
	// update functions
	virtual void OnTabSizing( int width );
	virtual void Update();
};

#endif // _ugtab_H_