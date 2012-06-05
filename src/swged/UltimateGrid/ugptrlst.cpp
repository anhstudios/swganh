/*************************************************************************
				Class Implementation : CUGPtrList
**************************************************************************
	Source file : ugptrlst.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGPtrList::CUGPtrList()
{
	m_elementsUsed = 0;
	m_maxElements = 0;
	m_arrayPtr = NULL;
}

CUGPtrList::~CUGPtrList()
{
	EmptyList();
}

/***************************************************
AddPointer
	function is called to add a generic (void) pointer
	to the link list.  The list will be created if
	it does not contain any elements
Params:
	ptr		- pointer to add
	param	- is generally used to indicate number of
			  references to this pointer.
	id		- additional information about the pointer.
Returns:
	zero based index, -1 on error
*****************************************************/
int CUGPtrList::AddPointer(void *ptr,long param,UGID*id)
{
	//check to see if more elements need to be added
	if(m_elementsUsed >= m_maxElements)
	{
		if(AddMoreElements() == UG_ERROR)
			return -1;
	}

	for(int loop = 0;loop <m_maxElements; loop++)
	{
		if(m_arrayPtr[loop].isUsed == FALSE)
		{
			m_arrayPtr[loop].isUsed = TRUE;
			m_arrayPtr[loop].pointer = ptr;
			m_arrayPtr[loop].param = param;
			if(id != NULL)
				memcpy(&m_arrayPtr[loop].id,id,sizeof(UGID));

			m_elementsUsed++;

			return loop;
		}
	}
	return -1;
}

/***************************************************
UpdateParam
	function is called to update the 'param' portion
	of the list item.  The 'param' is used to store
	information on how many times this pointer is
	referenced.
Params:
	index		- index of the pointer in the link list
	param		- new reference count
Returns:
	UG_SUCCESS	- upon success
	UG_ERROR	- if the index is out of range
	2			- if the index is not used.
*****************************************************/
int CUGPtrList::UpdateParam(int index,long param)
{
	if(index <0 || index >= m_maxElements)
		return UG_ERROR;

	if(m_arrayPtr[index].isUsed == FALSE)
		return 2;

	m_arrayPtr[index].param = param;
	
	return UG_SUCCESS;
}

/***************************************************
GetParam
	is called to objtain the current value of the
	param.
Params:
	index	- index of the pointer in the link list
Returns:
	a long variable indicating the current value of
	the 'param'.
*****************************************************/
long CUGPtrList::GetParam(int index)
{
	if(index <0 || index >= m_maxElements)
		return NULL;

	return m_arrayPtr[index].param;
}

/***************************************************
GetPointer
	called to retrive the pointer stored
Params:
	index	- index of the pointer in the link list
Returns:
	pointer to the object stored, or NULL on error
*****************************************************/
LPVOID CUGPtrList::GetPointer(int index)
{
	if(index <0 || index >= m_maxElements)
		return NULL;

	if(m_arrayPtr[index].isUsed == FALSE)
		return NULL;

	return m_arrayPtr[index].pointer;
}

/***************************************************
GetPointerIndex
	called to retrieve index of given pointer.
Params:
	ptr		- pointer to search for
Returns:
	zero based index, -1 on error
*****************************************************/
int CUGPtrList::GetPointerIndex(void * ptr)
{
	for(int loop = 0;loop <m_maxElements; loop++)
	{
		if(m_arrayPtr[loop].isUsed == FALSE)
			continue;

		if(m_arrayPtr[loop].pointer == ptr)
			return loop;
	}
	return -1;
}

/***************************************************
DeletePointer
	called to remove individual pointer from the list.
Params:
	index	- index of the pointer in the link list
Returns:
	UG_SUCCESS	- if the item is removed from the list
	UG_ERROR	- if index parameter is out of range
*****************************************************/
int CUGPtrList::DeletePointer(int index)
{
	if(index <0 || index >= m_maxElements)
		return UG_ERROR;

	if(m_arrayPtr[index].isUsed != FALSE)
	{
		m_arrayPtr[index].isUsed = FALSE;
		m_arrayPtr[index].pointer = NULL;
		m_elementsUsed--;
	}

	return UG_SUCCESS;
}

/***************************************************
EmptyList
	called to remove all of the items in the list.
Params:
	<none>
Returns:
	UG_SUCCESS, this function will never fail.
*****************************************************/
int CUGPtrList::EmptyList()
{
	delete[] m_arrayPtr;
	m_arrayPtr = NULL;
	
	m_elementsUsed = 0;
	m_maxElements = 0;

	return UG_SUCCESS;
}

/***************************************************
GetCount
	is called to retrieve the number of items in the list.
Params:
	<none>
Returns:
	number of items in the list
*****************************************************/
int CUGPtrList::GetCount()
{
	return m_elementsUsed;
}

/***************************************************
GetMaxCount
	called to rerieve current maximum number of 
	allowed items in the list.
Params:
	<none>
Returns:
	maximum number of items allowed in a list
*****************************************************/
int CUGPtrList::GetMaxCount()
{
	return m_maxElements;
}

/***************************************************
AddMoreElements
	function is called when maximum allowed items
	is reached.  It is called in attempt to allocate
	additional memory required.
Params:
	<none>
Returns:
	UG_SUCCESS	- on success
	UG_ERROR	- failed to allocated additional memory.
*****************************************************/
int CUGPtrList::AddMoreElements()
{
	UGPtrList* temp;
	int oldMaxElements = m_maxElements;

	try
	{
		temp = new UGPtrList[m_maxElements+10];
		m_maxElements += 10;
	}
	catch(...)
	{
		return UG_ERROR;
	}

	int loop;

	for(loop = 0;loop <oldMaxElements;loop++)
	{
		temp[loop].isUsed = m_arrayPtr[loop].isUsed;
		temp[loop].pointer = m_arrayPtr[loop].pointer;
		temp[loop].param = m_arrayPtr[loop].param;
		memcpy(&temp[loop].id,&m_arrayPtr[loop].id,sizeof(UGID));
	}

	for(loop = m_elementsUsed;loop <m_maxElements;loop++)
	{
		temp[loop].isUsed = FALSE;
	}
	delete[] m_arrayPtr;

	m_arrayPtr = temp;

	return UG_SUCCESS;
}

