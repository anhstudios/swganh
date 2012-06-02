/*************************************************************************
	Header file : ugstruct.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	UGID - Ultimate Grid global ID - GUID
*************************************************************************/
#ifndef _ugstruct_H_
#define _ugstruct_H_

typedef struct
{
    unsigned long	Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char	Data4[8];
} UGID;


#endif // _ugstruct_H_