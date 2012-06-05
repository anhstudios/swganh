/*************************************************************************
				Class Declaration : CUGCell
**************************************************************************
	Source file : UGCell.cpp
	Header file : UGCell.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

	Purpose
		The CUGCell object is the main interface object
		used by the Ultimate Grid to interact with cells.
		Each cell object contains all information about
		a cell (its data, colors, font, celltype, etc),
		and provides functions to get and set any
		of these attributes.

		When either GetCell or GetCellIndirect functions
		are called the Ultimate Grid will collect cell's
		data from currently acrive datasource.  It is also
		possible to populate the cell object without
		a datasource, in this case we can use the OnGetCell
		notification (in the CUGCtrl derived class) to
		populate cell's data.

		Similarly the SetCell function has to be called to
		make any changes permanent.  The Ultimate Grid provides
		set of QuickSet[...] functions to simplify this
		process for some of the most common properties.

*************************************************************************/

#pragma warning (disable: 4786)

#ifndef _UGCell_H_
#define _UGCell_H_

#define UGCELL_TEXT_SET			BIT0
#define UGCELL_STRING_SET		BIT0
#define UGCELL_MASK_SET			BIT1
#define UGCELL_LABEL_SET		BIT2
#define UGCELL_DATATYPE_SET		BIT3
#define UGCELL_PARAM_SET		BIT4
#define UGCELL_CELLTYPE_SET		BIT5
#define UGCELL_CELLTYPEEX_SET	BIT6
#define UGCELL_TEXTCOLOR_SET	BIT7
#define UGCELL_BACKCOLOR_SET	BIT8
#define UGCELL_HTEXTCOLOR_SET	BIT9
#define UGCELL_HBACKCOLOR_SET	BIT10
#define UGCELL_BORDERSTYLE_SET	BIT11
#define UGCELL_BORDER_SET		BIT11
#define UGCELL_BORDERCOLOR_SET	BIT12
#define UGCELL_FONT_SET			BIT13
#define UGCELL_BITMAP_SET 		BIT14
#define UGCELL_ALIGNMENT_SET	BIT15
#define UGCELL_EXTRAMEMORY_SET	BIT16
#define UGCELL_JOIN_SET			BIT17
#define UGCELL_FORMAT_SET		BIT21
#define UGCELL_NOTUSED			BIT22
#define UGCELL_STYLE_SET		BIT23
#define UGCELL_READONLY_SET		BIT24
#define UGCELL_NUMBERDEC_SET	BIT25
#define UGCELL_DONOT_LOCALIZE	BIT26
#define UGCELL_XP_STYLE_SET		BIT27
// TD v 7.2 - for kvt xml ds changes
#define UGCELL_MULTIROWCELL		BIT31

#define UGCELLDATA_STRING		1
#define UGCELLDATA_NUMBER		2
#define UGCELLDATA_BOOL			3
#define UGCELLDATA_TIME			4
#define UGCELLDATA_CURRENCY		5

class CUGCtrl;

// the cell type enum represents any item which we may want to draw using XP themes
enum UGXPCellType {XPCellTypeData = 16, XPCellTypeTopCol = 32, XPCellTypeLeftCol = 64, XPCellTypeBorder = 128, 
XPCellTypeCombo = 256, XPCellTypeCheck = 512, XPCellTypeCheckYes = 1024, XPCellTypeCheckNo = 2048,
XPCellTypeRadio = 4096, XPCellTypeRadioYes = 8192, XPCellTypeRadioNo = 16384, XPCellTypeButton = 32768, 
XPCellTypeSpinUp = 65536, XPCellTypeSpinDown = 131072, XPCellTypeProgressSelected = 262144, XPCellTypeProgressUnselected = 524288};

// the theme state enum represents the state of cells being drawn.  ThemeStateNormal is always used for items that cannot be selected or current.
enum UGXPThemeState { ThemeStateNormal = 1, ThemeStateCurrent = 2, ThemeStateSelected = 4, ThemeStateTriState = 8, ThemeStatePressed = 16 };

class UG_CLASS_DECL CUGCell: public CObject
{
	friend class CUGMem; // So it can call ClearMemory
	friend class CUGCtrl;
protected:
	CUGCell * m_cellInitialState;

protected:

	void ClearMemory();

	unsigned long m_propSetFlags;	//one bit is used as a set/unset flag 
									//for each cell property

	bool m_useThemes;


	CUGCellFormat* m_format;//CUGCellFormat *m_format;
							//pointer to a format class
							//to validate editing and general setting
							//also used for diplay formating
	
	CUGCell * m_cellStyle;	//CUGCell *m_cellStyle;
							//pointer to a property style cell object

	// members that provide cell's data storage
	double	m_nNumber;		// numeric value set with SetNumber
	CString m_string;		// main text string set with SetText
	CString m_label;		// secondary text storage set with SetLabelText
	CString m_mask;			// mask string used by the MaskEdit control

	short	m_dataType;		//bit level flag - string,date,time,
							//time&date,number,bool,binary, currency
		
	BOOL	m_readOnlyFlag; //TRUE for readonly

	long	m_param;		//general purpose param

