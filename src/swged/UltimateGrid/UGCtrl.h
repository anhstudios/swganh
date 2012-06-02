/*************************************************************************
				Class Declaration : CUGCtrl
**************************************************************************
	Source file : UGCtrl.cpp
	Header file : UGCtrl.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		This is the main grid's class.  It contains and
		controls all of the windows and classes that
		make up Ultimate Grid for MFC.

		This class is very important and the first class
		that any development with Ultimate Grid will encounter.
		
	Details
		- Generaly a class derived from this class
		  is used in applications. A derived class
		  is neccessary in order to handle any of the
		  notifications that the grid sends.
		- Being derived from CWnd allows the grid to be
		  used anywhere a CWnd can be used.
		- When this class is created, using CreateGrid or AttachGrid,
		  it also:
			- creates all grid's child windows
			  (CUGGrid, CUGTopHdg, CUGSideHdg, etc.)
			- creates a default memory manager datasource
			  and registers grid's standard cell types
*************************************************************************/
#ifndef _UGCtrl_H_
#define _UGCtrl_H_

// grid feature enable defines - rem out one or more of these defines 
// to remove one or more features
#define UG_ENABLE_MOUSEWHEEL
#define UG_ENABLE_PRINTING
#define UG_ENABLE_FINDDIALOG
#define UG_ENABLE_SCROLLHINTS

#ifdef __AFXOLE_H__  //OLE must be included
	#define UG_ENABLE_DRAGDROP
#endif

#ifndef WS_EX_LAYOUTRTL
	#define WS_EX_LAYOUTRTL		0x00400000L
#endif // WS_EX_LAYOUTRTL

//standard grid classes
class CUGGridInfo;
class CUGGrid;
class CUGTopHdg;
class CUGSideHdg;
class CUGCnrBtn;
class CUGVScroll;
class CUGHScroll;
class CUGCtrl;
class CUGCell;
class CUGCellType;
class CUGEdit;
class CUGCellFormat;

//standard grid includes
#include "UGDefine.h"
#include "UGStruct.h"
#include "UGPtrLst.h"
#include "UGDrwHnt.h"
#include "UGCell.h"
#include "UGCnrBtn.h"
#include "UGGrid.h"
#include "UGHScrol.h"
#include "UGSideHd.h"
#include "UGTopHdg.h"
#include "UGVScrol.h"
#include "UGDtaSrc.h"
#include "UGMemMan.h"
#include "UGCelTyp.h"
#include "UGMultiS.h"
#include "UGEditBase.h"
#include "UGEdit.h"
#include "UGMEdit.h"
#include "UGTab.h"
#include "UGGdInfo.h"
#include "UGFormat.h"
#include "UGXPThemes.h"
#ifdef UG_ENABLE_SCROLLHINTS
	#include "ughint.h"
#endif 
#ifdef UG_ENABLE_PRINTING
	#include "UGPrint.h"
#endif


//standard cell types
#include "ugLstBox.h"
#include "ugdltype.h"
#include "ugcbtype.h"
#include "ugctarrw.h"

#ifdef UG_ENABLE_DRAGDROP
	#include "UGDrgDrp.h"
#endif

/////////////////////////////////////////////////
class UG_CLASS_DECL CUGCtrl : public CWnd
{
// Construction
public:
	CUGCtrl();


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnGetDlgCode();
	afx_msg LRESULT OnCellTypeMessage(WPARAM, LPARAM);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetInitialCellStates();

	int m_contructorResults;

	// Methods used to reset the grid
	void ResetCells(int startRow, int endRow, int startCol, int endCol);
	void ResetAll(bool allSheets = true);
	void ResetSizes(int startRow, int endRow, int startCol, int endCol);

protected:

	void UseDefaultStateStorage(bool use) { m_storeInitialStates = use; }

private:
	// This method stores the startup row and col sizes in the grid info class.
	void SetInitialSizes();
	bool m_storeInitialStates;

protected:

	//***** internal classes *****
	
	//data source list
	CUGDataSource		**m_dataSrcList;
	int					m_dataSrcListLength;
	int					m_dataSrcListMaxLength;

