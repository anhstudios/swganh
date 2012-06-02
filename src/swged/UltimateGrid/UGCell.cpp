// ==========================================================================
// 					Class Implementation : CUGCell
// ==========================================================================
// Source file : UGCell.cpp
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// ==========================================================================

#include "stdafx.h"
#include <ctype.h>
#include "UGCtrl.h"
 //#include "UGCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/********************************************
Constructor
	Inits all of the cell variables (properties)
	Calls ClearAll which clears all cell properties
*********************************************/
CUGCell::CUGCell()
{
	m_useThemes = true;
	m_extraMem = NULL;
	m_cellInitialState = NULL;
	ClearAll();
}
/********************************************
Destructor
	Performs clean-up
*********************************************/
CUGCell::~CUGCell()
{
	//perform clean up
	ClearAll();
}

/********************************************
ClearInfo
	Purpose
		Sets all the cell information properties to
		an "unused" state
	Params
		none
	Return
		none
*********************************************/
void CUGCell::ClearAll()
{
	SetDefaultInfo();

	// restore default border settings
	m_borderStyle = 0;
	m_borderColor = NULL;

	// clear the font pointer
	m_font = NULL;

	// clear the bitmap pointer
	m_bitmap = NULL;

	// clear celltype information
	m_cellType		= -1;
	m_cellTypeEx	= -1;

	// clear joins information
	m_joinOrigin	= FALSE;
	m_joinRow		= -1;
	m_joinCol		= -1;

	// Clear locking info
	m_isLocked = false;
	m_openSize = 0;
}
/********************************************
SetDefaultInfo
	Purpose
		Sets up the cell with default information
		NOTE: when a cell is created there is no
		default information
	Params
		none
	Return
		UG_SUCCESS - success
		UG_ERROR - error
*********************************************/
int CUGCell::SetDefaultInfo()
{
	m_propSetFlags	=	UGCELL_DATATYPE_SET |
						UGCELL_TEXTCOLOR_SET |
						UGCELL_BACKCOLOR_SET |
						UGCELL_HTEXTCOLOR_SET |
						UGCELL_HBACKCOLOR_SET |
						UGCELL_XP_STYLE_SET |
						UGCELL_ALIGNMENT_SET;

	m_string		= _T("");
	m_mask			= _T("");
	m_label			= _T("");
	m_format		= NULL;
	m_cellStyle		= NULL;
	m_cellInitialState = NULL;
	// numberic datatype specific information
	m_nNumber		= 0;
	m_numDecimals	= 0;

	// clear datatype
	m_dataType = UGCELLDATA_STRING;
	// clear the cell's param value
	m_param = -1;
	// clear the read only flag
	m_readOnlyFlag = FALSE;

	m_textColor		= GetSysColor(COLOR_WINDOWTEXT);
	m_backColor		= GetSysColor(COLOR_WINDOW);
	m_hTextColor	= GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_hBackColor	= GetSysColor(COLOR_HIGHLIGHT);

	m_alignment		= UG_ALIGNLEFT;

	m_XPStyle = XPCellTypeData;
	
	return UG_SUCCESS;
}

/********************************************
CopyInfoTo
	Purpose
		Copies all the cell information to the given
		destination cell
	Params
		dest - destination cell to copy properties to
	return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::CopyInfoTo(CUGCell *dest){
	
	//property flags
	dest->m_propSetFlags = m_propSetFlags;

	return AddCellInfo(this,dest);
}
/********************************************
CopyInfoFrom
	Purpose
		Copies all the cell information from the given
		source cell
	Params
		src - destination cell to copy properties from
	return 
		UG_SUCCESS	success
		UG_ERROR    fail
*********************************************/
int	CUGCell::CopyInfoFrom(CUGCell *src){

	//property flags
	m_propSetFlags = src->m_propSetFlags;

	return AddCellInfo(src,this);
}
/*****************************************************
CopyCellInfo
	Purpose
		Copies all the cell information from the given
		source to the destination cell 
	Params
		src	 - cell to copy properties from
		dest - cell to copy properties to
	return 
		UG_SUCCESS	- success
		UG_ERROR	- fail
*****************************************************/
int CUGCell::CopyCellInfo(CUGCell *src,CUGCell *dest){

	//property flags
	dest->m_propSetFlags = src->m_propSetFlags;

	return AddCellInfo(src,dest);
}

