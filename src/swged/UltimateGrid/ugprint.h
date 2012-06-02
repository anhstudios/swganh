/*************************************************************************
				Class Declaration : CUGPrint
**************************************************************************
	Source file : ugprint.cpp
	Header file : ugprint.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This class handles the setup and printing
		of the grid to a printer or other printing
		device.
	Details
		-This class has two printing sections,
		 setup, and printing. Setup is required
		 to figure out how many pages it will take
		 to print a range of cells from the grid.
		 Plus which cells to print on which pages.
		 Once the setup is complete a page can
		 now be printed.
		-Printing setup function: PrintInit
		-Page Printing function: PrintPage
		-The rest of the printing functons are optional
		 and are used to setup different print options.	
*************************************************************************/
#ifdef UG_ENABLE_PRINTING

#ifndef _ugprint_H_
#define _ugprint_H_

#include "Afxtempl.h"

class UG_CLASS_DECL CUGPrint
{
private:

	typedef struct CUGPrintListTag
	{
		int		page;
		int		startCol;
		int		endCol;
		long	startRow;
		long	endRow;
		CUGPrintListTag * next;
	}CUGPrintList;

	CUGPrintList*	m_printList;

	int				m_pagesWide;
	int				m_pagesHigh;

	float			m_printVScale;
	float			m_printHScale;

	int				m_printLeftMarginPX;
	int				m_printTopMarginPX;
	int				m_printRightMarginPX;
	int				m_printBottomMarginPX;
	
	int				m_paperWidthPX;
	int				m_paperHeightPX;

	int				m_virPixWidth;
	BOOL			m_bIsPreview;

	//overall print scale (percentage)
	int				m_printScale; 
	
	//print options
	int	m_printLeftMargin;	//margin in mm
	int	m_printTopMargin;
	int	m_printRightMargin;
	int	m_printBottomMargin;
	
	BOOL m_printFrame;		//print a frame(border) around the grid
	BOOL m_printCenter;		//center the grid within the margins
	BOOL m_fitToPage;		//fit the range of cells to one page
	BOOL m_printTopHeading;	//print the top heading
	BOOL m_printSideHeading;//print the side heading

	double m_scaleMultiplier;

	void ClearPrintList();
	int	AddPageToList(int page,int startCol,int endCol,long startRow,long endRow);
	int	GetPageFromList(int page,int *startCol,int *endCol,long *startRow,long *endRow);

	void InternPrintCell(CDC *dc,RECT * rect,int col,long row);

public:
	
	CUGCtrl *		m_ctrl;
	CUGGridInfo *	m_GI;
	CArray<RECT,RECT> m_ColRects;

	CUGPrint();
	~CUGPrint();

	int PrintInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
				int endCol,long endRow);
	int PrintPage(CDC * pDC, int pageNum);
	int PrintSetMargin(int whichMargin,int size);
	int PrintSetScale(int scalePercent);
	int PrintSetOption(int option,long param);
	int PrintGetOption(int option,long *param);

	// This function returns the m_printVScale.
	// If the CDC is not printer it returns 1.0.
	// It is used in dwawing cells.
	inline float GetPrintVScale (CDC* pDC)
	{
		float fScale = 1.0;
		if (pDC->IsPrinting()) fScale = m_printVScale;
		return fScale;
	}

	// This function returns the m_printHScale.
	// If the CDC is not printer it returns 1.0.
	// It is used in dwawing cells.
	inline float GetPrintHScale (CDC* pDC)
	{
		float fScale = 1.0;
		if (pDC->IsPrinting()) fScale = m_printHScale;
		return fScale;
	}

	// This function returns the m_printSideHeading.
	// It is used in dwawing cells.
	inline BOOL PrintSideHeading (void)
	{
		return m_printSideHeading;
	}
};

#endif // _ugprint_H_
#endif // UG_ENABLE_PRINTING
