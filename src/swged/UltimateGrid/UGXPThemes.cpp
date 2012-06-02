// UGXPThemes.cpp: implementation of the UGXPThemes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// This is here to deal with the nastiness of the VC6 STL implimentation.
// std::map has been used within the limitations of the VC6 implimentation, but for VC6, 
// a third party STL library is definitely recommended.
#pragma warning (disable: 4786)

#include "UGCtrl.h"

#pragma warning (push,3)
#include<map>
#pragma warning (pop)
#include <ATLConv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// For GetVersionEx
#include <Shlwapi.h>


// The UGThemeData class contains all the info we need to draw a themed item.  These class instances are associated with a value derived from a ThemeState and a XPCellType ( two enums with bitfield values )
// using a std::map in the anonymous namespace below.  The class itself is hidden here because it's only used by the UGXPThemes class, which exposes methods to add values to
// the map, UGXPThemes creates the UGThemeData instances.
class UGThemeData
{
private:
	std::wstring ThemeName;
	int PartID;
	int StateID;
	UGThemeData(){}

public:
	LPCWSTR GetThemeName(){ return ThemeName.c_str();}
	int GetPartID() { return PartID; }
	int GetStateID() { return StateID; }
	UGThemeData(LPCWSTR themeName, int partID, int stateID)
	{
		ThemeName = themeName;
		PartID = partID;
		StateID = stateID;
	}
};


// Maps are stored in an anonymous namespace so we can include <map> in the cpp only, thus evading the
// problems in VC6 with warning C4786, and other issues related to VC6 STL nastiness.
namespace
{
	std::map<LPCWSTR, HANDLE> m_Handles;
	std::map<int, UGThemeData*> m_themeData;