/********************************************
AddInfoTo
	Purpose
		Copies only the properties that are set
		from this cell to the destiination cell.
		NOTE: all properties have a set and unset 
		state. Their state is detirmined by a bit
		int the m_propSetFlags member variable
	Params
		dest -	cell that the set properties are
				copied to
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::AddInfoTo(CUGCell *dest){
	return AddCellInfo(this,dest);
}

/********************************************
AddInfoFrom
	Purpose
		Copies only the properties that are set
		in the source cell to this cell object.
		NOTE: all properties have a set and unset 
		state. Their state is detirmined by a bit
		int the m_propSetFlags member variable
	Params
		src -	cell that the set properties are
				copied from
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::AddInfoFrom(CUGCell *src){
	return AddCellInfo(src,this);
}
/*****************************************************
AddCellInfo
	Purpose
		Copies only the properties that are set
		in the source cell to the dest cell object.
		NOTE: all properties have a set and unset 
		state. Their state is detirmined by a bit
		int the m_propSetFlags member variable
	Params
		src -	cell that the set properties are
				copied from
		dest -	cell that the set properties are
				copied to
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*****************************************************/
int CUGCell::AddCellInfo(CUGCell *src,CUGCell *dest)
{
	dest->UseThemes(src->UseThemes());

	//property flags
	dest->m_propSetFlags |= src->m_propSetFlags;

	//String 
	if(src->m_propSetFlags&UGCELL_STRING_SET)
	{	// primary text string
		dest->m_string = src->m_string;
		// number
		dest->m_nNumber = src->m_nNumber;
	}

	//mask
	if(src->m_propSetFlags&UGCELL_MASK_SET)
		dest->m_mask = src->m_mask;

	//label
	if(src->m_propSetFlags&UGCELL_LABEL_SET)
		dest->m_label = src->m_label;

	//format and style class pointers
	if(src->m_propSetFlags&UGCELL_FORMAT_SET)
		dest->m_format = src->m_format;
	if(src->m_propSetFlags&UGCELL_STYLE_SET)
		dest->m_cellStyle = src->m_cellStyle;

	//data type
	if(src->m_propSetFlags&UGCELL_DATATYPE_SET)
		dest->m_dataType = src->m_dataType;

	//general purpose value
	if(src->m_propSetFlags&UGCELL_PARAM_SET)
		dest->m_param = src->m_param;
	
	//cell type
	if(src->m_propSetFlags&UGCELL_CELLTYPE_SET)
		dest->m_cellType = src->m_cellType;
	if(src->m_propSetFlags&UGCELL_CELLTYPEEX_SET)	
		dest->m_cellTypeEx = src->m_cellTypeEx;

	//colors
	if(src->m_propSetFlags&UGCELL_TEXTCOLOR_SET)
		dest->m_textColor = src->m_textColor;
	if(src->m_propSetFlags&UGCELL_BACKCOLOR_SET)
		dest->m_backColor = src->m_backColor;
	if(src->m_propSetFlags&UGCELL_HTEXTCOLOR_SET)
		dest->m_hTextColor = src->m_hTextColor;
	if(src->m_propSetFlags&UGCELL_HBACKCOLOR_SET)
		dest->m_hBackColor = src->m_hBackColor;

	//borders
	if(src->m_propSetFlags&UGCELL_BORDERSTYLE_SET)
		dest->m_borderStyle = src->m_borderStyle;
	if(src->m_propSetFlags&UGCELL_BORDERCOLOR_SET)
		dest->m_borderColor = src->m_borderColor;

	//font and bitmap
	if(src->m_propSetFlags&UGCELL_FONT_SET)
		dest->m_font = src->m_font;
	if(src->m_propSetFlags&UGCELL_BITMAP_SET)
		dest->m_bitmap = src->m_bitmap;

	//alignment
	if(src->m_propSetFlags&UGCELL_ALIGNMENT_SET)
		dest->m_alignment = src->m_alignment;

	// XP Stype
	if(src->m_propSetFlags&UGCELL_XP_STYLE_SET)
		dest->m_XPStyle = src->m_XPStyle;

	
	//extra memory
	// This has been changed so that it copies the pointers only.  
	// The grid will clear the memory as it closes, using the ClearMemory function below.
	// The old code would only work if no non pointer CUGCells were used to get/set cells in code.
	if(src->m_propSetFlags&UGCELL_EXTRAMEMORY_SET)
	{
		dest->m_extraMem = src->m_extraMem;
		dest->m_extraMemSize = src->m_extraMemSize;
	}
	else
	{
		src->m_extraMem = dest->m_extraMem = NULL;
	}

	//join cells
	if(src->m_propSetFlags&UGCELL_JOIN_SET){
		dest->m_joinOrigin = src->m_joinOrigin;
		dest->m_joinCol = src->m_joinCol;
		dest->m_joinRow = src->m_joinRow;
	}

	//decimals
	if(src->m_propSetFlags&UGCELL_NUMBERDEC_SET)
		dest->m_numDecimals = src->m_numDecimals;

	//read only
	if(src->m_propSetFlags&UGCELL_READONLY_SET)
		dest->m_readOnlyFlag = src->m_readOnlyFlag;

	// initial state
	dest->m_cellInitialState = src->m_cellInitialState;

	return UG_SUCCESS;
}

/********************************************
//********* text data type functions ********
*********************************************/

