/*************************************************************************
				Class Declaration : CUGVScroll
**************************************************************************
	Source file : ugvscrol.cpp
	Header file : ugvscrol.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	This class is grid's vertical scrollbar.

*************************************************************************/
#ifndef _ugvscrol_H_
#define _ugvscrol_H_

class UG_CLASS_DECL CUGVScroll : public CScrollBar
{
// Construction
public:
	CUGVScroll();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGVScroll)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGVScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGVScroll)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	
	friend CUGCtrl;
	CUGCtrl *		m_ctrl;
	CUGGridInfo *	m_GI;			//pointer to the grid information


	double			m_multiRange;	//scroll bar multiplication factor
									//for setting the scroll range
	double			m_multiPos;		//multiplication factor for setting the 
									//top row during a thumb track
	long			m_lastMaxTopRow;//last max top row

	int				m_lastScrollMode;

	int				m_lastNumLockRow;

	long			m_trackRowPos;

public:

	//internal functions
	void Update();
	void Moved();
	void VScroll(UINT nSBCode, UINT nPos);

};

#endif // _ugvscrol_H_