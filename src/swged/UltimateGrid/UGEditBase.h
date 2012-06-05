/*************************************************************************
				Class Declaration : CUGEditBase
**************************************************************************
	Source file : UGEditBase.cpp
	Header file : UGEditBase.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

    Purpose
		This class will automatically size the font of any cell using 
		this celltype so that all of the text will be visible.
*************************************************************************/

#ifndef _UGEditBase_H_
#define _UGEditBase_H_

#include "ugctrl.h"

//CUGEditBase Declaration
class UG_CLASS_DECL CUGEditBase : public CEdit
{
// Construction
public:
	CUGEditBase();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGEditBase)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// based on user action destination cell needs to be set 
	int m_continueCol;
	long   m_continueRow; 
	BOOL   m_cancel;
	BOOL   m_continueFlag;

	// member variable that keeps track of the last pressed key
	UINT m_lastKey;

	void UpdateCtrl();
	
	CBrush m_Brush;

public:

	// Grid will try to set this pointer to itself
	CUGCtrl * m_ctrl;    

	virtual ~CUGEditBase();

	UINT  GetLastKey();
	// Generated message map functions
protected:
	//{{AFX_MSG(CUGEditBase)
	    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnKillfocus();	
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //#ifndef _UGEditBase_H_