/********************************************
SetText
	Purpose
		Sets the text property of a cell object.
		The text property is also the 'data' 
		property of a cell
	Params
		text - pointer to a string
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetText(LPCTSTR text)
{	
	m_string = text;

	m_dataType = UGCELLDATA_STRING;

	if ( m_string.GetLength() > 0 )
		m_propSetFlags |= (UGCELL_STRING_SET+UGCELL_DATATYPE_SET);
	else
		m_propSetFlags &= ~UGCELL_STRING_SET;

	return UG_SUCCESS;
}
/********************************************
AppendText
	Purpose
		Appends text to the cells text property
	Params
		text - pointer to a string
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::AppendText(LPCTSTR text){
	
	if(( m_propSetFlags & UGCELL_STRING_SET ) == 0 )
		return UG_ERROR;

	m_string += text;
	m_dataType	= UGCELLDATA_STRING;
	m_propSetFlags |= (UGCELL_STRING_SET +UGCELL_DATATYPE_SET);

	return UG_SUCCESS;
}
/********************************************
GetText
	Purpose
		Returns the text from the cells text
		property.
	Params
		text - pointer to a CString object to 
			receive the text
	Return 
		UG_SUCCESS	success
		UG_ERROR	this function will never fail
*********************************************/
int	CUGCell::GetText(CString *text)
{
	if( m_propSetFlags&UGCELL_STRING_SET )
	{
		*text = GetText();
		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/********************************************
GetText
	Purpose
		The GetText function will return a text
		representation of cell's data, if current
		data type is other than string than data
		will be converted using current format settings.

		For numbers we introduced a way to retrieve
		localized or plain number representation, this
		mode can be controled using the UGCELL_DONOT_LOCALIZE
		flag.  If it is set than numbers will be retured as plain,
		by default all numeric data will be localized.
	Params
		none
	Return 
		pointer to the text object
*********************************************/
LPCTSTR	CUGCell::GetText()
{
	if (!( m_propSetFlags&UGCELL_STRING_SET ))
	{	// if a value is not set to a cell than return NULL
		return NULL;
	}
	else if ( m_dataType == UGCELLDATA_NUMBER )
	{
		CString strDouble;
		// parse the string without localized formatting
		m_string = "";
		if( m_propSetFlags&UGCELL_NUMBERDEC_SET )
		{

			CString formatStr = _T("%lf");
			formatStr.Format(_T("%%1.%dlf"), m_numDecimals ); //
		strDouble.Format( formatStr, m_nNumber );
		}
		else
		{
			strDouble.Format(_T("%.20f"), m_nNumber );
			strDouble.TrimRight(_T('0'));
		}

		if(!( m_propSetFlags&UGCELL_NUMBERDEC_SET ))
		{
			strDouble.TrimLeft(_T('0'));
			strDouble.TrimRight(_T('0'));
		}

		// if the number ends with a period, than remove it
		if ( strDouble.Right( 1 ) == _T('.'))
			strDouble.TrimRight( _T('.'));

		if ( strDouble == _T("") && m_propSetFlags&UGCELL_STRING_SET )
			strDouble = _T("0");

		// create a 'proper' localized string representation of the numeric value
		if (!( m_propSetFlags&UGCELL_DONOT_LOCALIZE ))
		{
			TCHAR szBuffer[256];
			NUMBERFMT nformat;

			// Determine the number of decimal places
			if (m_propSetFlags & UGCELL_NUMBERDEC_SET)
				nformat.NumDigits = m_numDecimals;
			else
			{
				int iDotIndex = strDouble.ReverseFind('.');
				if (iDotIndex == -1)
					nformat.NumDigits = 0;
				else
					nformat.NumDigits = strDouble.GetLength() - iDotIndex - 1;
			}

			// Set up the rest of the members of NUMBERFMT
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO|LOCALE_RETURN_NUMBER,
				(TCHAR*)&nformat.LeadingZero, sizeof(nformat.LeadingZero));

			nformat.Grouping = 3;

			TCHAR szDecimalSep[4];
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecimalSep, 4);
			nformat.lpDecimalSep = szDecimalSep;

			TCHAR szThousandSep[4];
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandSep, 4);
			nformat.lpThousandSep = szThousandSep;

			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER|LOCALE_RETURN_NUMBER,
				(TCHAR*)&nformat.NegativeOrder, sizeof(nformat.NegativeOrder));

			// Format the buffer
			int iRes = ::GetNumberFormat(LOCALE_USER_DEFAULT, 0, strDouble, &nformat, szBuffer, 256); 
			if (iRes == 0)
			{
				// Too many decimal digits, so GetNumberFormat failed - let's format manually
				strDouble.Replace(_T("."), szDecimalSep);
			}
			else
				strDouble = szBuffer;
		}

		m_string = strDouble;
	}

	return m_string;
}
/********************************************
GetTextLength
	Purpose
		Returns the length of the cell's text
		property.
	Params
		none
	Return
		the length of the text properties text
*********************************************/
int	CUGCell::GetTextLength(){

	if(m_propSetFlags&UGCELL_STRING_SET)
		return m_string.GetLength();
	return 0;
}
/********************************************
SetMask
	Purpose
		Sets the mask property of a cell object.
	Params
		text - pointer to a string
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetMask(LPCTSTR text){

	m_mask = text;
	m_propSetFlags |= UGCELL_MASK_SET;

	return UG_SUCCESS;
}
/********************************************
GetMask
	Purpose
		Returns the mask text from the cells mask
		property.
	Params
		text - pointer to a CString object to 
			receive the mask
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetMask(CString *text){

	if(m_propSetFlags&UGCELL_MASK_SET){
		*text = m_mask;
		return UG_SUCCESS;
	}
	return UG_ERROR;
}
/********************************************
GetMask
	Purpose
		Returns the mask text from the cells mask
		property.
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return 
		LPCTSTR to the mask string
*********************************************/
LPCTSTR	CUGCell::GetMask(){

	if(m_propSetFlags&UGCELL_MASK_SET)
		return (LPCTSTR)m_mask;
	return NULL;
}
/********************************************
SetLabelText
	Purpose
		Sets the label text property of a cell 
		object.
	Params
		text - pointer to a string
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetLabelText(LPCTSTR text)
{
	m_label = text;
	m_propSetFlags |= UGCELL_LABEL_SET;

	return UG_SUCCESS;
}
/********************************************
GetLabelText
	Purpose
		Returns the label text from the cells label
		property.
	Params
		text - pointer to a CString object to 
			receive the label text
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetLabelText(CString *text){

	if(m_propSetFlags&UGCELL_LABEL_SET){
		*text = m_label;
		return UG_SUCCESS;
	}
	return UG_ERROR;
}
/********************************************
GetLabelText
	Purpose
		Returns the label text from the cells label
		property.
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return 
		pointer to the label text
*********************************************/
LPCTSTR	CUGCell::GetLabelText(){

	if(m_propSetFlags&UGCELL_LABEL_SET)
		return (LPCTSTR)m_label;
	return NULL;
}

/********************************************
	//***** BOOL data type functions *****
*********************************************/

/********************************************
SetBool
	Purpose
		Sets the booleen state for the cell.
		This data is stored in the cells text
		property, which is also its data property.
	Params
		state - boolean state to set the cell to
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetBool(BOOL state)
{
	if (state)
		m_string = _T("1");
	else
		m_string = _T("0");

	m_dataType	= UGCELLDATA_BOOL;
	m_propSetFlags |= (UGCELL_STRING_SET + UGCELL_DATATYPE_SET);

	return UG_SUCCESS;
}
/********************************************
GetBool
	Purpose
		Returns the booleen state for the cell.
		This information is retrieved from the
		cell's text property.  This function does 
		not check to see if the proprty is set, 
		so make sure to use the IsPropertySet 
		function first.
	Params
		none
	Return 
		The boolean state of the cell
*********************************************/
BOOL CUGCell::GetBool()
{
	//get the bool
	if ( GetNumber() > 0 )
		return TRUE;
	else if ( m_string.GetLength() > 0 )
	{
		if( m_string.GetAt(0) == _T('T') || m_string.GetAt(0) == _T('t'))
			return TRUE;
	}

	return FALSE;
}
/********************************************
	//***** Number data type functions *****
*********************************************/

