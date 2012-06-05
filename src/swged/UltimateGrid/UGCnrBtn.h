/*************************************************************************
				Class Declaration : CUGnrBtn
**************************************************************************
	Source file : UGCnrBtn.cpp
	Header file : UGCnrBtn.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Class 
		CUGnrBtn
	Purpose
		The Ultimate Grid draws the corner button
		(CUGnrBtn) object in the area that neither
		top heading nor the side heading should be,
		this is normally on the top left.

		This class is a special kind of a grid's
		headings, its size is controled by the height
		of the top heading and the width of the side
		heading.

		Cells displayed on the corner button have
		coordinates with negative values for both
		the column and row numbers.

*************************************************************************/
#ifndef _UGCnrBtn_H_
#define _UGCnrBtn_H_

class UG_CLASS_DECL CUGCnrBtn : public CWnd
{
// Construction
public:
	CUGCnrBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGCnrBtn)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGCnrBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGCnrBtn)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
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

public:


	// internal information
	friend CUGCtrl;
	CUGCtrl		*	m_ctrl;		//pointer to the main class
	CUGGridInfo *	m_GI;		//pointer to the grid information

	int				m_isSizing;		//sizing flag
	int				m_canSize;		//sizing flag
	int				m_sizingHeight;	//sizing flag
	int				m_sizingWidth;	//sizing flag
	
	//internal functions
	void Update();
	void Moved();
};

#endif // _UGCnrBtn_H_