/*************************************************************************
				Class Declaration : CUGCellFormat
**************************************************************************
	Source file : ugformat.cpp
	Header file : ugformat.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This class is used for the formating the 
		cells data for display and/or for editing.
*************************************************************************/
#ifndef _ugformat_H_
#define _ugformat_H_

class CUGCell;

class UG_CLASS_DECL CUGCellFormat: public CObject
{
public:
	CUGCellFormat();
	virtual ~CUGCellFormat();

	virtual void ApplyDisplayFormat(CUGCell *cell);
	
	virtual int  ValidateCellInfo(CUGCell *cell);
};

#endif	// _ugformat_H_