/********************************************
SetNumber
	Purpose
		Sets the number value for the cell. The
		number value is stored in the cell's 
		text property
	Params
		number - number to set 
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetNumber(double number)
{
	m_string = "";
	m_nNumber = number;

	m_dataType		= UGCELLDATA_NUMBER;
	m_propSetFlags |= (UGCELL_STRING_SET + UGCELL_DATATYPE_SET);

	return UG_SUCCESS;
}
/********************************************
GetNumber
	Purpose
		Returns the number value for the cell.
		This number is retrieved from the cell's
		text property.
	Params
		number - pointer to an int to recieve 
			the value
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetNumber(int *number)
{
	if( m_propSetFlags&UGCELL_STRING_SET )
	{
		*number = (int)GetNumber();
		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/********************************************
GetNumber
	Purpose
		Returns the number value for the cell.
		This number is retrieved from the cell's
		text property.
	Params
		number - pointer to a long to recieve 
			the value
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetNumber(long *number)
{
	if( m_propSetFlags&UGCELL_STRING_SET )
	{
		*number = (long)GetNumber();
		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/********************************************
GetNumber
	Purpose
		Returns the number value for the cell.
		This number is retrieved from the cell's
		text property.
	Params
		number - pointer to a float to recieve 
			the value
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetNumber(float *number)
{
	if( m_propSetFlags&UGCELL_STRING_SET )
	{
		*number = (float)GetNumber();
		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/********************************************
GetNumber
	Purpose
		Returns the number value for the cell.
		This number is retrieved from the cell's
		text property.
	Params
		number - pointer to a double to recieve 
			the value
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetNumber(double *number)
{
	if( m_propSetFlags&UGCELL_STRING_SET )
	{
		*number = (double)GetNumber();
		return UG_SUCCESS;
	}

	return UG_ERROR;
}
/********************************************
GetNumber
	Purpose
		Returns the number value for the cell.
		This number is retrieved from the cell's
		text property. 
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return 
		the number value of the cell
*********************************************/
double	CUGCell::GetNumber()
{
	if ( m_propSetFlags&UGCELL_STRING_SET )
	{
		if ( m_dataType == UGCELLDATA_NUMBER ||
			  m_dataType == UGCELLDATA_CURRENCY ||
			  m_dataType == UGCELLDATA_TIME )
		{
			return m_nNumber;
		}
		else
		{
			double returnVal;
			StringToNumber( &m_string, &returnVal );
			return returnVal;
		}
	}
	return 0;
}
/********************************************
SetNumDecimals
	Purpose
		Sets the number of decimal points to
		display for numbers, this value does
		not change the actual value stored,
		it is only used for display purposes.
	Params
		number - number of decimal points to 
			display
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetNumberDecimals(int number)
{
	m_numDecimals = number;
	
	m_propSetFlags |= UGCELL_NUMBERDEC_SET;

	return UG_SUCCESS;
}
/********************************************
GetNumberDecimals	
	Purpose
		Sets the number of decimal points to
		display for numbers, this value does
		not change the actual value stored,
		it is only used for display purposes.
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		number - number of decimal points to 
			display
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetNumberDecimals(){
	
	if(m_propSetFlags&UGCELL_NUMBERDEC_SET)
		return m_numDecimals;
	return 0;
}


/********************************************
	//***** Time data type functions *****
*********************************************/

/********************************************
SetTime
	Purpose
		Sets the time value for the cell. This 
		value is converted into a string and
		put in the cell's text property
	Params
		second	0 - 59
		minute	0 - 59
		hour	0 - 23
		day		0 - 31
		month	0 - 12
		year	100 - 9999

	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetTime(int second,int minute,int hour,int day,int month,int year)
{
	// populate the datetime structure with the information passed in
	COleDateTime odt;
	odt.SetDateTime(year,month,day,hour,minute,second);
	// set the display string
	SetText( odt.Format());  // use default of LANG_USER_DEFAULT
	// store the native form of the date
	m_nNumber = odt;

	m_dataType	= UGCELLDATA_TIME;
	m_propSetFlags |= (UGCELL_STRING_SET + UGCELL_DATATYPE_SET);

	return UG_SUCCESS;
}
/********************************************
GetTime
	Purpose
		Gets the time value stored in the cell's
		text property. The value is converted from
		a string to the time components. 
	Params
		second	0 - 59
		minute	0 - 59
		hour	0 - 23
		day		1 - 31
		month	1 - 12
		year	100 - 9999

	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::GetTime(int* second,int* minute,int* hour,int* day,int* month,int* year)
{
	COleDateTime odt;

	if(m_propSetFlags&UGCELLDATA_TIME)
		odt = m_nNumber;
	else
		// convert cell's string into a date fromat
		odt.ParseDateTime(m_string);

	*day = odt.GetDay();
	*month = odt.GetMonth();
	*year = odt.GetYear();
			
	*second = odt.GetSecond();
	*minute = odt.GetMinute();
	*hour = odt.GetHour();

	return UG_SUCCESS;
}
/********************************************
	//set and get data types
*********************************************/