	CUGPtrList			*m_fontList;
	CUGPtrList			*m_bitmapList;
	CUGPtrList			*m_cellTypeList;
	CUGPtrList			*m_cellStylesList;
	CUGPtrList			*m_validateList;
	CUGPtrList			*m_displayFormatList;

	//standard cell types
	CUGCellType			m_normalCellType;
	CUGDropListType		m_dropListType;
	CUGCheckBoxType		m_checkBoxType;
	CUGArrowType		m_arrowType;


	#ifdef UG_ENABLE_PRINTING
	CUGPrint*			m_CUGPrint;
	#endif

	//popup menu
	CMenu				* m_menu;			
	int					m_menuCol;
	long				m_menuRow;
	int					m_menuSection;

	CUGCell m_cell;


	//grid info list / sheet variables
	CUGGridInfo ** m_GIList;
	int m_currentSheet;
	int m_numberSheets;


	//update enable/disable flag
	BOOL m_enableUpdate;


public:

	//current sheet 
	CUGGridInfo * m_GI;

	//child window classes
	CUGGrid				*m_CUGGrid;
	CUGTopHdg			*m_CUGTopHdg;
	CUGSideHdg			*m_CUGSideHdg;
	CUGCnrBtn			*m_CUGCnrBtn;

	CUGVScroll			*m_CUGVScroll;
	CUGHScroll			*m_CUGHScroll;

	//scroll hint window
	#ifdef UG_ENABLE_SCROLLHINTS
	CUGHint				*m_CUGHint;
	#endif
	
	//tabs
	CUGTab				*m_CUGTab;
	//tab sizing flag
	BOOL m_tabSizing;

	//tracking topmost window
	CWnd				*m_trackingWnd;

	//default edit control
	CUGEdit			m_defEditCtrl;
	CUGMaskedEdit	m_defMaskedEditCtrl;

	//editing
	BOOL	m_editInProgress;		//TRUE or FALSE
	long	m_editRow;
	int		m_editCol;
	CWnd *	m_editCtrl;				//edit control currently being used
	CUGCell m_editCell;
	CWnd *	m_editParent;

	//***** other variables *****
	CPen m_threeDLightPen;
	CPen m_threeDDarkPen;


protected:

	//***** internal functions *****
	void CalcTopRow();			//Calcs the max top row then adjusts the top row if needed
	void CalcLeftCol();			//Calcs the max left col then adjusts the left col if needed
	void AdjustTopRow();		//moves top row so that the current row is in view
	void AdjustLeftCol();		//move the left col so that the current col is in view
	void Update();				//updates all windows - also performs recalculations
	void Moved();				//this function is called whenever a grid movement is made
								//this function then notifies the child windows
	
	BOOL CreateChildWindows();	//creates the child windows
	void ToggleLayout( CWnd *pWnd );

	void SetLockRowHeight();
	void SetLockColWidth();

	void MoveTrackingWindow();

public:

	//check to see if the new position is valid
	int VerifyTopRow(long* newRow);
	int VerifyCurrentRow(long* newRow);
	int VerifyLeftCol(int* newCol);
	int VerifyCurrentCol(int* newCol);


