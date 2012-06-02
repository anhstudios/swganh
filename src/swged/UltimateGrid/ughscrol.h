/*************************************************************************
				Class Declaration : CUGHScroll
**************************************************************************
	Source file : ughscrol.cpp
	Header file : ughscrol.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	This class is grid's horizontal scrollbar.

*************************************************************************/
#ifndef _ughscrol_H_
#define _ughscrol_H_

class UG_CLASS_DECL CUGHScroll : public CScrollBar
{
// Construction
public:
	CUGHScroll();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGHScroll)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGHScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGHScroll)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


protected:

	friend CUGCtrl;
	CUGCtrl		*	m_ctrl;		//pointer to the main class
	CUGGridInfo *	m_GI;			//pointer to the grid information

	int	m_lastMaxLeftCol;
	int m_lastNumLockCols;

	int m_trackColPos;

public:

	//internal functions
	void Update();
	void Moved();

	void HScroll(UINT nSBCode, UINT nPos);
};

#endif // _ughscrol_H_
