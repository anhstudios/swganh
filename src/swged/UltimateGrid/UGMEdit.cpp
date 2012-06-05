/*************************************************************************
				Class Implementation : CUGMaskedEdit
**************************************************************************
	Source file : UGMEdit.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
*************************************************************************/

#include "stdafx.h"
#include <ctype.h>
#include "UGCtrl.h"
//#include "UGMEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
CUGMaskedEdit - Constructor
	Intialize all member variables
****************************************************/
CUGMaskedEdit::CUGMaskedEdit()
{

	m_ctrl = NULL;

	m_MaskKeyInProgress = FALSE;
	m_mask = _T("");
	m_useMask = FALSE;
	m_blankChar = _T('_');
	m_storeLiterals = 1;
	
	static TCHAR Chars[] ={_T("09#L?Aa&C")};
	m_maskChars = Chars; 	

}

/***************************************************
~CUGMaskedEdit - Destructor
	Free all allocated resources
****************************************************/
CUGMaskedEdit::~CUGMaskedEdit()
{
}


/***************************************************
Message Map
****************************************************/
BEGIN_MESSAGE_MAP(CUGMaskedEdit, CEdit)
	//{{AFX_MSG_MAP(CUGMaskedEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
OnKillFocus
	Restores the controls max. text length
Params:
	See CWnd::OnKillFocus
Return
	See CWnd::OnKillFocus
****************************************************/
void CUGMaskedEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	// the mask edit control sets max str length
	// this will cause a problem when some of the cells
	// do not use a mask (last length will still be set)
	SetLimitText((ULONG) -1 );
}

/***************************************************
OnSetFocus
	Retrieves the cells input mask
Params:
	See CWnd::OnSetFocus
Return
	See CWnd::OnSetFocus
****************************************************/
void CUGMaskedEdit::OnSetFocus(CWnd* pOldWnd) 
{
	
	SetMask(m_ctrl->m_editCell.GetMask());

	m_cancel = FALSE;
	m_continueFlag = FALSE;

	SetSel(0,-1);
	
	CEdit::OnSetFocus(pOldWnd);
}

