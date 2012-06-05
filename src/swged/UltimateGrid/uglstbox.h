/*************************************************************************
				Class Declaration : CUGLstBox
**************************************************************************
	Source file : uglstbox.cpp
	Header file : uglstbox.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGLstBox class is a helper class for the
		droplist cell type.  This class is the drop list
		that is poped up and shown to the user.
		
		The Select function is the most important function
		in this class, this function makes sure that the
		user's selection is properly passed back to the
		gird control, and that the OnCellTypeNotify
		notification is properly informed of the user's
		action.
*************************************************************************/
#ifndef _uglstbox_H_
#define _uglstbox_H_


class CUGDropListType;

class UG_CLASS_DECL CUGLstBox : public CListBox
{
// Construction
public:
	CUGLstBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGLstBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGLstBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGLstBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	//pointer to the main class
	CUGCtrl *	m_ctrl;
	int			m_HasFocus;
	int		*	m_col;
	long	*   m_row;
	BOOL		m_selected;

	CUGDropListType* m_cellType;
	int			m_cellTypeId;

private:
	void Select();
};

#endif // _uglstbox_H_
