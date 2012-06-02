/*************************************************************************
				Class Implementation : CUGEditBase
**************************************************************************
	Source file : UGEditBase.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/

#include "stdafx.h"
#include <ATLConv.h>
#include "ugctrl.h"
//#include "UGEditBase.h"

#include "UGXPThemes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
CUGEditBase - Constructor
****************************************************/
CUGEditBase::CUGEditBase() :
	m_continueCol(0),
	m_continueRow(0),
	m_continueFlag(0),
	m_cancel(0),
	m_lastKey(0),
	m_ctrl(NULL)
{
	m_Brush.CreateStockObject(HOLLOW_BRUSH);
}

/***************************************************
~CUGEditBase - Destructor
****************************************************/
CUGEditBase::~CUGEditBase()
{
	m_Brush.DeleteObject();
}

/***************************************************
MessageMap
****************************************************/
BEGIN_MESSAGE_MAP(CUGEditBase, CEdit)
	//{{AFX_MSG_MAP(CUGEditBase)
	ON_WM_CTLCOLOR_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
WindowProc
	Provides all of the default functionality that
	any CEdit based Ultimate Grid edit control must
	have. 
	
	Most CEdit derived edit controls can be easily
	used in Ultimate Grid by just changing the base 
	class. There is generally no need to change any
	functions calls from CEdit::xxx to CUGEditBase::xxx
	
	All of the messages are put here instead of 
	in a Message Map, so that the so that they are 
	properly called, without having to switch any 
	CEdit::xxx calls over to CUGEditBase::xxx calls

	The only stipulation is that the CEdit derived
	class cannot override the WindowsProc. If it
	does, then the base funtionality will be rendered
	useless. In this case, the code below may be copied
	into the controls WindowsProc (also the member 
	variables will need to be copied over and initialized
	as well).
Params
	See CWnd::WindowsProc
Return
	See CWnd::WindowsProc
