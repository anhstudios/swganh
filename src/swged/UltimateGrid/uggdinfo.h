/*************************************************************************
				Class Declaration : CUGGridInfo
**************************************************************************
	Source file : uggdinfo.cpp
	Header file : uggdinfoh
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGGridInfo class contains setup information
		about each sheet in the grid.  All of the memeber
		variables of this class are set externally by the
		CUGCtrl class.
	Details
		The CUGCtrl class holds an array of these
		classes where each sheet has one entry.
*************************************************************************/
#ifndef _uggdinfo_H_
#define _uggdinfo_H_

#pragma warning (disable: 4786)


typedef struct _UGCOLINFO
{
	int				width;
	CUGDataSource *	dataSource;
	CUGCell	*		colDefault;
	int				colTranslation;

}UGCOLINFO;

class UG_CLASS_DECL CUGGridInfo: public CObject
{
public:
	CUGGridInfo();
	virtual ~CUGGridInfo();

	//column info
	int		m_numberCols;
	int		m_currentCol;
	int		m_lastCol;
	int		m_leftCol;
	int		m_lastLeftCol;
	int		m_maxLeftCol;
	int		m_defColWidth;
	int		m_rightCol;
	int		m_dragCol;
	int	*	m_startingWidths;
	int		m_startingCols;

	UGCOLINFO*	m_colInfo;

	//row info
	long	m_numberRows;
	long	m_currentRow;
	long	m_lastRow;
	long	m_topRow;
	long	m_lastTopRow;
	long	m_maxTopRow;
	int *	m_rowHeights;
	int		m_defRowHeight;
	int		m_uniformRowHeightFlag;	//true or false
	long	m_bottomRow;
	long	m_dragRow;
	int *	m_startingHeights;
	int		m_startingRows;

	//headings
	int		m_numberTopHdgRows;
	int		*m_topHdgHeights;

	int		m_numberSideHdgCols;
	int		*m_sideHdgWidths;

	//defaults
	CUGCell *	m_gridDefaults;
	CUGCell *	m_hdgDefaults;

	//current cell
	CUGCell	*	m_currentCell;
	CUGCell		m_cell;			//general purpose cell object
		
	//sizes
	int m_topHdgHeight;		//pixels
	int m_sideHdgWidth;		//pixels
	int m_vScrollWidth;		//pixels
	int m_hScrollHeight;	//pixels
	int m_tabWidth;			//pixels

	int m_showHScroll;		//TRUE or FALSE
	int m_showVScroll;		//TRUE or FALSE

	int m_gridWidth;		//calcualated using the values above
	int m_gridHeight;

	RECT	m_gridRect;		//calcualated using the values above
	RECT	m_topHdgRect;
	RECT	m_sideHdgRect;
	RECT	m_cnrBtnRect;
	RECT	m_tabRect;
	RECT	m_vScrollRect;
	RECT	m_hScrollRect;

	//highlighting
	int		m_highlightRowFlag;		// TRUE or False
	int		m_multiSelectFlag;		// Multiselect mode
	int		m_currentCellMode;		// mode(bits) 1:focus rect 2:highlight
	BOOL	m_showFocusRect;
	BOOL	m_highLightCurrentCell;


	//other options
	int		m_mouseScrollFlag;		//TRUE or FALSE

	int		m_threeDHeight;			// 1 - n

	int		m_paintMode;			//if false then do not paint

	int		m_enablePopupMenu;		//TRUE or FALSE

	int		m_enableHints;			//TRUE or FALSE
	int		m_enableVScrollHints;	//TRUE or FALSE
	int		m_enableHScrollHints;	//TRUE or FALSE

	int		m_userSizingMode;		//0 -off 1-normal 2-update on the fly
	int		m_userBestSizeFlag;		//TRUE or FALSE

	int		m_enableJoins;			//TRUE or FALSE

	int		m_enableColSwapping;	//TRUE or FALSE

	int		m_enableCellOverLap;	//TRUE or FALSE

	int		m_enableExcelBorders;	//TRUE or FALSE

	//scrollbars
	int		m_vScrollMode;			// 0-normal 2- tracking 3-joystick
	int		m_hScrollMode;			// 0-normal 2- tracking 

	// Scroling on partially visible cells
	BOOL	m_bScrollOnParialCells;

	//balistic 
	int		m_ballisticMode;		//0- off 1-increment 2-squared 3- cubed
	int		m_ballisticDelay;		//slow scroll delay
	int		m_ballisticKeyMode;		//0- off n - number of key repeats for speed
									//increase
	int		m_ballisticKeyDelay;	//slow scroll delay
	
	//column and row locking
	int		m_numLockCols;
	int		m_numLockRows;
	int		m_lockColWidth;
	int		m_lockRowHeight;

	
	//zooming multiplication factor
	float	m_zoomMultiplier;
	BOOL	m_zoomOn;

	//allow cells to be partially visible when moved into
	BOOL m_noPartlyVisible; //TRUE = must be visible, FALSE= may not be 

	CFont *	m_defFont;
	 
	int				m_defDataSourceIndex;
	CUGDataSource*	m_defDataSource;
	CUGMem*			m_CUGMem;	


	//movement type 0-keyboard 1-lbutton 2-rbutton 3-mousemove
	int		m_moveType;		
	//flags - if moved by mouse
	UINT	m_moveFlags;


	//multi-select
	CUGMultiSelect		* m_multiSelect;

	//cursors
	HCURSOR m_arrowCursor;
	HCURSOR m_WEResizseCursor;
	HCURSOR m_NSResizseCursor;


	//edit control variables
	CWnd * m_editCtrl;
	CWnd * m_maskedEditCtrl;

	//margins for drawing text in cells
	int m_margin;	

	//overlay objects
	CUGPtrList	*m_CUGOverlay;

	//def border pen
	CPen * m_defBorderPen;

	int m_trackingWndMode; // 0-normal 1-stay close

	BOOL m_bCancelMode;

	BOOL m_bExtend;
};

#endif // _uggdinfo_H_