	//*************** creation/setup *****************
	//window creation
	BOOL CreateGrid(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//dialog resource functions
	BOOL AttachGrid(CWnd * wnd,UINT ID);

	void AdjustComponentSizes();	//adjusts and positions the child windows

	//*************** editing *****************
	int		StartEdit();
	int		StartEdit(int key);
	int		StartEdit(int col,long row,int key);
	int		ContinueEdit(int adjustCol,long adjustRow);

	//*************** row and column *****************
	int		SetNumberRows(long rows,BOOL redraw = TRUE);
	long	GetNumberRows();
	
	int		SetNumberCols(int cols,BOOL redraw = TRUE);
	int		GetNumberCols();

	int		SetDefColWidth(int width);
	int		SetColWidth(int col,int width);
	int		GetColWidth(int col,int *width);
	int		GetColWidth(int col);

	int		SetDefRowHeight(int height);
	int		SetUniformRowHeight(int flag);
	int		SetRowHeight(long row,int height);
	int		GetRowHeight(long row,int *height);
	int		GetRowHeight(long row);

	int		GetCurrentCol();
	long	GetCurrentRow();
	int		GetLeftCol();
	long	GetTopRow();
	int		GetRightCol();
	long	GetBottomRow();

	int		InsertCol(int col);
	int		AppendCol();
	int		DeleteCol(int col);
	int		InsertRow(long row);
	int		AppendRow();
	int		DeleteRow(long row);

	//************ heading functions *************
	int		SetTH_NumberRows(int rows);
	int		SetTH_RowHeight(int row,int height);
	int		SetSH_NumberCols(int cols);
	int		SetSH_ColWidth(int col,int width);


	//************* find/replace/sort *********************
	int FindFirst(CString *string,int *col,long *row,long flags);
	int FindNext(CString *string,int *col,long *row,int flags);

	int FindDialog();
	int ReplaceDialog();
	long ProcessFindDialog(UINT,long);
	int FindInAllCols(BOOL state);
	BOOL m_findDialogRunning;
	BOOL m_findDialogStarted;
	BOOL m_findInAllCols;
	CFindReplaceDialog *m_findReplaceDialog;


	int SortBy(int col,int flag = UG_SORT_ASCENDING);
	int SortBy(int *cols,int num,int flag = UG_SORT_ASCENDING);


	//************* child windows *********************
	int		SetTH_Height(int height);
	int		GetTH_Height();
	int		SetSH_Width(int width);
	int		GetSH_Width();
	int		SetVS_Width(int width);
	int		GetVS_Width();
	int		SetHS_Height(int height);
	int		GetHS_Height();

	//************* default/cell information *********************
	int		GetCell(int col,long row,CUGCell *cell);
	int		GetCellIndirect(int col,long row,CUGCell *cell);
	int		SetCell(int col,long row,CUGCell *cell);
	int		DeleteCell(int col,long row);

	int		SetColDefault(int col,CUGCell *cell);
	int		GetColDefault(int col,CUGCell *cell);
	int		SetGridDefault(CUGCell *cell);
	int		GetGridDefault(CUGCell *cell);
	int		SetHeadingDefault(CUGCell *cell);
	int		GetHeadingDefault(CUGCell *cell);

	//column information and translation
	int		GetColTranslation(int col);
	int		SetColTranslation(int col,int transCol);


	//cell joining
	int		JoinCells(int startcol,long startrow,int endcol,long endrow);
	int		UnJoinCells(int col,long row);
	int		EnableJoins(BOOL state);

	//cell functions
	int		QuickSetText(int col,long row,LPCTSTR string);
	int		QuickSetNumber(int col,long row,double number);
	int		QuickSetMask(int col,long row,LPCTSTR string);
	int		QuickSetLabelText(int col,long row,LPCTSTR string);
	int		QuickSetTextColor(int col,long row,COLORREF color);
	int		QuickSetBackColor(int col,long row,COLORREF color);
	int		QuickSetAlignment(int col,long row,short align);
	int		QuickSetBorder(int col,long row,short border);
	int		QuickSetBorderColor(int col,long row,CPen *pen);
	int		QuickSetFont(int col,long row,CFont * font);
	int		QuickSetFont(int col,long row,int index);
	int		QuickSetBitmap(int col,long row,CBitmap * bitmap);
	int		QuickSetBitmap(int col,long row,int index);
	int		QuickSetCellType(int col,long row,long type);
	int		QuickSetCellTypeEx(int col,long row,long typeEx);
	int		QuickSetHTextColor(int col,long row,COLORREF color);
	int		QuickSetHBackColor(int col,long row,COLORREF color);

	int		QuickSetRange(int startCol,long startRow,int endCol,long endRow,
				CUGCell* cell);

	int			QuickGetText(int col,long row,CString *string);
	LPCTSTR		QuickGetText(int col,long row);

	int		DuplicateCell(int destCol,long destRow, int srcCol, long srcRow);


	//************* general modes and settings *********************
	int	SetCurrentCellMode(int mode);	//focus and highlighting
											//bit 1:focus 2:hightlight
	int	SetHighlightRow(int mode, BOOL bExtend = TRUE);		//on,off
	int	SetMultiSelectMode(int mode);	//on,off
	int	Set3DHeight(int height);		//in pixels
	int	SetPaintMode(int mode);			//on,off
	int	GetPaintMode();
	int	SetVScrollMode(int mode);		// 0-normal 2- tracking 3-joystick
	int	SetHScrollMode(int mode);		// 0-normal 2- tracking
	
	int HideCurrentCell();

	int SetBallisticMode(int mode);	// 0:off  1:on  2:on-squared 3:on-cubed
	int SetBallisticDelay(int milisec);

	int SetBallisticKeyMode(int mode); //0-off n-number of key repeats before increase
	int SetBallisticKeyDelay(int milisec);
	
	int SetDoubleBufferMode(int mode);	// 0:off 1:on
	int SetGridLayout( int layoutMode );	// 0: Left-to-Right (default),
											// 1: Right-to-Left

	int LockColumns(int numCols);		
	int	LockRows(int numRows);

	int	EnableCellOverLap(BOOL state);
	int EnableColSwapping(BOOL state);
	int EnableExcelBorders(BOOL state);
	int EnableScrollOnParialCells( BOOL state );


	//************* movement *********************
	int MoveTopRow(int flag);	//0-lineup 1-linedown 2-pageup 3-pagedown 4-top 5-bottom
	int AdjustTopRow(long adjust);
	int MoveCurrentRow(int flag);
	int AdjustCurrentRow(long adjust);
	int GotoRow(long row);
	int	SetTopRow(long row);

	int MoveLeftCol(int flag);
	int AdjustLeftCol(int adjust);
	int MoveCurrentCol(int flag);
	int AdjustCurrentCol(int adjust);
	int GotoCol(int col);
	int	SetLeftCol(int col);

	int GotoCell(int col,long row);


	//************* finding cells *********************
	int GetCellFromPoint(int x,int y,int *col,long *row);
	int GetCellFromPoint(int x,int y,int *ptcol,long *ptrow,RECT *rect);
	int GetAbsoluteCellFromPoint(int x,int y,int *ptcol,long *ptrow);

	int GetCellRect(int col,long row,RECT *rect);
	int GetCellRect(int *col,long *row,RECT *rect);
	int GetRangeRect(int startCol,long startRow,int endCol,long endRow,RECT *rect);
	
	int GetJoinStartCell(int *col,long *row);
	int GetJoinStartCell(int *col,long *row,CUGCell *cell);
	int GetJoinRange(int *col,long *row,int *col2,long *row2);


	//************* cell types *********************
	long AddCellType(CUGCellType *);   //returns the cell type ID
	int RemoveCellType(int ID);
	CUGCellType * GetCellType(int type);	//returns the pointer to a cell type
	int GetCellType(CUGCellType * type);

	//************* data sources *********************
	int AddDataSource(CUGDataSource * ds);
	CUGDataSource * GetDataSource(int index);
	int RemoveDataSource(int index);
	int SetDefDataSource(int index);
	int GetDefDataSource();
	int SetGridUsingDataSource(int index);

	//************* fonts *********************
	int AddFont(LPCTSTR fontName,int height,int weight);
	int AddFont(int height,int width,int escapement,int orientation, 
			int weight,BYTE italic,BYTE underline,BYTE strikeOut, 
			BYTE charSet,BYTE outputPrecision,BYTE clipPrecision, 
			BYTE quality,BYTE pitchAndFamily,LPCTSTR fontName);
	int AddFontIndirect( LOGFONT lgFont );
	int RemoveFont(int index);
	int ClearAllFonts();
	CFont * GetFont(int index);
	int SetDefFont(CFont *font);
	int SetDefFont(int index);

	//************* bitmaps *********************
	int AddBitmap( UINT resourceID,LPCTSTR resourceName= NULL);
	int AddBitmap( LPCTSTR fileName);
	int RemoveBitmap(int index);
	int ClearAllBitmaps();
	CBitmap* GetBitmap(int index);


	//************* redrawing *********************
	int RedrawAll();
	int RedrawCell(int col,long row);
	int RedrawRow(long row);
	int RedrawCol(int col);
	int RedrawRange(int startCol,long startRow,int endCol,long endRow);
	void TempDisableFocusRect();


	//************* multi-select *********************
	int ClearSelections();
	int Select(int col,long row);
	int SelectRange(int startCol,long startRow,int endCol,long endRow);
	int EnumFirstSelected(int *col,long *row);
	int EnumNextSelected(int *col,long *row);
	int EnumFirstBlock(int *startCol,long *startRow,int *endCol,long *endRow);
	int EnumNextBlock(int *startCol,long *startRow,int *endCol,long *endRow);
	BOOL IsSelected(int col,long row,int *blockNum = NULL);

	//************* clipboard ********************
	int CopySelected();
	int CutSelected();
	int CopySelected(int cutflag);  //TRUE,FALSE
	int Paste();
	int Paste(CString &string);
	int CopyToClipBoard(CString* string);
	int CopyFromClipBoard(CString* string);
	void CreateSelectedString(CString& string,int cutFlag);


	//************* column sizing ********************
	virtual int FitToWindow(int startCol,int endCol);
	virtual int BestFit(int startCol,int endCol,int CalcRange,int flag);


	//************* printing ********************
	#ifdef UG_ENABLE_PRINTING
		int PrintInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
			int endCol,long endRow);
		int PrintPage(CDC * pDC, int pageNum);
		int PrintSetMargin(int whichMargin,int size);
		int PrintSetScale(int scalePercent);
		int PrintSetOption(int option,long param);
		int PrintGetOption(int option,long *param);
	#endif