****************************************************/
LRESULT CUGEditBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
		//***** WMCREATE *****
		case WM_CREATE:{
			
			if(m_ctrl == NULL)
				m_ctrl = (CUGCtrl*)GetParent();
			
			break;
		}
		//***** WM_SETFOCUS *****
		case WM_SETFOCUS:{
			
			if(m_ctrl == NULL)
				m_ctrl = (CUGCtrl*)GetParent();

			CEdit::WindowProc(message, wParam, lParam);

			m_lastKey = 0;
			m_cancel = FALSE;
			m_continueFlag = FALSE;

			SetSel(GetWindowTextLength(),0);

			if(m_ctrl->m_editCell.GetReadOnly())
				SetReadOnly(TRUE);

			return 0;
		}
		//***** WM_KILLFOCUS *****
		case WM_KILLFOCUS:{
			
			SetReadOnly(FALSE);
			
			CEdit::WindowProc(message, wParam, lParam);
			
			CWnd* pNewWnd = CWnd::FromHandle((HWND)wParam);

			CEdit::OnKillFocus(pNewWnd);

			if( m_ctrl->m_GI->m_bCancelMode != FALSE )
			{
				if(pNewWnd->GetSafeHwnd() != NULL)
				{
					if(pNewWnd != m_ctrl && pNewWnd->GetParent() != m_ctrl)
						m_cancel = TRUE;
				}
				else
					m_cancel = TRUE;
			}

			if( pNewWnd != m_ctrl )
				m_ctrl->OnKillFocus( UG_GRID, pNewWnd );

			if(pNewWnd->GetSafeHwnd() != NULL)
			{
				if( pNewWnd == m_ctrl )
					::SetFocus( m_ctrl->m_CUGGrid->GetSafeHwnd());
			}

			CString string;
			GetWindowText(string);

			if(m_ctrl->EditCtrlFinished(string,m_cancel,m_continueFlag,m_continueCol,m_continueRow) == FALSE)
			{
				m_ctrl->GotoCell(m_ctrl->m_editCol,m_ctrl->m_editRow);
				SetCapture();
				ReleaseCapture();
				SetFocus();
			}
			return 0;
		}
			// We need this to stop the control from drawing itself outside WM_PAINT when selecting text.
		case WM_MOUSEMOVE:
			{
				UpdateCtrl();
			}
			break;
		//***** WM_KEYDOWN *****
		case WM_KEYDOWN:{

			m_lastKey = (UINT)wParam;

			switch(wParam){
				case VK_TAB:{

					if(GetKeyState(VK_SHIFT)<0){
						m_continueCol = m_ctrl->m_editCol -1;
						m_continueRow = m_ctrl->m_editRow;
					}
					else{
						m_continueCol = m_ctrl->m_editCol +1;
						m_continueRow = m_ctrl->m_editRow;
					}

					m_continueFlag = TRUE;
					m_ctrl->SetFocus();
					return 0;
				}
				case VK_RETURN:{

					if( GetKeyState(VK_CONTROL) < 0 &&
						m_ctrl->m_editCell.GetCellTypeEx() & UGCT_NORMALMULTILINE )
					{	// in mulitiline cells allow CTRL-RETURN to advance to a new line
						return 0;
					}

					m_continueCol = m_ctrl->m_editCol;
					m_continueRow = m_ctrl->m_editRow +1;
					m_continueFlag = TRUE;
					m_ctrl->SetFocus();

					if (UGXPThemes::IsThemed())
						UpdateCtrl();

					return 0;
				}

				case VK_ESCAPE:{
					m_cancel = TRUE;
					m_continueFlag = FALSE;
					m_ctrl->SetFocus();
					return 0;
				}

				case VK_UP:{
					m_continueCol = m_ctrl->m_editCol;
					m_continueRow = m_ctrl->m_editRow-1;
					m_continueFlag = TRUE;
					m_ctrl->SetFocus();

					if (UGXPThemes::IsThemed())
						UpdateCtrl();

					return 0;
				}

				case VK_DOWN:{
					m_continueCol = m_ctrl->m_editCol;
					m_continueRow = m_ctrl->m_editRow+1;
					m_continueFlag = TRUE;
					m_ctrl->SetFocus();

					if (UGXPThemes::IsThemed())
						UpdateCtrl();

					return 0;
				}
				
				case VK_RIGHT:{
					CString cellText;
					int cellTextLength;
					int nStartChar;
					int nStopChar;

					// Get edited text and current selection information
					CEdit::GetWindowText(cellText);
					cellTextLength = cellText.GetLength();
					CEdit::GetSel(nStartChar, nStopChar);

					// Allow edit to jump to the next cell if the user hits the
					// right key and:
					// - the cursor reached the end of the cell
					if( nStartChar == nStopChar && nStopChar == cellTextLength )
					{
						m_continueCol = m_ctrl->m_editCol + 1;
						m_continueRow = m_ctrl->m_editRow;
						m_continueFlag = TRUE;
						m_ctrl->SetFocus();

						if (UGXPThemes::IsThemed())
							UpdateCtrl();

						return 0;
					}
					// - or the entire cell's text content is selected
					else if (nStartChar == 0 && nStopChar == cellTextLength)
					{
						m_continueCol = m_ctrl->m_editCol + 1;
						m_continueRow = m_ctrl->m_editRow;
						m_continueFlag = TRUE;
						m_ctrl->SetFocus();

						if (UGXPThemes::IsThemed())
							UpdateCtrl();

						return 0;
					}

					if (UGXPThemes::IsThemed())
						Invalidate();

					break;
				}

				case VK_LEFT:{
					CString cellText;
					int cellTextLength;
					int nStartChar;
					int nStopChar;

					// Get edited text and current selection information
					CEdit::GetWindowText(cellText);
					cellTextLength = cellText.GetLength();
					CEdit::GetSel(nStartChar, nStopChar);

					// If the user hit the Left key and the cursor position is at
					// the beginning of the edit strin without any selection
					// then allow the edit to jump to the previous cell.
					if( nStartChar == nStopChar && nStartChar == 0 )
					{
						m_continueCol = m_ctrl->m_editCol - 1;
						m_continueRow = m_ctrl->m_editRow;
						m_continueFlag = TRUE;
						m_ctrl->SetFocus();


						if (UGXPThemes::IsThemed())
							UpdateCtrl();

						return 0;
					}

					if (UGXPThemes::IsThemed())
						Invalidate();


					break;
				}

				case VK_DELETE:{
					// The OnEditVerify should also called when user enters
					// the DELETE key.  ASCII representation of the DEL key is decimal 127
					UINT nChar = 127;
					int col = m_ctrl->GetCurrentCol();
					long row = m_ctrl->GetCurrentRow();

					// First the datasource has a look ...
					if(m_ctrl->m_GI->m_defDataSource->OnEditVerify(col,row,this,&nChar) == FALSE)
						return 0;
					// then the ctrl class
					if(m_ctrl->OnEditVerify(col,row,this,&nChar) == FALSE)
						return 0;
				}
			}

			break;
		}
		//***** WM_CHAR *****
		case WM_CHAR:{

			UINT nChar = (UINT)wParam;
			m_lastKey = nChar;

			if(nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
				return 0;

			// allow OnEditVerify a look at the char...
			int col = m_ctrl->GetCurrentCol();
			long row = m_ctrl->GetCurrentRow();
			
			// First the datasource has a look ...
			if(m_ctrl->m_GI->m_defDataSource->OnEditVerify(col,row,this,&nChar) == FALSE)
				return 0;

			// then the ctrl class
			if(m_ctrl->OnEditVerify(col,row,this,&nChar) == FALSE)
				return 0;

			// if the characted is allowed by both datasource and ctrl class
			// than set back any changes to the character that user could make
			wParam = (WPARAM)nChar;
			// and allow the edit control to process the new char.

			return CEdit::WindowProc(message, wParam, lParam);
		}
		//***** WM_GETDLGCODE *****
		case WM_GETDLGCODE:{

			return DLGC_WANTALLKEYS | DLGC_WANTARROWS;
		}
		//***** WM_MOUSEACTIVATE *****
		case WM_MOUSEACTIVATE:
			
			return MA_ACTIVATE;	

//		case WM_ERASEBKGND:
		case WM_PAINT:
		{
			if(!UGXPThemes::UseThemes())
				break;
			PAINTSTRUCT ps;
			CDC* cdc = BeginPaint(&ps);
			RECT rc;
			GetWindowRect(&rc);
			ScreenToClient(&rc);

			if (UGXPThemes::DrawBackground(m_hWnd, *cdc, XPCellTypeData, (UGXPThemeState)( ThemeStateSelected | ThemeStateCurrent), &rc, NULL))
			{
				CString text;

				GetWindowText(text);
				// Use the font in the edit box, not the themed font
				// this is because although we're doing our own drawing,
				// the edit box is still telling us how to lay out the text,
				// what is selected, etc.  Changing fonts will make a mess of all of this.
				CFont * pOld = cdc->SelectObject(this->GetFont());
				cdc->SetBkMode(TRANSPARENT);
				RECT rcText(rc);
				rcText.left += 2;
				rcText.top += 2;

				// Work out the part of the string that's selected.
				int nFirstChar = LOWORD(CharFromPos(CPoint(rcText.left, rcText.top)));
				int nBegin, nEnd;
				GetSel(nBegin, nEnd);
				bool bSelected = (nBegin != nEnd);

				nBegin -= nFirstChar;
				nEnd -= nFirstChar;

				RECT rcDraw;
				CopyRect(&rcDraw, &rcText);

				int nRowCount = GetLineCount();

				TCHAR nChar[1024];

				int nCharCount = nFirstChar;

				COLORREF backCol = cdc->GetBkColor();

				// Step through each line of the text.  By letting the
				// edit control tell us what's on each line, we make sure
				// we draw it as the underlying code expects it to be.
				for(int i = this->GetFirstVisibleLine(); i < nRowCount; ++i)
				{
					int nRowChars = GetLine(i, &nChar[0], 1024);

					if (GetStyle() & ES_PASSWORD)
					{
						::memset(&nChar[0], '*', nRowChars);
					}

					nChar[nRowChars] = 0;
					
					cdc->DrawText(&nChar[0], &rcText, DT_LEFT);
					CSize szText = cdc->GetTextExtent(&nChar[0]);
					CString row(&nChar[0]);
					::memset(&nChar[0], 0, 1024);

					rcText.top += szText.cy;

					if (nCharCount + nRowChars >= nBegin && bSelected)
					{
						cdc->SetBkMode(OPAQUE);
						cdc->SetBkColor(0xD00000);

						CString beforeSel = row.Left(nBegin - nCharCount);// text.Mid(nCharCount, nBegin-nCharCount);

						CSize size = cdc->GetTextExtent(beforeSel);

						CString inSel = row.Mid(nBegin - nCharCount, nEnd - nBegin);// text.Mid(nBegin, min(nEnd - nBegin, nRowChars + nCharCount - nBegin));
 
						rcDraw.left += size.cx;
						cdc->DrawText(inSel, &rcDraw, DT_LEFT);
						rcDraw.left = rcText.left;

						// If we selected the entire line, then we need the size of the drawn text for the height
						if (size.cy == 0)
							size = cdc->GetTextExtent(inSel);
	
						rcDraw.top += size.cy;

						cdc->SetBkMode(TRANSPARENT);
						cdc->SetBkColor(backCol);

						// This is to deal with /r/n appearing in the selection text.
						nEnd -= 2;
					}
					else
					{
						CString inSel = text.Mid(nCharCount, nRowChars);
						CSize size = cdc->GetTextExtent(inSel);
						rcDraw.top += size.cy;
					}

					nCharCount += nRowChars;

					if (nBegin < nCharCount) nBegin = nCharCount;

					// This is to deal with /r/n appearing in the selection text.
					nBegin -= 2;
					nEnd -= 2;
				}

				cdc->SelectObject(pOld);

				EndPaint(&ps);
				return true;
			}

			EndPaint(&ps);
		}
		break;
	}

	return CEdit::WindowProc(message, wParam, lParam);
}