/********************************************
SetDataType
	Purpose
		Sets the data type that the cells
		text property is to be treaded as.
		valid values:
			UGCELLDATA_STRING
			UGCELLDATA_NUMBER
			UGCELLDATA_BOOL	
			UGCELLDATA_TIME	
			UGCELLDATA_CURRENCY
	Params
		type
	Return 
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetDataType(short type)
{
	if ( type == m_dataType && ( m_propSetFlags&UGCELL_DATATYPE_SET ))
		return UG_SUCCESS;

	if ( type == UGCELLDATA_NUMBER && m_propSetFlags&UGCELL_STRING_SET )
	{
		CString cellVal = GetText();
		StringToNumber( &cellVal, &m_nNumber );
		m_string = "";
	}
	else if ( type == UGCELLDATA_TIME && m_propSetFlags&UGCELL_STRING_SET )
	{
		CString cellVal = GetText();
		COleDateTime dateTime;
		dateTime.ParseDateTime( cellVal );
		m_nNumber = dateTime;
	}
	else if ( type == UGCELLDATA_CURRENCY && m_propSetFlags&UGCELL_STRING_SET )
	{
		if ( m_dataType == UGCELLDATA_NUMBER )
		{
			m_string.Format(_T("%.2f"), m_nNumber );
		}
		else
		{
			CString cellVal = GetText();
			COleCurrency currency;
			currency.ParseCurrency( cellVal );
			m_nNumber = (double)(currency.m_cur.int64 / 10000);
		}
	}

	m_dataType	= type;
	m_propSetFlags |= UGCELL_DATATYPE_SET;

	return UG_SUCCESS;
}

/********************************************
GetDataType
	Purpose
		Returns the data type that the cells
		data is to be treated as.
		valid values are:
			UGCELLDATA_STRING
			UGCELLDATA_NUMBER
			UGCELLDATA_BOOL	
			UGCELLDATA_TIME	
			UGCELLDATA_CURRENCY
	Params
		none
	Return
		the data type
*********************************************/
int	CUGCell::GetDataType(){
	
	if(m_propSetFlags&UGCELL_DATATYPE_SET)
		return m_dataType;
	else
		return UGCELLDATA_STRING;
}
	
/********************************************
	//***** General functions *****
*********************************************/

/********************************************
SetTextColor
	Purpose
		Sets the text color property of a cell
	Params
		color - color to set the property to
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetTextColor(COLORREF color)
{
	m_textColor = color;
	m_propSetFlags |= UGCELL_TEXTCOLOR_SET;

	return UG_SUCCESS;
}

/********************************************
GetTextColor
	Purpose
		Returns the current color of the
		cells text color property.
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
COLORREF CUGCell::GetTextColor(){

	if(m_propSetFlags&UGCELL_TEXTCOLOR_SET)
		return m_textColor;
	else
		return GetSysColor(COLOR_WINDOWTEXT);
}

/********************************************
SetBackColor
	Purpose
		Sets the background color property of a cell
	Params
		color - color to set the property to
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetBackColor(COLORREF color)
{
	m_backColor = color;
	m_propSetFlags |= UGCELL_BACKCOLOR_SET;

	return UG_SUCCESS;
}

/********************************************
GetBackColor
	Purpose
		Returns the current color of the
		cells background color property. This
		function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
COLORREF CUGCell::GetBackColor(){

	if(m_propSetFlags&UGCELL_BACKCOLOR_SET)
		return m_backColor;
	else
		return GetSysColor(COLOR_WINDOW);
}

/********************************************
SetHTextColor
	Purpose
		Sets the highlight text color property 
		of a cell
	Params
		color - color to set the property to
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetHTextColor(COLORREF color)
{
	m_hTextColor = color;
	m_propSetFlags |= UGCELL_HTEXTCOLOR_SET;
	
	return UG_SUCCESS;
}

/********************************************
GetHTextColor
	Purpose
		Returns the current color of the
		cells highlight text color property. 
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
COLORREF CUGCell::GetHTextColor(){

	if(m_propSetFlags&UGCELL_HTEXTCOLOR_SET)
		return m_hTextColor;
	else
		return GetSysColor(COLOR_HIGHLIGHTTEXT);
}

/********************************************
SetHBackColor
	Purpose
		Sets the highlight background color 
		property of a cell
	Params
		color - color to set the property to
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetHBackColor(COLORREF color)
{
	m_hBackColor = color;
	m_propSetFlags |= UGCELL_HBACKCOLOR_SET;

	return UG_SUCCESS;
}

/********************************************
GetHBackColor
	Purpose
		Returns the current color of the
		cells highlight background color property. 
		This function does not check to see
		if the property is set, so use the
		IsPropertySet function first.
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
COLORREF CUGCell::GetHBackColor(){

	if(m_propSetFlags&UGCELL_HBACKCOLOR_SET)
		return m_hBackColor;
	else
		return GetSysColor(COLOR_HIGHLIGHT);
}

/********************************************
SetAlignment
	Purpose
		Sets the alignment property of the
		cell. Verticle and horizontal settings
		can be ORed together.
		Valid values are:
			UG_ALIGNLEFT
			UG_ALIGNRIGHT
			UG_ALIGNCENTER
			UG_ALIGNTOP
			UG_ALIGNBOTTOM
			UG_ALIGNVCENTER
	Params
		align - the alignment value to set
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetAlignment(short align)
{	
	m_propSetFlags |= UGCELL_ALIGNMENT_SET;
	m_alignment = align;

	return UG_SUCCESS;
}

/********************************************
GetAlignment
	Purpose
		Returns the alignment value from the
		cell's alignment property.
		Valid values are:
			UG_ALIGNLEFT
			UG_ALIGNRIGHT
			UG_ALIGNCENTER
			UG_ALIGNTOP
			UG_ALIGNBOTTOM
			UG_ALIGNVCENTER
		These values are set as BITs within the
		return value.
	Params
		none
	Return
		Alignment property value
*********************************************/
short CUGCell::GetAlignment(){

	if(m_propSetFlags&UGCELL_ALIGNMENT_SET)
		return m_alignment;
	return 0;
}