	void PopulateStyle1()
	{
		if (m_themeData[XPCellTypeData | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeData | ThemeStateNormal] = new UGThemeData(L"TAB", 1, 1);
		if (m_themeData[XPCellTypeData | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeData | ThemeStateCurrent ] = new UGThemeData(L"TAB", 1, 3);
		if (m_themeData[XPCellTypeData | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeData | ThemeStateSelected ] = new UGThemeData(L"TAB", 1, 2);
		if (m_themeData[XPCellTypeData | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeData | ThemeStateTriState ] = new UGThemeData(L"TAB", 1, 4);
		if (m_themeData[XPCellTypeData | ThemeStateCurrent | ThemeStateSelected ] == NULL)
			m_themeData[XPCellTypeData | ThemeStateSelected | ThemeStateCurrent ] = new UGThemeData(L"TAB", 1, 5);

		if (m_themeData[XPCellTypeLeftCol | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeLeftCol | ThemeStateNormal ] = new UGThemeData(L"TASKBAR", 4, 0);
		if (m_themeData[XPCellTypeLeftCol | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeLeftCol | ThemeStateCurrent ] = new UGThemeData(L"TASKBAR", 8, 0);

		if (m_themeData[XPCellTypeTopCol | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeTopCol | ThemeStateNormal ] = new UGThemeData(L"TASKBAR", 3, 0);
		if (m_themeData[XPCellTypeTopCol | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeTopCol | ThemeStateCurrent ] = new UGThemeData(L"TASKBAR", 7, 0);

		if (m_themeData[XPCellTypeBorder | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeBorder | ThemeStateNormal ] = new UGThemeData(L"WINDOW", 11, 0);

		if (m_themeData[XPCellTypeCombo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateNormal ] = new UGThemeData(L"COMBOBOX", 1, 0);
		if (m_themeData[XPCellTypeCombo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateCurrent ] = new UGThemeData(L"COMBOBOX", 1, 0);
		if (m_themeData[XPCellTypeCombo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateSelected ] = new UGThemeData(L"COMBOBOX", 1, 0);
		if (m_themeData[XPCellTypeCombo | ThemeStateCurrent | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateCurrent | ThemeStateSelected ] = new UGThemeData(L"COMBOBOX", 1, 0);

		if (m_themeData[XPCellTypeCheck | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 1);
		if (m_themeData[XPCellTypeCheck | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 2);
		if (m_themeData[XPCellTypeCheck | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 3);
		if (m_themeData[XPCellTypeCheck | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 4);
		
		if (m_themeData[XPCellTypeCheckYes | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 5);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 6);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 7);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 8);

		if (m_themeData[XPCellTypeCheckNo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 9);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 10);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 11);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 12);


		if (m_themeData[XPCellTypeRadio | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 1);
		if (m_themeData[XPCellTypeRadio | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 2);
		if (m_themeData[XPCellTypeRadio | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 3);
		
		if (m_themeData[XPCellTypeRadioYes | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 5);
		if (m_themeData[XPCellTypeRadioYes | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 6);
		if (m_themeData[XPCellTypeRadioYes | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 7);

		if (m_themeData[XPCellTypeRadioNo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 5);
		if (m_themeData[XPCellTypeRadioNo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 6);
		if (m_themeData[XPCellTypeRadioNo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 7);

		if (m_themeData[XPCellTypeButton | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 1, 1);
		if (m_themeData[XPCellTypeButton | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 1, 2);
		if (m_themeData[XPCellTypeButton | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 1, 5);
		if (m_themeData[XPCellTypeButton | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeButton | ThemeStatePressed ] = new UGThemeData(L"BUTTON", 1, 3);


		if (m_themeData[XPCellTypeSpinUp | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateNormal ] = new UGThemeData(L"SPIN", 1, 1);
		if (m_themeData[XPCellTypeSpinUp | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateCurrent ] = new UGThemeData(L"SPIN", 1, 2);
		if (m_themeData[XPCellTypeSpinUp | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateSelected ] = new UGThemeData(L"SPIN", 1, 2);
		if (m_themeData[XPCellTypeSpinUp | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStatePressed ] = new UGThemeData(L"SPIN", 1, 3);

		if (m_themeData[XPCellTypeSpinDown | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateNormal ] = new UGThemeData(L"SPIN", 2, 1);
		if (m_themeData[XPCellTypeSpinDown | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateCurrent ] = new UGThemeData(L"SPIN", 2, 2);
		if (m_themeData[XPCellTypeSpinDown | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateSelected ] = new UGThemeData(L"SPIN", 2, 2);
		if (m_themeData[XPCellTypeSpinDown | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStatePressed ] = new UGThemeData(L"SPIN", 2, 3);


		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateNormal ] = new UGThemeData(L"PROGRESS", 1, 1);
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateCurrent ] = new UGThemeData(L"PROGRESS", 1, 1);
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected ] = new UGThemeData(L"PROGRESS", 1, 1);	
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected | ThemeStateCurrent] = new UGThemeData(L"PROGRESS", 1, 1);	

		if (m_themeData[XPCellTypeProgressSelected | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateNormal ] = new UGThemeData(L"PROGRESS", 3, 0);
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateCurrent ] = new UGThemeData(L"PROGRESS", 3, 0);
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateSelected ] = new UGThemeData(L"PROGRESS", 3, 0);	
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateSelected | ThemeStateCurrent] = new UGThemeData(L"PROGRESS", 3, 0);	
	}

	void PopulateStyle2()
	{
		if (m_themeData[XPCellTypeData | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeData | ThemeStateNormal] = new UGThemeData(L"LISTVIEW", 1, 1);
		if (m_themeData[XPCellTypeData | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeData | ThemeStateCurrent ] = new UGThemeData(L"LISTVIEW", 1, 3);
		if (m_themeData[XPCellTypeData | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeData | ThemeStateSelected ] = new UGThemeData(L"LISTVIEW", 1, 2);
		if (m_themeData[XPCellTypeData | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeData | ThemeStateTriState ] = new UGThemeData(L"LISTVIEW", 1, 4);
		if (m_themeData[XPCellTypeData | ThemeStateCurrent | ThemeStateSelected ] == NULL)
			m_themeData[XPCellTypeData | ThemeStateSelected | ThemeStateCurrent ] = new UGThemeData(L"LISTVIEW", 1, 5);

		if (m_themeData[XPCellTypeLeftCol | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeLeftCol | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 1, 0);
		if (m_themeData[XPCellTypeLeftCol | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeLeftCol | ThemeStateCurrent ] = new UGThemeData(L"EXPLORERBAR", 9, 0);

		if (m_themeData[XPCellTypeTopCol | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeTopCol | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 5, 0);
		if (m_themeData[XPCellTypeTopCol | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeTopCol | ThemeStateCurrent ] = new UGThemeData(L"EXPLORERBAR", 9, 0);

		if (m_themeData[XPCellTypeBorder | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeBorder | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 5, 0);

		if (m_themeData[XPCellTypeCombo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 4, 1);
		if (m_themeData[XPCellTypeCombo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateCurrent ] = new UGThemeData(L"EXPLORERBAR", 4, 2);
		if (m_themeData[XPCellTypeCombo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateSelected ] = new UGThemeData(L"EXPLORERBAR", 4, 3);
		if (m_themeData[XPCellTypeCombo | ThemeStateCurrent | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCombo | ThemeStateCurrent | ThemeStateSelected ] = new UGThemeData(L"EXPLORERBAR", 4, 3);

		if (m_themeData[XPCellTypeCheck | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 1);
		if (m_themeData[XPCellTypeCheck | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 2);
		if (m_themeData[XPCellTypeCheck | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 3);
		if (m_themeData[XPCellTypeCheck | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheck | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 4);
		
		if (m_themeData[XPCellTypeCheckYes | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 5);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 6);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 7);
		if (m_themeData[XPCellTypeCheckYes | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheckYes | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 8);

		if (m_themeData[XPCellTypeCheckNo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 3, 9);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 3, 10);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 3, 11);
		if (m_themeData[XPCellTypeCheckNo | ThemeStateTriState] == NULL)
			m_themeData[XPCellTypeCheckNo | ThemeStateTriState ] = new UGThemeData(L"BUTTON", 3, 12);


		if (m_themeData[XPCellTypeRadio | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 1);
		if (m_themeData[XPCellTypeRadio | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 2);
		if (m_themeData[XPCellTypeRadio | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadio | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 3);
		
		if (m_themeData[XPCellTypeRadioYes | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 5);
		if (m_themeData[XPCellTypeRadioYes | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 6);
		if (m_themeData[XPCellTypeRadioYes | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadioYes | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 7);

		if (m_themeData[XPCellTypeRadioNo | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 2, 5);
		if (m_themeData[XPCellTypeRadioNo | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 2, 6);
		if (m_themeData[XPCellTypeRadioNo | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeRadioNo | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 2, 7);

		if (m_themeData[XPCellTypeButton | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateNormal ] = new UGThemeData(L"BUTTON", 1, 1);
		if (m_themeData[XPCellTypeButton | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateCurrent ] = new UGThemeData(L"BUTTON", 1, 2);
		if (m_themeData[XPCellTypeButton | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeButton | ThemeStateSelected ] = new UGThemeData(L"BUTTON", 1, 5);
		if (m_themeData[XPCellTypeButton | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeButton | ThemeStatePressed ] = new UGThemeData(L"BUTTON", 1, 3);

		if (m_themeData[XPCellTypeSpinUp | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateNormal ] = new UGThemeData(L"SCROLLBAR", 1, 1);
		if (m_themeData[XPCellTypeSpinUp | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateCurrent ] = new UGThemeData(L"SCROLLBAR", 1, 2);
		if (m_themeData[XPCellTypeSpinUp | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStateSelected ] = new UGThemeData(L"SCROLLBAR", 1, 2);
		if (m_themeData[XPCellTypeSpinUp | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeSpinUp | ThemeStatePressed ] = new UGThemeData(L"SCROLLBAR", 1, 3);

		if (m_themeData[XPCellTypeSpinDown | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateNormal ] = new UGThemeData(L"SCROLLBAR", 1, 5);
		if (m_themeData[XPCellTypeSpinDown | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateCurrent ] = new UGThemeData(L"SCROLLBAR", 1, 6);
		if (m_themeData[XPCellTypeSpinDown | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStateSelected ] = new UGThemeData(L"SCROLLBAR", 1, 6);
		if (m_themeData[XPCellTypeSpinDown | ThemeStatePressed] == NULL)
			m_themeData[XPCellTypeSpinDown | ThemeStatePressed ] = new UGThemeData(L"SCROLLBAR", 1, 7);

		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 1, 0);
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateCurrent ] = new UGThemeData(L"EXPLORERBAR", 1, 0);
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected ] = new UGThemeData(L"EXPLORERBAR", 1, 0);	
		if (m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressUnselected | ThemeStateSelected | ThemeStateCurrent] = new UGThemeData(L"EXPLORERBAR", 1, 0);	

		if (m_themeData[XPCellTypeProgressSelected | ThemeStateNormal] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateNormal ] = new UGThemeData(L"EXPLORERBAR", 12, 0);
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateCurrent ] = new UGThemeData(L"EXPLORERBAR", 12, 0);
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateSelected] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateSelected ] = new UGThemeData(L"EXPLORERBAR", 12, 0);	
		if (m_themeData[XPCellTypeProgressSelected | ThemeStateSelected | ThemeStateCurrent] == NULL)
			m_themeData[XPCellTypeProgressSelected | ThemeStateSelected | ThemeStateCurrent] = new UGThemeData(L"EXPLORERBAR", 12, 0);	
	}



	/********************************************
PopulateData
	Purpose
		This is called whenever a lookup fails, which should only be the first time it is called.
		It checks every value individually, so that it only sets values that have not been set by 
		the library consumer.
	Params
		none
	Return
		none
*********************************************/

	void PopulateData()
	{
		switch(UGXPThemes::GetGridStyle())
		{
		case Style2:
			PopulateStyle2();
			break;
		default:
			PopulateStyle1();
		}
	}

/********************************************
LookupThemeData
	Purpose
		This function is called instead of looking up the values manually,
		so that PopulateData is called, and the code is not repeated.
	Params
		type - the UGXPCellType enum contains all the 'types' of objects that we draw
		state - the UGXPThemeState enum contains three states - selected, current and normal.  Not all are used by all cell types
	Return
		A pointer to a UGThemeData instance, which contains all the info needed to render the item
*********************************************/
	UGThemeData * LookupThemeData(UGXPCellType type, UGXPThemeState state)
	{
		if (m_themeData.find(type | state) == m_themeData.end())
		{
			PopulateData();
		}

		UGThemeData * td = m_themeData[type | state];

		return td;
	}

}

// Initialise static variables.
HMODULE UGXPThemes::m_huxtheme = NULL;
bool UGXPThemes::useThemes = false;
bool UGXPThemes::drawEdgeBorder = true;
UGXPThemeStyles UGXPThemes::m_style = Style1;

UGXPThemes::UGXPThemes()
{
}

UGXPThemes::~UGXPThemes()
{
}

/********************************************
LoadLibrary
	Purpose
		This function is called by every other function that is called, to load the uxtheme.dll if it's not already been loaded
	Params
		None
	Return
		A bool to indicate if we succeeded in finding and loading the dll.
*********************************************/
bool UGXPThemes::LoadLibrary()
{
	if (!m_huxtheme) m_huxtheme = ::LoadLibrary(_T("uxtheme.dll"));

	return (m_huxtheme != NULL);
}

/********************************************
IsThemed
	Purpose
		This function checks two things - if we're running on a machine that is Theme capable, and if we want to use themes
	Params
		None
	Return
		A bool which is true if themes are available and enabled.
*********************************************/
bool UGXPThemes::IsThemed()
{
    bool ret = false;

	if (useThemes)
	{
		OSVERSIONINFO ovi = {0};
		ovi.dwOSVersionInfoSize = sizeof ovi;
		GetVersionEx(&ovi);
		// TODO: Fix for Vista
		if(ovi.dwMajorVersion==5 && ovi.dwMinorVersion==1)
		{
			//Windows XP detected
			typedef BOOL WINAPI ISAPPTHEMED();
			typedef BOOL WINAPI ISTHEMEACTIVE();
			ISAPPTHEMED* pISAPPTHEMED = NULL;
			ISTHEMEACTIVE* pISTHEMEACTIVE = NULL;

			if(LoadLibrary())
			{
				pISAPPTHEMED = reinterpret_cast<ISAPPTHEMED*>(
					GetProcAddress(m_huxtheme,("IsAppThemed")));
				pISTHEMEACTIVE = reinterpret_cast<ISTHEMEACTIVE*>(
					GetProcAddress(m_huxtheme,("IsThemeActive")));
				if(pISAPPTHEMED && pISTHEMEACTIVE)
				{
					if(pISAPPTHEMED() && pISTHEMEACTIVE())                
					{                
						typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
						DLLGETVERSION* pDLLGETVERSION = NULL;

						HMODULE hModComCtl = ::LoadLibrary(_T("comctl32.dll"));
						if(hModComCtl)
						{
							pDLLGETVERSION = reinterpret_cast<DLLGETVERSION*>(
								GetProcAddress(hModComCtl,("DllGetVersion")));
							if(pDLLGETVERSION)
							{
								DLLVERSIONINFO dvi = {0};
								dvi.cbSize = sizeof dvi;
								if(pDLLGETVERSION(&dvi) == NOERROR )
								{
									ret = true; //dvi.dwMajorVersion >= 6; TODO: this is returning 5.8, reflecting the one in the system32 dir
								}
							}
							FreeLibrary(hModComCtl);                    
						}
					}
				}
			}
		}
	}

    return ret;
}

/********************************************
  OpenThemeData
	Purpose
		This function maps directly to the OpenThemeData function in the dll.
	Params
		hwnd - the hwnd of the Window we intend to draw to.  This can be NULL, 
		and often is, rather than significanly modify the library, as the draw functions 
		are often not called from a CWnd derived class.
	Return
		A HANDLE to the theme data, which we cache or future calls.
*********************************************/
HANDLE UGXPThemes::OpenThemeData(HWND hwnd, LPCWSTR pszClassList, bool useCache)
{
	if (m_Handles[pszClassList] == NULL && LoadLibrary())
	{
		OPENTHEMEDATA pOpenThemeData = (OPENTHEMEDATA)GetProcAddress(m_huxtheme, "OpenThemeData");

		if (pOpenThemeData != NULL)
		{
			if (useCache)
			{
				m_Handles[pszClassList] = (*pOpenThemeData)(hwnd, pszClassList);
			}
			else
			{
				return (*pOpenThemeData)(hwnd, pszClassList);
			}
		}
	}
	
	return m_Handles[pszClassList];
}

/********************************************
GetThemeMargins
	Purpose
		This calls GetThemeMargins, which returns the margins of the area 
		which will be taken up by drawing the theme edge.
	Params
		hTheme     - the theme handle
		hdc        - the HDC to draw to
		iPartId    - the theme part to draw
		iStateId   - the state of the part to draw
		iPropId    - the margin type to return
		prc        - the rect to draw to
		XPMARGINS  - an internal struct which copies the MARGINS struct, so this code
					 will work if the XPThemes header files are available or not.
	Return
		A HRESULT to indicate success or failure.
*********************************************/
HRESULT UGXPThemes::GetThemeMargins(HANDLE hTheme,  HDC hdc, int iPartId, 
		 int iStateId, int iPropId,  RECT *prc, XPMARGINS *pMargins)
{
	GETTHEMEMARGINS pGetThemeMargins = (GETTHEMEMARGINS)GetProcAddress (m_huxtheme, "GetThemeMargins");

	if (pGetThemeMargins)
	{
		return (*pGetThemeMargins)(hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
	}

	return E_FAIL;
}

HRESULT UGXPThemes::GetThemePartSize(HANDLE hTheme, HDC dc, int iPartId, int iStateId, RECT * pRect, int eSize,  SIZE *psz)
{
	GETTHEMEPARTSIZE pGetThemePartSize = (GETTHEMEPARTSIZE)GetProcAddress (m_huxtheme, "GetThemePartSize");

	if (pGetThemePartSize)
	{
		return (*pGetThemePartSize)(hTheme, dc, iPartId, iStateId, pRect, eSize, psz);
	}

	return E_FAIL;

}

bool UGXPThemes::GetThemePartSize(LPCWSTR theme, HDC dc, int iPartId, int iStateId, RECT * pRect, int eSize,  SIZE *psz)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(NULL, theme);

		if (themeHandle)
		{
			HRESULT hr = GetThemePartSize(themeHandle, dc, iPartId, iStateId, pRect, eSize, psz);
			success = SUCCEEDED(hr);
		}
	}
	return success;
}


bool UGXPThemes::GetThemeRect(LPCWSTR theme, int iPartId, int iStateId, RECT *pRect)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(NULL, theme);

		if (themeHandle)
		{

			const int default_pane_size = 5002;
			HRESULT hr = GetThemeRect(themeHandle, iPartId, iStateId, default_pane_size, pRect);
			success = SUCCEEDED(hr);
		}
	}
	return success;
}

HRESULT UGXPThemes::GetThemeRect(HANDLE hTheme, int iPartId, int iStateId, int iPropId,  RECT *pRect)
{
	GETTHEMERECT pGetThemeRect = (GETTHEMERECT)GetProcAddress (m_huxtheme, "GetThemeRect");

	if (pGetThemeRect)
	{
		return (*pGetThemeRect)(hTheme, iPartId, iStateId, iPropId, pRect);
	}

	return E_FAIL;
}


HRESULT UGXPThemes::GetThemeString(HANDLE hTheme, int iPartId, int iStateId, int iPropId,  LPWSTR pszBuff, int cchMaxBuffChars)
{
	GETTHEMESTRING pGetThemeString = (GETTHEMESTRING)GetProcAddress (m_huxtheme, "GetThemeString");

	if (pGetThemeString)
	{
		return (*pGetThemeString)(hTheme, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars);
	}

	return E_FAIL;
}


/********************************************
GetThemeTextExtent
	Purpose
		This calls GetThemeTextExtent, which returns the area
		which will be taken up by drawing themed text.
	Params
		hTheme         - the theme handle
		hdc            - the HDC to draw to
		iPartId        - the theme part to draw
		iStateId       - the state of the part to draw
		pszText	       - the text to measure
		iCharCount     - the number of characters to draw
		dwTextFlags    - the text style to use when measuring the text area
		pBoundingRect  - the rect to draw text into
		pExtentRect    - used to return the rect which will be used by the text.
	Return
		A HRESULT to indicate success or failure.
*********************************************/
HRESULT UGXPThemes::GetThemeTextExtent(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
		DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect)
{
	GETTHEMETEXTEXTENT pGetThemeTextExtent = (GETTHEMETEXTEXTENT)GetProcAddress (m_huxtheme, "GetThemeTextExtent");

	if (pGetThemeTextExtent)
	{
		return (*pGetThemeTextExtent)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);
	}

	return E_FAIL;
}

/********************************************
GetMargins
	Purpose
		This wraps the GetThemeMargins call, which returns the margins of the area 
		which will be taken up by drawing the theme edge.
	Params
		hwnd	   - the window to draw to, often NULL
		hdc        - the HDC to draw to
		type	   - the cell type to draw
		state      - the state of the cell ( selected, current, etc )
		propId    - the margin type to return
		prc        - the rect to draw to
		XPMARGINS  - an internal struct which copies the MARGINS struct, so this code
					 will work if the XPThemes header files are available or not.
	Return
		A bool to indicate success or failure.
*********************************************/
bool UGXPThemes::GetMargins(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state, int propID,
		RECT *prc, XPMARGINS *pMargins)
{
	bool success = false;

	if (useThemes)
	{
		UGThemeData * td = LookupThemeData(type, state);

		if (!td)
		{
			return false;
		}

		HANDLE themeHandle = OpenThemeData(hwnd, td->GetThemeName());

		if (!themeHandle)
			return false;

		success = SUCCEEDED(GetThemeMargins(themeHandle, hdc, td->GetPartID(), td->GetStateID(), propID, prc, pMargins));
	}

	return success;
}


bool UGXPThemes::GetThemeString(LPCWSTR theme, int iPartId, int iStateId, int iPropId,  LPWSTR pszBuff, int cchMaxBuffChars)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(NULL, theme);

		if (!themeHandle)
			return false;

		success = SUCCEEDED(GetThemeString(themeHandle, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars));
	}

	return success;
}


/********************************************
GetTextExtent
	Purpose
		This wraps the call to GetThemeTextExtent, which returns 
		the area that will be taken up by drawing themed text.
		This version still takes a theme name instead of using our lookup
		table, it's used in cases where we hard code the theme part ( such a progress bars )
	Params
		hwnd	       - the window to draw to, often NULL
		hdc            - the HDC to draw to
		themeName      - the name of the theme to use
		iPartId        - the theme part to draw
		iStateId       - the state of the part to draw
		pszText	       - the text to measure
		iCharCount     - the number of characters to draw
		dwTextFlags    - the text style to use when measuring the text area
		pBoundingRect  - the rect to draw text into
		pExtentRect    - used to return the rect which will be used by the text.
	Return
		A HRESULT to indicate success or failure.
*********************************************/
bool UGXPThemes::GetTextExtent(HWND hwnd, HDC hdc, LPCWSTR themeName, int iPartId, int iStateId,
		LPCSTR pszText, int iCharCount, DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(hwnd, themeName);

		if (themeHandle)
		{
			USES_CONVERSION;

			HRESULT hr = GetThemeTextExtent(themeHandle, hdc, iPartId, iStateId, A2W(pszText), iCharCount,
				dwTextFlags, pBoundingRect, pExtentRect);
			success = SUCCEEDED(hr);
		}
	}

	return success;
	
}

/********************************************
GetTextExtent
	Purpose
		This wraps the call to GetThemeTextExtent, which returns 
		the area that will be taken up by drawing themed text.
	Params
		hwnd	       - the window to draw to, often NULL
		hdc            - the HDC to draw to
		type	       - the cell type to draw
		state          - the state of the cell ( selected, current, etc )
		pszText	       - the text to measure
		iCharCount     - the number of characters to draw
		dwTextFlags    - the text style to use when measuring the text area
		pBoundingRect  - the rect to draw text into
		pExtentRect    - used to return the rect which will be used by the text.
	Return
		A HRESULT to indicate success or failure.
*********************************************/
bool UGXPThemes::GetTextExtent(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state,
		LPCTSTR pszText, int iCharCount, DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect)
{
	bool success = false;

	if (useThemes)
	{
		UGThemeData * td = LookupThemeData(type, state);

		if (!td)
		{
			return false;
		}

		HANDLE themeHandle = OpenThemeData(hwnd, td->GetThemeName());

		if (!themeHandle)
			return false;

		USES_CONVERSION;

		success = SUCCEEDED(GetThemeTextExtent(themeHandle, hdc, td->GetPartID(), td->GetStateID(), A2W((LPCSTR)pszText), iCharCount,
			dwTextFlags, pBoundingRect, pExtentRect));
	}

	return success;
}


/********************************************
CloseThemeData
	Purpose
		A direct mapping to the CloseThemeData in the dll, this is called by our CleanUp function for all HANDLEs opened.
	Params
		hTheme - the HANDLE to the theme to close.
	Return
		A HRESULT indicating success or failure.
*********************************************/
HRESULT UGXPThemes::CloseThemeData(HANDLE hTheme)
{
	if (LoadLibrary())
	{
		CLOSETHEMEDATA pCloseThemeData = (CLOSETHEMEDATA)GetProcAddress(m_huxtheme, "CloseThemeData");

		if (pCloseThemeData != NULL)
		{
			return (*pCloseThemeData)(hTheme);
		}
	}
	
	return E_FAIL;
}

/********************************************
DrawThemeText
	Purpose
		Directly maps to the function in the dll.
	Params
		Check MSDN for more in depth info.
	Return
		A HRESULT to indicate success or failure.
*********************************************/
HRESULT UGXPThemes::DrawThemeText(HANDLE hTheme, HDC hdc, int iPartID, int iStateID, LPCWSTR pszText,
		int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT * pRect)
{
	if (!LoadLibrary()) return E_FAIL;
	
	DRAWTHEMETEXT pDrawThemeText = 
		(DRAWTHEMETEXT)GetProcAddress(m_huxtheme, "DrawThemeText");

	if(pDrawThemeText)
	{		
		return (*pDrawThemeText)(hTheme, hdc, iPartID, iStateID, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
	}

	return E_FAIL;

}

BOOL UGXPThemes::IsThemeBackgroundPartiallyTransparent(HANDLE hTheme, int iPartId, int iStateId)
{
	if (!LoadLibrary()) return E_FAIL;
	
	ISTHEMEBACKGROUNDPARTIALLYTRANSPARENT pIsBackgroundTransparent = 
		(ISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)GetProcAddress(m_huxtheme, "IsThemeBackgroundPartiallyTransparent");

	if(pIsBackgroundTransparent)
	{		
		return (*pIsBackgroundTransparent)(hTheme, iPartId, iStateId);
	}

	return FALSE;
}

HRESULT UGXPThemes::DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT * prc)
{
	if (!LoadLibrary()) return E_FAIL;
	
	DRAWTHEMEPARENTBACKGROUND pDrawThemeParentBackground = 
		(DRAWTHEMEPARENTBACKGROUND)GetProcAddress(m_huxtheme, "DrawThemeParentBackground");

	if(pDrawThemeParentBackground)
	{		
		return (*pDrawThemeParentBackground)(hwnd, hdc, prc);
	}

	return E_FAIL;
}

/********************************************
DrawThemeBackground
	Purpose
		A direct mapping to the dll function
	Params
		See MSDN for more info
	Return
		A HRESULT
*********************************************/
HRESULT UGXPThemes::DrawThemeBackground(HANDLE hTheme, HDC hdc, 
	int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
	if (!LoadLibrary()) return E_FAIL;
	
	DRAWTHEMEBACKGROUND pDrawThemeBackground = 
		(DRAWTHEMEBACKGROUND)GetProcAddress(m_huxtheme, "DrawThemeBackground");

	if(pDrawThemeBackground)
	{		
		return (*pDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
	}

	return E_FAIL;
}

/********************************************
DrawThemeEdge
	Purpose
		A direct mapping to the dll function
	Params
		See MSDN for more info
	Return
		A HRESULT
*********************************************/
HRESULT UGXPThemes::DrawThemeEdge(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
	if (!LoadLibrary()) return E_FAIL;
	
	DRAWTHEMEEDGE pDrawThemeEdge = 
		(DRAWTHEMEEDGE)GetProcAddress(m_huxtheme, "DrawThemeEdge");

	if(pDrawThemeEdge)
	{		
		return (*pDrawThemeEdge)(hTheme, hdc, iPartId, iStateId, pRect, uEdge, uFlags, pContentRect);
	}

	return E_FAIL;

}

/********************************************
DrawBackground
	Purpose
		Provides a more straightforward way to draw a background by wrapping all the calls required into the dll,
		as well as using our theme data mappings to find the core info required.
		This version still takes a theme name instead of using our lookup
		table, it's used in cases where we hard code the theme part ( such a progress bars )
	Params
		hwnd      - the HWND of the control we draw to, or NUL
		hdc       - the device context for the dll to draw to
		theme     - the name of the theme to use
		iPartId   - the theme part to draw
		iStateId  - the state of the part to draw
		pRect     - represents the area we want to draw to
		pClipRect - is exposed, but is almost always NULL
	Return
		A  bool to indicate success or failure, which is used to control drawing in the old style throughout the library
*********************************************/
bool UGXPThemes::DrawBackground(HWND hwnd, HDC hdc, LPCWSTR theme, int partID, 
		int stateID,  const RECT *pRect, const RECT *pClipRect, bool useCache)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(hwnd, theme, useCache);

		if (themeHandle)
		{
			// If the background is transparent, we draw using the Data cell type first,
			// which fills the background.  DrawThemeParentBackground just plain does not work
			// for us, perhaps because the parent is an owner drawn control ?
			// Of course, if the Data cell type has transparency, this will fail.
			if (IsThemeBackgroundPartiallyTransparent(themeHandle, partID, stateID))
			{
				//RECT temp = *pRect;
				UGThemeData * tdCell = LookupThemeData(XPCellTypeData, ThemeStateNormal);

				HANDLE hThemeCell = OpenThemeData(hwnd, tdCell->GetThemeName());

				DrawThemeBackground(hThemeCell, hdc, partID, stateID, pRect, pClipRect);
			}

			HRESULT hr = DrawThemeBackground(themeHandle, hdc, partID, stateID, pRect, pClipRect);
			success = SUCCEEDED(hr);

			if (!useCache)
			{
				CloseThemeData(themeHandle);
			}
		}
	}

	return success;

}

/********************************************
DrawBackground
	Purpose
		Provides a more straightforward way to draw a background by wrapping all the calls required into the dll,
		as well as using our theme data mappings to find the core info required.
	Params
		hwnd - the HWND of the control we draw to, or NUL
		hdc - the device context for the dll to draw to
		type - the cell type we want to draw
		state - the state of the cell we want to draw
		pRect - represents the area we want to draw to
		pClipRect - is exposed, but is almost always NULL
	Return
		A  bool to indicate success or failure, which is used to control drawing in the old style throughout the library
*********************************************/
bool UGXPThemes::DrawBackground(HWND hwnd, HDC hdc, UGXPCellType type, 
		UGXPThemeState state, const RECT *pRect, const RECT *pClipRect)
{
	bool success = false;

	if (useThemes)
	{
		UGThemeData * td = LookupThemeData(type, state);

		if (!td)
		{
			return false;
		}

		HANDLE hTheme = OpenThemeData(hwnd, td->GetThemeName());

		if(hTheme)
		{
			// If the background is transparent, we draw using the Data cell type first,
			// which fills the background.  DrawThemeParentBackground just plain does not work
			// for us, perhaps because the parent is an owner drawn control ?
			// Of course, if the Data cell type has transparency, all we can do is return E_FAILED
			if (IsThemeBackgroundPartiallyTransparent(hTheme, td->GetPartID(), td->GetStateID()))
			{
				if (type == XPCellTypeData) return FALSE;

				UGThemeData * tdCell = LookupThemeData(XPCellTypeData, (state == ThemeStatePressed || state == ThemeStateTriState) ? ThemeStateNormal : state);

				HANDLE hThemeCell = OpenThemeData(hwnd, tdCell->GetThemeName());

				DrawThemeBackground(hThemeCell, hdc, tdCell->GetPartID(), tdCell->GetStateID(), pRect, pClipRect);
			}

			success = SUCCEEDED(DrawThemeBackground(hTheme, hdc, td->GetPartID(), td->GetStateID(), pRect, pClipRect));
	//		CloseThemeData(hTheme);
		}
	}
	
	return success;
}

/********************************************
WriteText
	Purpose
		Provides a more straightforward way to draw text by wrapping all the calls required into the dll,
		as well as using our theme data mappings to find the core info required.  Was called DrawText, 
		but the preprocessor insisted on changing it to DrawTextA, which then obviously would not compile
	Params
		hwnd           - the HWND of the control we draw to, or NULL
		hdc            - the device context for the dll to draw to
		type	       - the cell type to draw
		state          - the state of the cell ( selected, current, etc )
		text	       - the text to draw
		textLength     - the number of characters to draw
		textFlags      - the text style to use when drawing the text area
		pRect          - the rect to draw text into
	Return
		A  bool to indicate success or failure, which is used to control drawing in the old style throughout the library
*********************************************/
bool UGXPThemes::WriteText(HWND hwnd, HDC hdc, UGXPCellType type, 
		UGXPThemeState state, LPCTSTR text, int textLength, DWORD textFlags, const RECT *pRect)
{
	bool success = false;

	if (useThemes)
	{
		UGThemeData * td = LookupThemeData(type, state);

		if (!td)
		{
			return false;
		}

		HANDLE hTheme = OpenThemeData(hwnd, td->GetThemeName());

		if(hTheme)
		{
			USES_CONVERSION;

			success = SUCCEEDED(DrawThemeText(hTheme, hdc, td->GetPartID(), td->GetStateID(), T2W((LPTSTR)text), textLength, textFlags, 0, pRect));
	//		CloseThemeData(hTheme);
			success = true;
		}
	}
	
	return success;
}

/********************************************
WriteText
	Purpose
		Provides a more straightforward way to draw text by wrapping all the calls required into the dll,
		as well as using our theme data mappings to find the core info required.  Was called DrawText, 
		but the preprocessor insisted on changing it to DrawTextA, which then obviously would not compile
	Params
		hwnd           - the HWND of the control we draw to, or NULL
		hdc            - the device context for the dll to draw to
		theme          - the name of the theme to use
		partId         - the theme part to draw
		stateId        - the state of the part to draw
		text	       - the text to draw, which is converted from a char * to a wide string in this method.
		textLength     - the number of characters to draw
		textFlags      - the text style to use when drawing the text area
		pRect          - the rect to draw text into
	Return
		A  bool to indicate success or failure, which is used to control drawing in the old style throughout the library
*********************************************/
bool UGXPThemes::WriteText(HWND hwnd, HDC hdc, LPCTSTR theme, int partID, 
		int stateID, LPCTSTR text, int textLength, DWORD textFlags, const RECT *pRect, bool useCache)
{
	bool success = false;

	if (useThemes)
	{
		USES_CONVERSION;

		HANDLE themeHandle = OpenThemeData(hwnd, T2W((LPTSTR)theme), useCache);

		if (themeHandle)
		{
			HRESULT hr = DrawThemeText(themeHandle, hdc, partID, stateID, T2W((LPTSTR)text), textLength, textFlags, 0, pRect);
			success = SUCCEEDED(hr);

			if (!useCache)
			{
				CloseThemeData(themeHandle);
			}
		}
	}

	return success;

}


/********************************************
DrawEdge
	Purpose
		Provides a more straightforward way to draw the edge by wrapping all the calls required into the dll,
		as well as using our theme data mappings to find the core info required.
	Params
		hwnd  - the HWND of the control we draw to, or NUL
		hdc   - the device context for the dll to draw to
		type  - the cell type we want to draw
		state - the state of the cell we want to draw
		pRect - represents the area we want to draw to
		uEdge - specifies the type of inner and outer edges to draw. 
			This parameter must be a combination of one inner-border flag and one outer-border flag, 
			or one of the combination flags.
		uFlags - specifies the type of border to draw.
		pContentRect - is exposed, but is almost always NULL
	Return
		A  bool to indicate success or failure, which is used to control drawing in the old style throughout the library
*********************************************/
bool UGXPThemes::DrawEdge(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state, 
			const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
	bool success = false;

	if (useThemes)
	{
		UGThemeData * td = LookupThemeData(type, state);

		if (!td)
		{
			return false;
		}

		HANDLE hTheme = OpenThemeData(hwnd, td->GetThemeName());

		short edgeFormat =  (uEdge & UG_BDR_RAISED) ? BDR_RAISEDINNER | BDR_SUNKENOUTER : BDR_SUNKENINNER | BDR_RAISEDOUTER;

		if(hTheme)
		{
			success = SUCCEEDED(DrawThemeEdge(hTheme, hdc, td->GetPartID(), td->GetStateID(), pRect, edgeFormat, uFlags, pContentRect));

			// We populate pContentRect with the rect to draw content in.
			if (success && pContentRect)
			{
				*pContentRect = *pRect;
				// Because the grid draws itself multiple times, this was causing areas 
				// to become undrawn.  May need some investigation, may just be unnecessary.
/*
				// 3602 = TMT_CONTENTMARGINS
				XPMARGINS margins;
				if (SUCCEEDED(UGXPThemes::GetThemeMargins(hTheme, hdc, td->GetPartID(), td->GetStateID(), 3602, const_cast<RECT*>(pRect), &margins)))
				{
					*pContentRect = *pRect;
					pContentRect->bottom -= margins.cyBottomHeight;
					pContentRect->right -= margins.cxRightWidth;
					pContentRect->top += margins.cyTopHeight;
					pContentRect->left += margins.cxLeftWidth;
				}*/
			}
	//		CloseThemeData(hTheme);
		}
	}
	
	return success;
}

/********************************************
CleanUp
	Purpose
		As the class is static, users do not need to write an instance into their code ( which means they can call it from anywhere )
		However, as various resources are allocated by the code, this means we need to provide a static method to be called when
		closing, in order to avoid a lot of warnings due to memory leaks.
	Params
		None
	Return
		None
*********************************************/
void UGXPThemes::CleanUp()
{
	// Empty the map of themeData information
	std::map<int, UGThemeData *>::iterator itThemes = m_themeData.begin();

	while(itThemes != m_themeData.end())
	{
		delete itThemes->second;
		m_themeData[itThemes->first] = NULL;
		++itThemes;
	}

	m_themeData.clear();

	// Empty the map of theme handles
	std::map<LPCWSTR, HANDLE>::iterator itHandles = m_Handles.begin();

	while(itHandles != m_Handles.end())
	{
		CloseThemeData(itHandles->second);
		++itHandles;
	}

	m_Handles.clear();

	// Close the library
	if (m_huxtheme)
	{
		FreeLibrary(m_huxtheme);
		m_huxtheme = NULL;
	}
}

/********************************************
SetXPTheme
	Purpose
		Provides a wrapper around the changing of items in our map of UGThemeData objects.  Making this
		class private forces users to use this method, thus avoiding potential memory leaks, and other problems
		that would occur if data was removed from the map.
	Params
		type - the UGXPCellType we want to set the style for
		state - the UGXPThemeState for which we want to set the state on this cell type.
		typeName - the string that indicates the broad theme to use
		iPartID - the number that indicates the item to draw
		iStateID - the number that indicates the state of the item to draw
	Return
		Nothing
*********************************************/
void UGXPThemes::SetXPTheme(UGXPCellType type, UGXPThemeState state, LPCWSTR typeName, int iPartId, int iStateId)
{
	// First delete the old one, if it's there.
	if (m_themeData[type | state] != NULL)
		delete m_themeData[type|state];

	m_themeData[type | state] = new UGThemeData(typeName, iPartId, iStateId);
}

/***********************************************
GetState
	Purpose
		A helper function to turn the two bools into a UGXPThemeState
	Params
		Self explanatory
	Return
		The UGXPThemeState that represents the state
*************************************************/
UGXPThemeState UGXPThemes::GetState(bool selected, bool current)
{
	UGXPThemeState state = ThemeStateNormal;

	if (selected && current)
	{
		state = (UGXPThemeState)(ThemeStateCurrent | ThemeStateSelected);
	}
	else if (selected)
	{
		state = ThemeStateSelected ;
	}
	else if (current)
	{
		state = ThemeStateCurrent;
	}

	return state;
}


bool UGXPThemes::DrawEdge(HWND hwnd, HDC hdc, LPCWSTR theme, int partID, int stateID, 
		const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
	bool success = false;

	if (useThemes)
	{
		HANDLE themeHandle = OpenThemeData(hwnd, theme);

		if (themeHandle)
		{
			HRESULT hr = DrawThemeEdge(themeHandle, hdc, partID, stateID, pRect, uEdge, uFlags, pContentRect);
			success = SUCCEEDED(hr);
		}
	}

	return success;

}

void UGXPThemes::SetGridStyle(UGXPThemeStyles style)
{
	m_style = style;

	CleanUp();
	PopulateData();
}