	//************* hints ********************
	int UseHints(BOOL state);
	int UseVScrollHints(BOOL state);
	int UseHScrollHints(BOOL state);


	//************* pop-up menu ********************
	CMenu * GetPopupMenu();
	int EmptyMenu();
	int AddMenuItem(int ID,LPCTSTR string);
	int RemoveMenuItem(int ID);
	int EnableMenu(BOOL state);


	//************* drag and drop ********************
	#ifdef UG_ENABLE_DRAGDROP
		COleDataSource	m_dataSource;
		CUGDropTarget	m_dropTarget;

		int StartDragDrop();
		int DragDropTarget(BOOL state);
	#endif


	//************* tabs ********************
	int AddTab( CString label, long ID );
	int InsertTab( int pos, CString label, long ID );
	int DeleteTab( long ID );
	int SetTabBackColor( long ID, COLORREF color );
	int SetTabTextColor( long ID, COLORREF color );
	int SetTabWidth( int width );
	int SetCurrentTab( long ID );
	int GetCurrentTab();


	//************* sheets ********************
	int SetNumberSheets(int numSheets);
	int GetNumberSheets();
	int SetSheetNumber(int index,BOOL update = TRUE);
	int GetSheetNumber();

	//****************************************************
	//*********** Over-ridable Notify Functions **********
	//****************************************************
	virtual void OnSetup();
	virtual void OnReset();
	virtual void OnSheetSetup(int sheetNumber);

