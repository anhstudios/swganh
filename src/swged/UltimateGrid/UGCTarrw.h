/*************************************************************************
				Class Declaration : CUGArrowType
**************************************************************************
	Source file : UGCTArrw.cpp
	Header file : UGCTArrw.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

  	Purpose
		The CUGArrowType is a basic cell type that allows to display
		left/right arrows and double arrows.

		To use this cell type all you have to do is set any cell's cell type
		property to UGCT_ARROW (or 3).

		Extended styles
			UGCT_ARROWRIGHT		- show a single arrow to the right
			UGCT_ARROWLEFT		- show a single arrow to the left
			UGCT_ARROWDRIGHT	- show a double arrow to the right
			UGCT_ARROWDLEFT		- show a double arrow to the left

**************************************************************************/
#ifndef _UGCTarrw_H_
#define _UGCTarrw_H_

// define cell type extensions
#define UGCT_ARROWRIGHT		BIT4
#define UGCT_ARROWLEFT		BIT5
#define	UGCT_ARROWDRIGHT	BIT6
#define	UGCT_ARROWDLEFT		BIT7

// define size of each triangle
#define UG_TRIANGLE_WIDTH	4
#define UG_TRIANGLE_HEIGHT	7

//CUGArrowType class declaration
class UG_CLASS_DECL CUGArrowType: public CUGCellType
{
public:
	CUGArrowType();
	~CUGArrowType();

	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
};

#endif //_UGCTarrw_H_