/*************************************************************************
				Class Declaration : CUGDropTarget
**************************************************************************
	Source file : UGDrgDrp.cpp
	Header file : UGDrgDrp.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGDropTarget class is used by the Ultimate
		Grid to provide the drag-and-drop functionality.
		An instance of this object is only created when
		the 'AfxOle.h' is included and OLE initialized.

		This class receives messages related to the
		drag-and-drop and passed them to the CUGCtrl
		derived class for further processing.  It will
		also take care of the copy and paste functionality
		if needed.
*************************************************************************/
#include "ugctrl.h"

#ifndef _UGDrgDrp_H_
#define _UGDrgDrp_H_

#ifdef UG_ENABLE_DRAGDROP

class UG_CLASS_DECL CUGDropTarget: public COleDropTarget
{
public:
	CUGDropTarget();
	~CUGDropTarget();

	//pointer to the main class
	CUGCtrl	*m_ctrl;

	virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDropEx( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );

	virtual DROPEFFECT OnDragScroll( CWnd* pWnd, DWORD dwKeyState, CPoint point );
};

#endif // UG_ENABLE_DRAGDROP
#endif // _UGDrgDrp_H_