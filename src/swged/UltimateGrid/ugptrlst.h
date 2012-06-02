/*************************************************************************
				Class Declaration : CUGPtrList
**************************************************************************
	Source file : ugptrlst.cpp
	Header file : ugptrlst.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGPrtList class is a specialized
		link list, which is used by the Ultimate
		Grid to store additional pointers that are
		assigned to the grid (i.e.: additional
		fonts, bitmaps, celltypes, etc.).

		This class also provides functions to add,
		remove, and access stored pointers or the
		additional information that is storred
		with each pointer.
*************************************************************************/
#ifndef _ugptrlst_H_
#define _ugptrlst_H_

class UG_CLASS_DECL CUGPtrList: public CObject
{
protected:
	
	typedef struct UGPtrListTag
	{
		BOOL    isUsed;
		LPVOID	pointer;
		long	param;
		UGID	id;
	}UGPtrList;

	UGPtrList * m_arrayPtr;

	int		m_maxElements;
	int		m_elementsUsed;

	int AddMoreElements();

public:
	CUGPtrList();
	~CUGPtrList();

	int AddPointer(void *ptr,long param = 0,UGID *id = NULL);
	
	LPVOID GetPointer(int index);
	long GetParam(int index);
	UGID* GetUGID(int index);

	int GetPointerIndex(void * ptr);

	int UpdateParam(int index,long param);

	int DeletePointer(int index);
	int EmptyList();

	int GetCount();
	int GetMaxCount();

	int InitEnum();
	void *EnumPointer();
};

#endif // _ugptrlst_H_