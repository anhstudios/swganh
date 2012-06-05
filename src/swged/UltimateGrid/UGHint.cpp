/*************************************************************************
				Class Implementation : CUGHint
**************************************************************************
	Source file : UGHint.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGHint::CUGHint()
{
}

CUGHint::~CUGHint()
{
}

/********************************************
	Message handlers
*********************************************/
BEGIN_MESSAGE_MAP(CUGHint, CWnd)
	//{{AFX_MSG_MAP(CUGHint)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
OnPaint
	The framework calls this member function when Windows
	or an application makes a request to repaint a portion
	of an application’s window.
	The CUGHint uses this event to draw the tool tip
	window.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGHint::OnPaint() 
{
	if ( m_ctrl->m_GI->m_paintMode == FALSE )
		return;

	CDC* dc = GetDC();

	if(m_hFont != NULL)
		dc->SelectObject(m_hFont);
	
	CRect rect;
	GetClientRect(rect);

	dc->SetTextColor(m_textColor);
	dc->SetBkColor(m_backColor);

	dc->SetBkMode(OPAQUE);
	dc->DrawText(m_text,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	ReleaseDC(dc);
	ValidateRect(NULL);
}

/***************************************************
Create
	function is called by the CUGCtrl during constraction
	of the CUGCtrl, if the UG_ENABLE_SCROLLHINTS is defined.
Params:
	pParentWnd		- pointer to an instance of parent
					  control (CUGCtrl) object.
	hbrBackground	- pointer to a HBRUSH object, which
					  will be used to draw the backgroud
					  area of the too tip.
Returns:
	TRUE or FALSE indicating if the creation process
	was successful (TRUE) or failed (FALSE).
*****************************************************/
BOOL CUGHint::Create(CWnd* pParentWnd, HBRUSH hbrBackground)
{
	UNREFERENCED_PARAMETER(pParentWnd);
	//
	// create the tool tip window
	//

	if(hbrBackground==NULL)
	{	// if the brush was not specified use system brush
		hbrBackground=(HBRUSH) (COLOR_INFOBK+1);
	}

	WNDCLASS wndClass;
	wndClass.style=CS_SAVEBITS|CS_DBLCLKS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hCursor=0; 
    wndClass.hbrBackground=hbrBackground; 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("HintWnd");
	
	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	CRect rect(0,0,0,0);
    if(!CWnd::CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST, wndClass.lpszClassName, _T(""), 
		WS_BORDER|WS_POPUP, rect, NULL, 0, NULL))
	{
        return FALSE;
	}

	//init the variables
	m_text=_T("");					//display text
	m_textColor = RGB(0,0,0);		//text color
	m_backColor = RGB(255,255,224);	//background color
	m_windowAlign = UG_ALIGNLEFT;	//UG_ALIGNLEFT,UG_ALIGNRIGHT,UG_ALIGNCENTER
									//UG_ALIGNTOP,UG_ALIGNBOTTOM,UG_ALIGNVCENTER
	m_textAlign	= UG_ALIGNLEFT;		//UG_ALIGNLEFT,UG_ALIGNRIGHT,UG_ALIGNCENTER
	m_hFont		= NULL;				//font handle
	
	//get the font height
	CDC * dc = GetDC();
	CSize fontSize = dc->GetTextExtent(_T("X"),1);
	m_fontHeight = fontSize.cy;
	ReleaseDC(dc);

    return TRUE;
}

