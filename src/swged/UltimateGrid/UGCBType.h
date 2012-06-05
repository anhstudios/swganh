/*************************************************************************
				Class Declaration : CUGCheckBoxType
**************************************************************************
	Source file : UGCBType.cpp
	Header file : UGCBType.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGCheckBoxType class is standard check box cell type in the
		Ultimate Grid control.

		By default an instance of this class is created and added, and
		can be used without any additional coding.

		To use this cell type all you have to do is set any cell's cell type
		property to UGCT_CHECKBOX (or 2).

		Extended styles
			UGCT_CHECKBOXUSEALIGN	- use this style to allow the check box
									  to respect the alignment set to a cell.
									  Please note that when this style is set
									  then the check box will not draw the text
									  assigned to a cell.

			UGCT_CHECKBOXFLAT		- check box is drawn in default flat mode
			UGCT_CHECKBOX3DRECESS	- the frame of the check should be shown as sunken
			UGCT_CHECKBOX3DRAISED	- the frame of the check should be shown as raised

			UGCT_CHECKBOXCROSS		- when a check box is set than the check
									  mark will be drawn as a cross.
			UGCT_CHECKBOXCHECKMARK	- when a check box is set than the check
									  mark will be drawn as a check mark.

			UGCT_CHECKBOX3STATE		- the check box is using 3 check states.
										- not checked
										- checked
										- un certain

		Notificatoins sent to the OnCellTypeNotify:
			UGCT_CHECKBOXSET		- provides information that the user has 
									  changed the state of the check box

*************************************************************************/
#ifndef _UGCBType_H_
#define _UGCBType_H_

// extended cell type styles
#define UGCT_CHECKBOXFLAT		0
#define UGCT_CHECKBOXCROSS		0
#define UGCT_CHECKBOX3DRECESS	BIT8
#define UGCT_CHECKBOX3DRAISED	BIT9
#define UGCT_CHECKBOXROUND		BIT10
#define	UGCT_CHECKBOXUSEALIGN	BIT11
#define UGCT_CHECKBOXCHECKMARK	BIT12
#define UGCT_CHECKBOX3STATE		BIT13
#define UGCT_CHECKBOXDISABLED   BIT14
// cell type notification
#define UGCT_CHECKBOXSET		53
// define the maximum size of the check box
#define	UGCT_CHECKSIZE			12
// and the margin
#define UGCT_CHECKMARGIN		3

//CUGCheckBoxType class definition
class UG_CLASS_DECL CUGCheckBoxType: public CUGCellType
{
protected:
	CPen	m_darkPen;
	CPen	m_lightPen;
	CPen	m_facePen;
	HBRUSH	m_hbrDither;

	CUGCell m_cell;

public:
	CUGCheckBoxType();
	~CUGCheckBoxType();

	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();

	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
	virtual BOOL OnCharDown(int col,long row,UINT *vcKey);
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);

protected:
	void AdjustRect( RECT *rect, int alignment, int style, int top, int squareSize = UGCT_CHECKSIZE );
	HBITMAP CreateDitherBitmap();
	void FillDitheredRect( CDC* pDC, const CRect& rect );
};

#endif //_UGCBType_H_