/********************************************
SetBorder
	Purpose
		Sets the border property of the cell.
		Border styles are ORed together.
		Valid values are:
			UG_BDR_LTHIN
			UG_BDR_TTHIN
			UG_BDR_RTHIN
			UG_BDR_BTHIN
			UG_BDR_LMEDIUM
			UG_BDR_TMEDIUM
			UG_BDR_RMEDIUM
			UG_BDR_BMEDIUM
			UG_BDR_LTHICK
			UG_BDR_TTHICK
			UG_BDR_RTHICK
			UG_BDR_BTHICK
			UG_BDR_RECESSED
			UG_BDR_RAISED
	Params
		style
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetBorder(short style)
{
	m_propSetFlags |= UGCELL_BORDERSTYLE_SET;
	m_borderStyle = style;
	
	return UG_SUCCESS;
}

/********************************************
GetBorder
	Purpose
		Returns the cells border style property.
		The return value is made up of bits which
		corresond to a style for each piece of the
		border.
		Valid values are.
			UG_BDR_LTHIN
			UG_BDR_TTHIN
			UG_BDR_RTHIN
			UG_BDR_BTHIN
			UG_BDR_LMEDIUM
			UG_BDR_TMEDIUM
			UG_BDR_RMEDIUM
			UG_BDR_BMEDIUM
			UG_BDR_LTHICK
			UG_BDR_TTHICK
			UG_BDR_RTHICK
			UG_BDR_BTHICK
			UG_BDR_RECESSED
			UG_BDR_RAISED
	Params
		none
	Return
		border property value
*********************************************/
short CUGCell::GetBorder(){
	
	if(m_propSetFlags&UGCELL_BORDERSTYLE_SET)
		return m_borderStyle;
	return 0;
}

/********************************************
SetBorderColor
	Purpose
		Sets the border color property of 
		the cell. The color used is in the 
		form of a CPen object. The CPen should
		use the PS_SOLID style and be 1 pixel
		wide.
		The cell object will not destroy the object
		that is passed in here. It is the responsibilty
		of the caller to destroy the object when finished.
	Params
		pen - pointer to the cpen object to use.
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::SetBorderColor(CPen *pen)
{
	m_propSetFlags |= UGCELL_BORDERCOLOR_SET;
	m_borderColor = pen;
	
	return UG_SUCCESS;
}

/********************************************
GetBorderColor
	Purpose
		Returns the pointer to a CPen Object that
		is used to draw the cells border.
	Params
		none
	Return
		Pointer to a CPen object, or NULL if
		this property is not set
*********************************************/
CPen * CUGCell::GetBorderColor(){

	if(m_propSetFlags&UGCELL_BORDERCOLOR_SET)
		return m_borderColor;
	return NULL;
}

/********************************************
SetFont
	Purpose
		Sets the font property of the cell.
		The same CFont can be used for multiple
		cells.
		The cell object will not destroy the object
		that is passed in here. It is the responsibilty
		of the caller to destroy the object when finished.
	Params
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetFont(CFont *font)
{
	m_propSetFlags |= UGCELL_FONT_SET;
	m_font = font;
	
	return UG_SUCCESS;
}

/********************************************
GetFont
	Purpose
		Returns the font property for the cell.
		The font property is a pointer to a 
		CFont object.
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
CFont * CUGCell::GetFont(){
	
	if(m_propSetFlags&UGCELL_FONT_SET)
		return m_font;
	return NULL;
}

/********************************************
SetBitmap
	Purpose
		Sets the bitmap property of the cell.
		The bitmap property holds a pointer
		to a CBitmap object, but does not
		take ownership. When the cell object
		is destroyed it does not delete the 
		bitmap object, it is the responsibility
		of the calling function/application.
	Params
		bitmap - pointer to a CBitmap object
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetBitmap(CBitmap * bitmap)
{
	if(NULL != bitmap) 
	{
		m_propSetFlags |= UGCELL_BITMAP_SET;
	}
	else 
	{
		if(	m_propSetFlags&UGCELL_BITMAP_SET)
			 m_propSetFlags ^= UGCELL_BITMAP_SET;	// clear flag
	}
		
	m_bitmap = bitmap;

	return UG_SUCCESS;
}

/********************************************
GetBitmap
	Purpose
		Returns the cells bitmap property.
		The property is a pointer to an
		existing CBitmap object.
	Params
		none
	Return
		pointer to a CBitmap
*********************************************/
CBitmap * CUGCell::GetBitmap(){
	
	if(m_propSetFlags&UGCELL_BITMAP_SET)
		return m_bitmap;
	return NULL;
}

/********************************************
SetReadOnly
	Purpose
		Sets the cells read only property.
		If true then the cell cannot be 
		edited, but SetCell functions will
		still work.
	Params
		state - TRUE or FALSE
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetReadOnly(BOOL state)
{	
	if(state)
		m_readOnlyFlag = TRUE;
	else
		m_readOnlyFlag = FALSE;

	m_propSetFlags |= UGCELL_READONLY_SET;

	return UG_SUCCESS;
}

/********************************************
GetReadOnly
	Purpose
		Returns the cell's read only property.
	Params
		none
	Return
		TRUE/FALSE - read only property state
*********************************************/
BOOL CUGCell::GetReadOnly(){
	
	if(m_propSetFlags&UGCELL_READONLY_SET)
		return m_readOnlyFlag;
	return FALSE;
}

/********************************************
SetCellType
	Purpose
		Sets the cells cell type property.
		The cell type must be registered with
		the grid to work (or built in)
	Params
		index - the index value of the registered
			cell type
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::SetCellType(int index)
{
	m_propSetFlags |= UGCELL_CELLTYPE_SET;

	m_cellType = index;

	return UG_SUCCESS;
}

/********************************************
SetCellTypeEx
	Purpose
		Sets the extended style of a cell type.
		Extended styles are cell type dependant.
	Params
		type - extended style number
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetCellTypeEx(long type)
{	
	m_propSetFlags |= UGCELL_CELLTYPEEX_SET;
	m_cellTypeEx = type;
	
	return UG_SUCCESS;
}

/********************************************
GetCellType
	Purpose
		Returns the cell type that the cell is
		using. This function does not check
		to see if the property is set or not.
	Params
		none
	Return
		cell type that the cell is using
*********************************************/
int CUGCell::GetCellType(){

	if(m_propSetFlags&UGCELL_CELLTYPE_SET)
		return m_cellType;
	else
		return 0;
}