/***************************************************
SetFont
	function can be used to set new font to be used
	to draw the tool tips.
Params:
	font	- pointer to new font object to use
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::SetFont(CFont * font)
{
	m_hFont	= font;
	
	//get the font height
	CDC * dc =GetDC();
	if(m_hFont != NULL)
		dc->SelectObject(m_hFont);
	CSize s = dc->GetTextExtent(_T("Xy"),2);
	m_fontHeight = s.cy + 3;
	ReleaseDC(dc);

	return UG_SUCCESS;
}

/***************************************************
SetWindowAlign
	function is called everytime a tool window needs
	to be shown or moved.  The alignment set provides
	guide lines how the tool window should be positioned
	in regards to the (x, y) point passed in to the 
	MoveHintWindow function.
Params:
	align	- integer variable that defines window alignment.
			  Possible value can consist:
				UG_ALIGNLEFT,
				UG_ALIGNRIGHT,
				UG_ALIGNCENTER
			  combined with:
				UG_ALIGNTOP,
				UG_ALIGNBOTTOM,
				UG_ALIGNVCENTER
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int	CUGHint::SetWindowAlign(int align)
{
	m_windowAlign = align;
	return UG_SUCCESS;
}

/***************************************************
SetTextAlign
	function sets the the alignment of the text
	within the tool window.
Params:
	align	- integer variable that defines text alignment.
			  Possible value can be:
				UG_ALIGNLEFT,
				UG_ALIGNRIGHT,
				UG_ALIGNCENTER
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::SetTextAlign(int align)
{
	m_textAlign = align;
	return UG_SUCCESS;
}

/***************************************************
SetTextColor
	<...>
Params:
	color	- RGB value of new text color
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::SetTextColor(COLORREF color)
{
	m_textColor	= color;
	return UG_SUCCESS;
}

/***************************************************
SetBackColor
	<...>
Params:
	color	- RGB value of new background color
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::SetBackColor(COLORREF color)
{
	m_backColor	= color;
	return UG_SUCCESS;
}

/***************************************************
SetText
	<...>
Params:
	string	- new string to show in the tool tip
	update	- flag indicating if the tool window should
			  be repainted right away to reflect change.
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::SetText(LPCTSTR string,int update)
{
	m_text = string;

	if(update)
		Invalidate();

	return UG_SUCCESS;
}

/***************************************************
MoveHintWindow
	function is called whenever the tool window needs
	to be moved to a new position.  This normally happens
	when the user scrolls the view using scroll bars.
Params:
	x, y	- coordinates
	width	- minimum width of the tool window, the window
			  will be resized to fit the entire tool string.
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::MoveHintWindow(int x,int y,int width)
{
	CRect rect;
	CRect parentRect;
	int dif;

	//get the width of the string and reset the
	//specified width if needed
	CDC * dc =GetDC();
	// TD first select font...
	if(m_hFont != NULL)
		dc->SelectObject(m_hFont);

	// calculate the string's length in pixels
	CSize textSize = dc->GetTextExtent( m_text, m_text.GetLength());
	if(( textSize.cx+4 ) > width )
		width = textSize.cx+4;

	ReleaseDC(dc);
	
	//set up the horizontal pos
	if(m_windowAlign&UG_ALIGNCENTER)		//center
	{
		rect.left = x-(width/2);
		rect.right = x+width;
	}
	else if(m_windowAlign&UG_ALIGNRIGHT)	//right
	{
		rect.left = x-width;
		rect.right = x;
	}
	else									//left
	{
		rect.left = x;
		rect.right = x+width;
	}

	//set up the vertical pos
	if(m_windowAlign&UG_ALIGNVCENTER)		//center
	{
		rect.top	= y-(m_fontHeight/2);
		rect.bottom = rect.top+m_fontHeight;
	}
	else if(m_windowAlign&UG_ALIGNBOTTOM)	//bottom
	{
		rect.top	= y-m_fontHeight;
		rect.bottom = y;
	}
	else									//top
	{
		rect.top = y;
		rect.bottom = y+m_fontHeight;
	}

	//make sure the position is within the parent
	m_ctrl->GetClientRect( parentRect );
	// check the left border
	if( rect.left < 0 )
	{
		dif = 0 - rect.left;
		rect.left+=dif;
		rect.right +=dif;
	}
	// check the top border
	if(rect.top <0)
	{
		dif = 0 - rect.top;
		rect.top +=dif;
		rect.bottom +=dif;
	}
	// check the right border
	if(rect.right > parentRect.right)
	{
		dif = rect.right - parentRect.right;
		rect.right -=dif;
		rect.left -=dif;
	}
	// check the bottom border
	if(rect.bottom > parentRect.bottom)
	{
		dif = rect.bottom - parentRect.bottom;
		rect.top -= dif;
		rect.bottom -= dif;
	}

	m_ctrl->ClientToScreen(&rect);

	// reposition the tool window
	Hide();
	MoveWindow(&rect,TRUE);
	Show();
	// send message to redraw tool window
	SendMessage(WM_PAINT,0,0);

	return UG_SUCCESS;
}

/***************************************************
Hide
	function is called whenever the tool window needs
	to be hidden.
Params:
	<none>
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::Hide()
{
	ShowWindow(SW_HIDE);

	return UG_SUCCESS;
}

/***************************************************
Show
	function is called whenever the tool window needs
	to be shown.
Params:
	<none>
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGHint::Show()
{
	if(IsWindowVisible() == FALSE)
		ShowWindow(SW_SHOWNA);

	return UG_SUCCESS;
}

