/***********************************************
	Ultimate Grid
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.


	class CUGFormat
************************************************/

#include "stdafx.h"

#include "UGCtrl.h"
//#include "UGFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***********************************************
************************************************/
CUGCellFormat::CUGCellFormat(){
}

/***********************************************
************************************************/
CUGCellFormat::~CUGCellFormat(){
}

/***********************************************
************************************************/
void CUGCellFormat::ApplyDisplayFormat(CUGCell *cell){
	UNREFERENCED_PARAMETER(cell);
}
	
/***********************************************
return 
	0 - information valid
	1 - information invalid
************************************************/
int CUGCellFormat::ValidateCellInfo(CUGCell *cell){
	UNREFERENCED_PARAMETER(cell);
	return 0;
}