/********************************************
GetCellTypeEx
	Purpose
		Returns the celltype extended style
		property. This function does not check
		to see if the property is set or not.
	Params
		none
	Return
		cell type extended style property value
*********************************************/
long CUGCell::GetCellTypeEx(){

	if(m_propSetFlags&UGCELL_CELLTYPEEX_SET)
		return m_cellTypeEx;
	return 0;
}

/********************************************
SetJoinInfo
	Purpose
		Sets join information for a cell.
		If origin is TRUE then col and row
		point to the last cell in the region.
		If origin is FALSE then col and row
		point to the first cell in the region.
		col and row are relative numbers.
		Ever cell in the joined region need to
		have their join properties set.
	Params
		origin - join region origin flag
		col	- relative col to the start or end of join
		row - relative row to the start or end of join
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::SetJoinInfo(BOOL origin,int col,long row)
{	
	m_joinOrigin  = origin;
	m_joinCol	 = col;
	m_joinRow	 = row;

	m_propSetFlags |= UGCELL_JOIN_SET;

	return UG_SUCCESS;
}

/********************************************
GetJoinInfo
	Purpose
		Returns the join information for the cell,
		if set. see SetJoinInfo for more information.
		Values are returned in the parameters passed
		in.
	Params
		origin - join region origin flag
		col	- relative col to the start or end of join
		row - relative row to the start or end of join
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::GetJoinInfo(BOOL *origin,int *col,long *row){
	
	if(m_propSetFlags&UGCELL_JOIN_SET){
		*origin = m_joinOrigin;
		*col	= m_joinCol;
		*row	= m_joinRow;

		return UG_SUCCESS;
	}
	return UG_ERROR;
}

/********************************************
ClearExtraMem
	Purpose
		Clears data that was set in the cells
		extra memory property. 
	Params
		none
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::ClearExtraMem()
{
	if(m_propSetFlags&UGCELL_EXTRAMEMORY_SET)
		m_propSetFlags &= ~UGCELL_EXTRAMEMORY_SET;
	
	if(m_extraMem != NULL)
		delete[] m_extraMem;

	m_extraMem = NULL;
	m_extraMemSize = 0;

	return UG_SUCCESS;
}

/********************************************
AllocExtraMem
	Purpose
		Allocates a block of data using the 
		specified size. This data can hold any
		type of information.
	Params
		len - length of data of allocate, in bytes.
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
void *	CUGCell::AllocExtraMem(long len){

	ClearExtraMem();

	try{
		m_extraMem = new BYTE[len];
		if(m_extraMem == NULL)
			return NULL;
	}
	catch( CException* e ){
		e->Delete();
		return NULL;
	}    

	m_propSetFlags |= UGCELL_EXTRAMEMORY_SET;
	m_extraMemSize = len;

	return m_extraMem;
}

/********************************************
GetExtraMemPtr
	Purpose
		Returns a pointer to the extra memory
		that was allocated, using the function
		above. This function does not check to 
		see if this property was set or not.
	Params
		none
	Return
		pointer to the extra cell memory
*********************************************/
void *	CUGCell::GetExtraMemPtr(){

	if(m_propSetFlags&UGCELL_EXTRAMEMORY_SET)
		return m_extraMem;
	return NULL;
}

/********************************************
GetExtraMemSize
	Purpose
		Returns the number of bytes that were 
		allocated for the cells extra memory
		data block. This function does not 
		check to see if this property was set
		or not.
	Params
		none
	Return
		extra memory size in bytes.
*********************************************/
long CUGCell::GetExtraMemSize(){

	if(m_propSetFlags&UGCELL_EXTRAMEMORY_SET)
		return m_extraMemSize;
	return 0;
}

/********************************************
StringToNumber
	Purpose
		Converts a string to a number.
		This function will work with strings
		containing characters other than numbers,
		by removing non-digit characters before the
		conversion. Therefore it will work with 
		strings such as: "$123,456.00","123-456-789"
	Params
		string - string to convert
		number - resulting number
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::StringToNumber(CString *string,double *number)
{
	CString tempString;
	tempString = *string;
	// remove localized thousand separators
	TCHAR szThousandSep[4];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandSep, 4);
	tempString.Replace( szThousandSep, _T("") );
	// remove localized currency indicators
	TCHAR szCurrencyMark[6];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, szCurrencyMark, 6);
	tempString.Replace( szCurrencyMark, _T("") );
	// Replace the localized decimal separator with the standard, which is
	// understood by the _ttol function
	TCHAR szDecimalSep[4];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecimalSep, 4);
	tempString.Replace( szDecimalSep, _T(".") );

	// parse the string to its numeric value
	*number = ::_tcstod(tempString, NULL);

	return UG_SUCCESS;
}

/********************************************
StringToBool
	Purpose
		converts the given string to a bool value.
		This function returns strings containing 
		characters other than '0' or 'F' as true.
	Params
		string - string to convert
		state - resulting bool value
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::StringToBool(CString *string,BOOL * state){
	if(string->GetAt(0) == _T('0') || string->GetAt(0) == _T('F') ||
	  string->GetAt(0) == _T('f'))
		*state = FALSE;
	else
		*state = TRUE;
	return UG_SUCCESS;
}

/********************************************
StringToTime
	Purpose
		converts a string into a time value.
		This function using the current locale 
		setting to help perform the conversion.
	Params
		string - string to convert
		second - resulting second
		minute - resulting minute
		hour - resulting hour
		day - resulting day
		month  - resulting month
		year - resulting year
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::StringToTime(CString* string,int* second,int* minute,int* hour,int* day, int* month,int* year)
{
	COleVariant ovStr(*string);
	COleDateTime ovTime(ovStr);

	if(ovTime.GetStatus() != COleDateTime::valid)
		return UG_ERROR;

	*second = ovTime.GetSecond();
	*minute = ovTime.GetMinute();
	*hour = ovTime.GetHour();

	*day = ovTime.GetDay();
	*month = ovTime.GetMonth();
	*year = ovTime.GetYear();

	return UG_SUCCESS;
}

/********************************************
IsPropertySet
	Purpose
		Returns TRUE if the specified property has
		been set or FALSE is it has not been set.
		This function should be called before 
		most of the cells Get... functions. Since
		most Get... funtions do not check to see
		if the property has been set.
	Params
		flag - property to check - see the defines in the header file
	Return
		TRUE/FALSE value identifying if property is set
*********************************************/
BOOL CUGCell::IsPropertySet(long flag){
	if(flag&m_propSetFlags)
		return TRUE;
	return FALSE;
}

