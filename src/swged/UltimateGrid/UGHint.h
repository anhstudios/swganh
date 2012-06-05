/*************************************************************************
				Class Declaration : CUGHint
**************************************************************************
	Source file : UGHint.cpp
	Header file : UGHint.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Class 
		CUGHint
	Purpose
		The CUGHint class is used by the Ultimate
		Grid to display tool tips when user is
		using the scroll bars.

		The text displayed in the tool window is 
		set through OnHScrollHint and OnVScrollHint
		virtual functions.
*************************************************************************/
#ifndef _UGHint_H_
#define _UGHint_H_

class UG_CLASS_DECL CUGHint : public CWnd
{
// Construction
public:
	CUGHint();

// Attributes
public:

// Operations
public:

	BOOL Create(CWnd* pParentWnd, HBRUSH hbrBackground=NULL);
	// --- In  :	pParentWnd		-	pointer to parent window
	//				hbrBackground	-	brush to be used to fill background
	// --- Out : 
	// --- Returns:	TRUE if item tip window was successfully created, or FALSE 
	//				otherwise
	// --- Effect : creates item tip window

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGHint)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGHint();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGHint)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CUGCtrl *		m_ctrl;		//pointer to the main class
	CString			m_text;
	COLORREF		m_textColor;
	COLORREF		m_backColor;
	int				m_windowAlign;
	int				m_textAlign;
	CFont *			m_hFont;
	int				m_fontHeight;

	int	SetWindowAlign(int align);
	int SetTextAlign(int align);
	int SetTextColor(COLORREF color);
	int SetBackColor(COLORREF color);
	int SetFont(CFont *font);

	int SetText(LPCTSTR string,int update);
	int MoveHintWindow(int x,int y,int width);

	int Hide();
	int Show();

};

#endif // _UGHint_H_