/***************************************************
GetLastKey
	GetLastKey is handy if you need to know if
	ESC cancelled the edit.  Key state is not 
	saved (shift, ctrl etc)
Params:
	<none>
Returns:
	last key pressed
*****************************************************/
UINT CUGEditBase::GetLastKey()
{
	return m_lastKey;
}


void CUGEditBase::UpdateCtrl()
{
	if (UGXPThemes::IsThemed())
	{		
		CWnd* pParent = GetParent();
		CRect rect;
	    
		GetWindowRect(rect);
		pParent->ScreenToClient(rect);
		pParent->InvalidateRect(rect, TRUE);

		Invalidate(FALSE);
	}
}


void CUGEditBase::OnKillfocus() 
{
	if (UGXPThemes::IsThemed())
	    UpdateCtrl();

	CEdit::OnKillFocus(this);
}

void CUGEditBase::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (UGXPThemes::IsThemed())
	    UpdateCtrl();
    
    CEdit::OnLButtonDown(nFlags, point);
}


HBRUSH CUGEditBase::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (UGXPThemes::IsThemed())
	{
		pDC->SetBkMode(TRANSPARENT);	
		return (HBRUSH) m_Brush;
	}
	else
	{
		return CEdit::OnCtlColor(pDC, this, nCtlColor);
	}
}