	//movement and sizing
	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual int  OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual void OnHitBottom(long numrows,long rowspast,long rowsfound);
	virtual void OnHitTop(long numrows,long rowspast);
	
	virtual int  OnCanSizeCol(int col);
	virtual void OnColSizing(int col,int *width);
	virtual void OnColSized(int col,int *width);
	virtual int  OnCanSizeRow(long row);
	virtual void OnRowSizing(long row,int *height);
	virtual void OnRowSized(long row,int *height);

	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnTopHdgSizing(int *height);
	virtual int  OnSideHdgSizing(int *width);
	virtual int  OnTopHdgSized(int *height);
	virtual int  OnSideHdgSized(int *width);
	
	virtual void OnColChange(int oldcol,int newcol);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual void OnLeftColChange(int oldcol,int newcol);
	virtual void OnTopRowChange(long oldrow,long newrow);
	virtual void OnViewMoved( int nScrolDir, long oldPos, long newPos );
	virtual void OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum);

	//mouse and key strokes
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed=0);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_DClicked(RECT *rect,POINT *point,BOOL processed=0);
	
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual void OnKeyUp(UINT *vcKey,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	
	//GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);

	//SetCell notification
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	
	//data source notifications
	virtual void OnDataSourceNotify(int ID,long msg,long param);

	//cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param);

	//editing
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);
	virtual int OnEditKeyDown(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditKeyUp(int col,long row,CWnd *edit,UINT *vcKey);

	//menu notifications
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual int  OnMenuStart(int col,long row,int section);

	// Context help 
	virtual DWORD OnGetContextHelpID( int col, long row, int section );

	//hints
	virtual int OnHint(int col,long row,int section,CString *string);
	virtual int OnVScrollHint(long row,CString *string);
	virtual int OnHScrollHint(int col,CString *string);

	//drag and drop
	#ifdef UG_ENABLE_DRAGDROP
		virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
		virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,int col,long row);
		virtual DROPEFFECT OnDragDrop(COleDataObject* pDataObject,int col,long row);
	#endif

	//sorting
	virtual int  OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	//DC setup
	virtual void OnScreenDCSetup(CDC *dc,CDC *db_dc,int section);
	
	virtual void OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs);

	virtual void OnTabSelected(int ID);

	virtual COLORREF OnGetDefBackColor(int section);

	//focus rect setup
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);

	virtual void OnSetFocus(int section);
	virtual void OnKillFocus(int section);
	virtual void OnKillFocus(int section, CWnd *pNewWnd);
	
	void DrawExcelFocusRect(CDC *dc,RECT *rect);
		
	int StartMenu(int col,long row,POINT *point,int section);

	int SetArrowCursor(HCURSOR cursor);
	int SetWESizingCursor(HCURSOR cursor);
	int SetNSSizingCursor(HCURSOR cursor);

	int SetMargin(int pixels);

	//functions to be called by grid edit controls
	int EditCtrlFinished(LPCTSTR string,BOOL cancelFlag,
		BOOL continueFlag,int continueCol,long continueRow);

	int EditCancel();

	int SetCancelMode( BOOL bCancel );
	BOOL GetCancelMode();

	CUGCellType * GetCellType(int col,long row);

	virtual BOOL OnColSwapStart(int col);
	virtual BOOL OnCanColSwap(int fromCol,int toCol);
	virtual void OnColSwapped(int fromCol,int toCol);

	int MoveColPosition(int fromCol,int toCol,BOOL insertBefore);

	int SetNewTopHeadingClass(CUGTopHdg * topHeading);
	int SetNewSideHeadingClass(CUGSideHdg * sideHeading);
	int SetNewGridClass(CUGGrid * grid);
	int SetNewMultiSelectClass(CUGMultiSelect * multiSelect);
	int SetNewTabClass(CUGTab * tab);
	int SetNewVScrollClass(CUGVScroll * scroll);
	int SetNewHScrollClass(CUGHScroll * scroll);


	int SetNewEditClass(CWnd * edit);
	int SetNewMaskedEditClass(CWnd * edit);
	CWnd * GetEditClass();
	CWnd * GetMaskedEditClass();

	//tracking window
	int SetTrackingWindow(CWnd *wnd);
	int SetTrackingWindowMode(int mode);
	virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);

	int EnableUpdate(BOOL state);

	int SetUserSizingMode(int state);

	int SetColDataSource(int col,int dataSrcIndex);
	int SetColDataSource(int col,CUGDataSource * dataSrc);

	static BOOL CALLBACK ModifyDlgItemText(HWND hWnd, LPARAM lParam);

	// This function returns the m_CUGPrint.
	// It is used in printing cells.
	#ifdef UG_ENABLE_PRINTING
	inline	CUGPrint* GetUGPrint (void) {
		return m_CUGPrint;
	}
	#endif
};

#endif // _UGCtrl_H_
