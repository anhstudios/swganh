/*************************************************************************
	Header file : ugdefine.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

    Purpose
		This class file contains all standard defines for the Ultimate Grid.
*************************************************************************/
#ifndef _ugdefine_H_
#define _ugdefine_H_

//bit defines
#define BIT0	0x000000001
#define BIT1	0x000000002
#define BIT2	0x000000004
#define BIT3	0x000000008
#define BIT4	0x000000010
#define BIT5	0x000000020
#define BIT6	0x000000040
#define BIT7	0x000000080
#define BIT8	0x000000100
#define BIT9	0x000000200
#define BIT10	0x000000400
#define BIT11	0x000000800
#define BIT12	0x000001000
#define BIT13	0x000002000
#define BIT14	0x000004000
#define BIT15	0x000008000
#define BIT16	0x000010000
#define BIT17	0x000020000
#define BIT18	0x000040000
#define BIT19	0x000080000
#define BIT20	0x000100000
#define BIT21	0x000200000
#define BIT22	0x000400000
#define BIT23	0x000800000
#define BIT24	0x001000000
#define BIT25	0x002000000
#define BIT26	0x004000000
#define BIT27	0x008000000
#define BIT28	0x010000000
#define BIT29	0x020000000
#define BIT30	0x040000000
#define BIT31	0x080000000

#ifndef UG_CLASS_DECL
	#ifdef _BUILD_UG_INTO_EXTDLL
		#define UG_CLASS_DECL AFX_CLASS_EXPORT
	#elif defined _LINK_TO_UG_IN_EXTDLL
		#define UG_CLASS_DECL AFX_CLASS_IMPORT
	#else
		#define UG_CLASS_DECL
	#endif
#endif

//UGID structure defines
#define LPCUGID const UGID *

// up/down movement defines
#define UG_LINEUP	0
#define UG_LINEDOWN	1
#define UG_PAGEUP	2
#define UG_PAGEDOWN	3
#define UG_TOP		4
#define UG_BOTTOM	5

// left/right movement defines
#define UG_COLLEFT	0
#define UG_COLRIGHT	1
#define UG_PAGELEFT	2
#define UG_PAGERIGHT	3
#define UG_LEFT		4
#define UG_RIGHT	5

//common return codes  - error and warning codes are positive values
#define UG_SUCCESS		0
#define UG_ERROR		1
#define UG_NA			-1

#define UG_PROCESSED	1

//data source
#define	UG_MAXDATASOURCES 5

//scrolling
#define UG_SCROLLNORMAL		0
#define UG_SCROLLTRACKING	1
#define UG_SCROLLJOYSTICK	2

#define UG_DRAGDROP_SCROLL_BORDER	10

//ballistics
#define UG_BALLISTICS_OFF		0
#define UG_BALLISTICS_NORMAL	1
#define UG_BALLISTICS_SQAURED	2
#define UG_BALLISTICS_CUBED		3

//alignment defines
#define UG_ALIGNLEFT		BIT0
#define UG_ALIGNRIGHT		BIT1
#define UG_ALIGNCENTER		BIT2
#define UG_ALIGNTOP			BIT3
#define UG_ALIGNBOTTOM		BIT4
#define UG_ALIGNVCENTER		BIT5

//border style defines
#define UG_BDR_LTHIN		BIT0
#define UG_BDR_TTHIN		BIT1
#define UG_BDR_RTHIN		BIT2
#define UG_BDR_BTHIN		BIT3
#define UG_BDR_LMEDIUM		BIT4
#define UG_BDR_TMEDIUM		BIT5
#define UG_BDR_RMEDIUM		BIT6
#define UG_BDR_BMEDIUM		BIT7
#define UG_BDR_LTHICK		BIT8
#define UG_BDR_TTHICK		BIT9
#define UG_BDR_RTHICK		BIT10
#define UG_BDR_BTHICK		BIT11
#define UG_BDR_RECESSED		BIT12
#define UG_BDR_RAISED		BIT13

//sorting
#define UG_SORT_ASCENDING		1
#define UG_SORT_DESCENDING		2

//finding
#define UG_FIND_PARTIAL			1
#define UG_FIND_CASEINSENSITIVE	2
#define UG_FIND_UP				4
#define UG_FIND_ALLCOLUMNS		8

//printing defines
#define UG_PRINT_TOPHEADING		1
#define UG_PRINT_SIDEHEADING	2
#define UG_PRINT_LEFTMARGIN		3
#define UG_PRINT_TOPMARGIN		4
#define UG_PRINT_RIGHTMARGIN	5
#define UG_PRINT_BOTTOMMARGIN	6
#define UG_PRINT_LANDSCAPE		7
#define UG_PRINT_SMALL			8
#define UG_PRINT_LARGE			9
#define UG_PRINT_FRAME			10

//edit control define
#define UG_EDITID			123
#define UG_MEDITID			124

//CellType Custom Windows Message
#define UGCT_MESSAGE		WM_USER+100

//best fit defines
#define UG_BESTFIT_TOPHEADINGS	1
#define UG_BESTFIT_AVERAGE		2


//Normal CellType/CellType Ex Values - most other cell types 
//also use these values, so bits 1-8 are reserved generally reserved
#define UGCT_NORMAL				0
#define UGCT_NORMALSINGLELINE	BIT0 
#define UGCT_NORMALMULTILINE	BIT1
#define UGCT_NORMALELLIPSIS		BIT2
#define UGCT_NORMALLABELTEXT	BIT3

//Droplist CellType
#define UGCT_DROPLIST		1
//CheckBox CellType
#define UGCT_CHECKBOX		2
//Arrow CellType
#define UGCT_ARROW			3

//SpinButton CellType/CellTypeEx Values
//Spinbutton OnCellType notifications
#define UGCT_SPINBUTTONUP	16
#define UGCT_SPINBUTTONDOWN	17
#define UGCT_SPINBUTTONHIDEBUTTON	BIT9

//Grid Sections - used by menu commands, etc
#define UG_GRID				1
#define UG_TOPHEADING		2
#define UG_SIDEHEADING		3
#define UG_CORNERBUTTON		4
#define UG_HSCROLL  		5
#define UG_VSCROLL  		6
#define UG_TAB				7

#endif // _ugdefine_H_