/***************************************************
OnChar
	Checks to see if the characters being inputed 
	match the input mask, if so, then call the
	OnEditVerify event, and add the character if
	allowed to continue.
Params:
	See CWnd::OnChar
Return
	See CWnd::OnChar
****************************************************/
void CUGMaskedEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int col = m_ctrl->GetCurrentCol();
	long row = m_ctrl->GetCurrentRow();
	
	if(!m_MaskKeyInProgress){
		if(MaskKeyStrokes(&nChar)==FALSE)
			return;
		if(m_ctrl->OnEditVerify(col,row,this,&nChar)==FALSE)
			return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

/***************************************************
MaskKeyStrokes
	Checks the given key against its input position
	and the corresponding input mask character.
	If the input is allowed then TRUE is returned,
	otherwise FALSE
Params:
	vKey - key value
Return
	TRUE - if the key matches the mask
	FALSE - if the key does not match the mask
****************************************************/
int CUGMaskedEdit::MaskKeyStrokes(UINT *vKey){

	int key = *vKey;

	if(!m_useMask)
		return TRUE;

	//
	if(isprint(key)==0)
		return TRUE;
	
	//clear all selections, if any
	Clear();

	//check the key against the mask
	int startPos,endPos;
	GetSel(startPos,endPos);

	//make sure the string is not longer than the mask
	if(endPos >= m_mask.GetLength())
		return FALSE;

	//check to see if a literal is in this position
	int l = m_literals.GetAt(endPos);
	if(l != 8){
		m_MaskKeyInProgress = TRUE;
		#ifdef WIN32
			AfxCallWndProc(this,m_hWnd,WM_CHAR,l,1);
		#else
			SendMessage(WM_CHAR,l,1);
		#endif
		m_MaskKeyInProgress = FALSE;
		GetSel(startPos,endPos);
	}
	
	//check the key against the mask
	int c = m_mask.GetAt(endPos);
	switch(c){
		case '0':{
			if(isdigit(key))
				return TRUE;
			return FALSE;
		}
		case '9':{
			if(isdigit(key))
				return TRUE;
			if(key == VK_SPACE)
				return TRUE;
			return FALSE;
		}
		case '#':{
			if(isdigit(key))
				return TRUE;
			if(key == VK_SPACE || key == 43 || key == 45)
				return TRUE;
			return FALSE;
		}
		case 'L':{
			if(isalpha(key))
				return TRUE;
			return FALSE;
		}
		case '?':{
			if(isalpha(key))
				return TRUE;
			if(key == VK_SPACE)
				return TRUE;
			return FALSE;
		}
		case 'A':{
			if(isalnum(key))
				return TRUE;
			return FALSE;
		}
		case 'a':{
			if(isalnum(key))
				return TRUE;
			if(key == VK_SPACE)
				return TRUE;
			return FALSE;
		}
		case '&':{
			if(isprint(key))
				return TRUE;
			return FALSE;
		}
		case 'C':{
			if(isprint(key))
				return TRUE;
			return FALSE;
		}
	}

	return TRUE;
}

/***************************************************
ShowInvalidEntryMessage
	Called whenever a keystroke does not match the
	given mask. By default it produces a single beep.
Params
	none
Return
	none
****************************************************/
void CUGMaskedEdit::ShowInvalidEntryMessage(){
	MessageBeep((UINT)-1);
}

/***************************************************
SetMask
	Sets the mask to use during editing. 
Params:
	string - mask string
Return:
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGMaskedEdit::SetMask(LPCTSTR string){

	if(string == NULL){
		m_useMask = FALSE;
		return UG_SUCCESS;
	}

	//set the mask
	m_origMask = string;
	if(m_origMask.GetLength() > 0)
		m_useMask = TRUE;
	else{
		m_useMask = FALSE;
		return UG_SUCCESS;
	}

	int loop,loop2,pos;
	int len = m_origMask.GetLength();
	TCHAR *buf = new TCHAR[len+1];
	int maskChar;

	#ifdef WIN32
		SetLimitText(len);
	#else
		SendMessage(EM_LIMITTEXT,len,0);
	#endif

	//get the parsed mask
	pos =0;
	for(loop =0;loop < len;loop++){
		//check to see if the next mask char is a literal
		if(m_origMask.GetAt(loop)=='\\'){
			loop++;
			continue;
		}
		//check for each of the possible mask chars
		maskChar = FALSE;
		for(loop2 = 0 ;loop2 < 9;loop2++){
			if(m_origMask.GetAt(loop) == m_maskChars[loop2]){
				maskChar = TRUE;
				break;
			}
		}
		//copy the retults
		if(maskChar)
			buf[pos] = m_origMask[loop];
		else
			buf[pos] = 8;
		pos++;
	}
	//copy over the final mask string
	buf[pos]=0;
	m_mask = buf;


	//get the parsed literals
	pos =0;
	for(loop =0;loop < len;loop++){
		maskChar = FALSE;
		//check to see if the next mask char is a literal
		if(m_origMask.GetAt(loop)=='\\'){
			loop++;
		}
		else{
			//check for each of the possible mask chars
			for(loop2 = 0 ;loop2 < 9;loop2++){
				if(m_origMask.GetAt(loop) == m_maskChars[loop2]){
					maskChar = TRUE;
					break;
				}
			}
		}
		//copy the retults
		if(maskChar)
			buf[pos] = 8;
		else
			buf[pos] = m_origMask[loop];
		pos++;
	}
	//copy over the final literal string
	buf[pos]=0;
	m_literals = buf;
	delete[] buf;

	OnUpdate();

	return UG_SUCCESS;
}

/***************************************************
MaskString
	Applies the given string against the current
	mask. If characters within the string do not
	match the mask they will be disposed of.
Params:
	string - mask string
Return:
	UG_SUCCESS - success
	UG_ERROR - error
****************************************************/
int CUGMaskedEdit::MaskString(CString *string){

	//check to see if there is a mask
	if(m_useMask == FALSE)
		return 1;

	//make the mask
	int loop;
	int pos=0;	//original string pos
	int pos2=0; //new string pos
	int len = m_mask.GetLength();
	TCHAR *buf = new TCHAR[len+1];

	for(loop =0;loop <len;loop++){
		//check for literals
		if(m_mask.GetAt(loop) ==8){
			buf[pos2] = m_literals.GetAt(loop);
			pos2++;
		}
		//check to see if the string matches the mask
		else{
			int c = m_mask.GetAt(loop);
			if(pos == string->GetLength())
				break;
			int d = string->GetAt(pos);
			int valid = FALSE;
	
			switch(c){
				case '0':{
					if(isdigit(d))
						valid = TRUE;
					break;
				}
				case '9':{
					if(isdigit(d))
						valid = TRUE;
					if(d == VK_SPACE)
						valid = TRUE;
					break;
				}
				case '#':{
					if(isdigit(d))
						valid = TRUE;
					if( d == VK_SPACE || d == 43 || d == 45 )
						valid = TRUE;
					break;
				}
				case 'L':{
					if(isalpha(d))
						valid = TRUE;
					break;
				}
				case '?':{
					if(isalpha(d))
						valid = TRUE;
					if(d == VK_SPACE)
						valid = TRUE;
					break;
				}
				case 'A':{
					if(isalnum(d))
						valid = TRUE;
					break;
				}
				case 'a':{
					if(isalnum(d))
						valid = TRUE;
					if(d == VK_SPACE)
						valid = TRUE;
					break;
				}
				case '&':{
					if(isprint(d))
						valid = TRUE;
					break;
				}
				case 'C':{
					if(isprint(d))
						valid = TRUE;
					break;
				}
			}
			if(valid){
				buf[pos2] = (char)d;
				pos2++;
			}
			else
				loop--;
			
			pos++;
		}
		if(pos == string->GetLength())
			break;
	}

	
	buf[pos2]=0;
	*string = buf;
	delete[] buf;

	return UG_SUCCESS;
}

/***************************************************
OnUpdate
	Readies the edit control for editing
Params
	none
Return
	none
****************************************************/
void CUGMaskedEdit::OnUpdate() 
{
	if(m_useMask && m_MaskKeyInProgress == FALSE){
		int startPos,endPos;
		GetSel(startPos,endPos);
		CString s;
		GetWindowText(s);
		if(s.GetLength() >0){
			m_MaskKeyInProgress = TRUE;
			MaskString(&s);
			SetWindowText(s);
			SetSel(startPos,endPos);
			m_MaskKeyInProgress = FALSE;
		}
	}
}

/***************************************************
PreCreateWindow
	ensures that the edit control has the 
	ES_AUTOHSCROLL style
Params:
	See CWnd::PreCreateWindow
Return
	See CWnd::PreCreateWindow
****************************************************/
BOOL CUGMaskedEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= ES_AUTOHSCROLL;
	return CEdit::PreCreateWindow(cs);
}