	int		m_numDecimals;	//number of decimal points for numbers to display
	
	int		m_cellType;		//cell type index to use
	long	m_cellTypeEx;	//extended cell type param

	COLORREF m_textColor;	//colors
	COLORREF m_backColor;
	COLORREF m_hTextColor;
	COLORREF m_hBackColor;

	short	 m_borderStyle;	//border
	CPen *	 m_borderColor;

	CFont *	 m_font;	
	CBitmap* m_bitmap;

	short	m_alignment;	//text alignment

	LPBYTE  m_extraMem;		//extra memory pointer
	long	m_extraMemSize;

	BOOL	m_joinOrigin;	//joined cells
	long	m_joinRow;		//relative position
	long	m_joinCol;
	
	bool	m_isLocked;
	int	m_openSize;
	
	UGXPCellType m_XPStyle;	// The style of this cell, from an enum above

public:

	CUGCell();
	~CUGCell();

	bool UseThemes() { return m_useThemes; }
	void UseThemes(bool use) { m_useThemes = use; }

	void ClearAll();
	void SetInitialState();
	void LoadInitialState();

	// XP style set and get
	UGXPCellType GetXPStyle() { return m_XPStyle; }
	void SetXPStyle(UGXPCellType type) { m_XPStyle = type; /*m_propSetFlags &= UGCELL_XP_STYLE_SET;*/ }

	//cell information copying functions
	int		CopyInfoTo(CUGCell *dest);
	int		CopyInfoFrom(CUGCell *source);
	int		CopyCellInfo(CUGCell *src,CUGCell *dest);

	//cell information appending funcitons - only updates items that are set
	int		AddInfoTo(CUGCell *dest);
	int		AddInfoFrom(CUGCell *source);
	int		AddCellInfo(CUGCell *src,CUGCell *dest);

	//
	BOOL	IsPropertySet(long propertyFlag);
	int		ClearPropertySetFlag(long propertyFlag);
	int		ClearProperty(long propertyFlag);
	long	GetPropertyFlags();
	int		SetPropertyFlags(long flags);

	//***** text data type functions *****
	int		SetText(LPCTSTR text);
	int		GetText(CString *text);
	LPCTSTR	GetText();
	int		AppendText(LPCTSTR text);
	int		GetTextLength();

	int		SetMask(LPCTSTR text);
	int		GetMask(CString *text);
	LPCTSTR	GetMask();

	int		SetLabelText(LPCTSTR text);
	int		GetLabelText(CString *text);
	LPCTSTR	GetLabelText();

	//***** BOOL data type functions *****
	int		SetBool(BOOL state);
	BOOL	GetBool();

	//***** Number data type functions *****
	int		SetNumber(double number);
	int		GetNumber(int *number);
	int		GetNumber(long *number);
	int		GetNumber(float *number);
	int		GetNumber(double *number);
	double	GetNumber();

	int		SetNumberDecimals(int number);
	int		GetNumberDecimals();

	//***** Time data type functions *****
	int		SetTime(int second,int minute,int hour,int day,int month,int year);
	int		GetTime(int* second,int* minute,int* hour,int* day,int* month,int* year);
	
	//***** set and get data types
	int		SetDataType(short type);
	int		GetDataType();
	
	int			SetTextColor(COLORREF color);
	COLORREF	GetTextColor();
	int			SetBackColor(COLORREF color);
	COLORREF	GetBackColor();

	int			SetHTextColor(COLORREF color);
	COLORREF	GetHTextColor();
	int			SetHBackColor(COLORREF color);
	COLORREF	GetHBackColor();

	int		SetAlignment(short align);
	short	GetAlignment();

	int		SetBorder(short style);
	short	GetBorder();
	int		SetBorderColor(CPen *pen);
	CPen* 	GetBorderColor();

	int		SetFont(CFont *font);
	CFont *	GetFont();

	int		SetBitmap(CBitmap *bitmap);
	CBitmap* GetBitmap();

	//read-only flag
	int		SetReadOnly(BOOL state);
	BOOL	GetReadOnly();

	//style type settings
	int		SetCellType(int index);
	int		GetCellType();

	int		SetCellTypeEx(long type);
	long	GetCellTypeEx();

	//extra cell information
	void *	AllocExtraMem(long len);
	void *	GetExtraMemPtr();
	long	GetExtraMemSize();
	int		ClearExtraMem();

	//join information
	int SetJoinInfo(BOOL origin,int col,long row);
	int GetJoinInfo(BOOL *origin,int *col,long *row);

	//formating and validation
	int		SetFormatClass(CUGCellFormat * format);
	CUGCellFormat*	GetFormatClass();

	int		SetCellStyle(CUGCell *cell);
	CUGCell * GetCellStyle();

	//conversion functions
	int StringToNumber(CString *string,double *number);
	int StringToBool(CString *string,BOOL * state);
	int StringToTime(CString *string,int *second,int *minute,int *hour,
		int *day,int *month,int *year);	

	//defaults
	int SetDefaultInfo();

	//set/get the general param
	int SetParam(long param);
	long GetParam();
};

#endif // _UGCell_H_