/********************************************
SetPropertyFlags
	Purpose
		Sets the flag contains which properties have
		been set. The flag uses one bit for each property
		so to set multiple properties OR the property set
		defines together. These defines are found in the header
		file for this class.
	Params
		flags - flags containing the properties to be set
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetPropertyFlags(long flags)
{
	m_propSetFlags = flags;
	
	return UG_SUCCESS;
}

/********************************************
GetPropertyFlags
	Purpose
		Returns the property flags value.
		Each bit in this value is used for
		one property. See the header file for
		a complete list
	Params
		none
	Return
		property flags value
*********************************************/
long CUGCell::GetPropertyFlags(){
	return m_propSetFlags;
}

/********************************************
ClearProperty
	Purpose
		Removes a property from the cells
		properties flag. Any property that
		does not have its bit set in this flag
		is considered NOT SET.
	Params
		Flags to remove
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::ClearProperty(long propertyFlag){
	return ClearPropertySetFlag(propertyFlag);
}
//this one is kept for backward compat.
int CUGCell::ClearPropertySetFlag(long propertyFlag){
	if(m_propSetFlags&propertyFlag)
		m_propSetFlags -= propertyFlag;

	return UG_SUCCESS;
}

/********************************************
SetCellStyle
	Purpose
		Sets a pointer to another cell object
		that acts as a style for this cell.
		Therefore when properties are being retrieved
		the information found in this cell is used 
		first.
	Params
		cell - pointer to a CUGCell object
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int CUGCell::SetCellStyle(CUGCell *cell)
{	
	m_propSetFlags |= UGCELL_STYLE_SET;

	m_cellStyle = cell;

	return UG_SUCCESS;
}
/********************************************
GetCellStyle
	Purpose
		Returns the pointer to the cells
		style cell object. This cell is
		generally used to give another cell
		a specific style
	Params
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
CUGCell * CUGCell::GetCellStyle(){

	if(m_propSetFlags&UGCELL_STYLE_SET)
		return m_cellStyle;
	return NULL;
}

/********************************************
SetFormatClass
	Purpose
		Sets a pointer to a formating class
		that the cell will using for
		formating data for display and validation
		of its text
	Params
		formt - pointer to a format class
	Return
		UG_SUCCESS	success
		UG_ERROR	fail
*********************************************/
int	CUGCell::SetFormatClass(CUGCellFormat *format)
{
	if(format == NULL)
		return UG_ERROR;

	m_propSetFlags |= UGCELL_FORMAT_SET;
	m_format = format;

	return UG_SUCCESS;
}

/********************************************
GetFormatClass
	Purpose
		Returns the pointer to the cells format
		class. This funciton does not check to
		see if this property is set or not
	Params
		none
	Return
		pointer to the cells format class
*********************************************/
CUGCellFormat * CUGCell::GetFormatClass(){
	
	if(m_propSetFlags&UGCELL_FORMAT_SET)
		return m_format;
	return NULL;
}
/********************************************
SetParam
*********************************************/
int	CUGCell::SetParam(long param)
{
	m_param = param;
	m_propSetFlags |= UGCELL_PARAM_SET;

	return UG_SUCCESS;
}
/********************************************
GetParam
*********************************************/
long CUGCell::GetParam(){

	if(m_propSetFlags&UGCELL_PARAM_SET)
		return m_param;
	return 0;
}

/********************************************
SetInitialState
	Purpose
		Copys the current cell state into the initial
		cell state, and additionally deletes it if it's 
		been created before, and either way creates a new 
		one.
	Params
		none
	Return
		none
*********************************************/
void CUGCell::SetInitialState()
{
	if (m_cellInitialState) 
	{
		delete m_cellInitialState;
		m_cellInitialState = NULL;
	}

	m_cellInitialState = new CUGCell();

	m_cellInitialState->CopyInfoFrom(this);
}
/********************************************
LoadInitialState
	Purpose
		If the initial state of this cell 
		has been set, this method will restore it
	Params
		none
	Return
		none
*********************************************/
void CUGCell::LoadInitialState()
{
	if (m_cellInitialState)
	{
		this->CopyInfoFrom(m_cellInitialState);
	}
}

/********************************************
ClearMemory
	Purpose
		 This method exists to provide an internalised
		 method that can be called by the memory manager
		 to clean up pointers that exist in the cell class,
		 that are initialised when the cell is created, and 
		 which cannot be deleted in the destructor, as the cell
		 type is widely used to represent a copy of a cell.
	Params
		none
	Return
		none
*********************************************/
void CUGCell::ClearMemory()
{
	if (m_cellInitialState)
	{
		delete m_cellInitialState;
		m_cellInitialState = NULL;
	}

	//extra memory
	ClearExtraMem();
}