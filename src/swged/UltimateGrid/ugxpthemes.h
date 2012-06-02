// UGXPThemes.h: interface for the UGXPThemes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGXPThemes_H__5F3CE743_0206_482D_9C42_E408B983A3C6__INCLUDED_)
#define AFX_UGXPThemes_H__5F3CE743_0206_482D_9C42_E408B983A3C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ugcell.h"

#pragma warning (push,3)
#pragma warning (disable : 4018 4146)
#include<string>
#pragma warning (pop)

typedef struct _XPMARGINS
{
    int cxLeftWidth;      // width of left border that retains its size
    int cxRightWidth;     // width of right border that retains its size
    int cyTopHeight;      // height of top border that retains its size
    int cyBottomHeight;   // height of bottom border that retains its size
} XPMARGINS, *PXPMARGINS;


enum UGXPThemeStyles { Style1, Style2 };

class UG_CLASS_DECL UGXPThemes  
{
private:
	static UGXPThemeStyles m_style;
	static HMODULE m_huxtheme;
	static bool useThemes;
	static bool drawEdgeBorder;
public:
	virtual ~UGXPThemes();
	static bool IsThemed();

	static void SetGridStyle(UGXPThemeStyles style);
	static UGXPThemeStyles GetGridStyle() { return m_style; }
private:

	static BOOL IsThemeBackgroundPartiallyTransparent(HANDLE hTheme, int iPartId, int iStateId);
	static HRESULT DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT * prc);

	static HRESULT DrawThemeBackground(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
	static HANDLE OpenThemeData(HWND hwnd, LPCWSTR pszClassList, bool useCache = true);
	static HRESULT CloseThemeData(HANDLE hTheme);
	static HRESULT DrawThemeText(HANDLE hTheme, HDC hdc, int iPartId, 
		int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
		DWORD dwTextFlags2, const RECT *pRect);	
	static HRESULT DrawThemeEdge(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect);
	static HRESULT GetThemeMargins(HANDLE hTheme,  HDC hdc, int iPartId, 
		 int iStateId, int iPropId,  RECT *prc, XPMARGINS *pMargins);
		static HRESULT GetThemeTextExtent(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
		DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect);
	static HRESULT GetThemeString(HANDLE theme, int iPartId, 
		int iStateId, int iPropId,  LPWSTR pszBuff, int cchMaxBuffChars);

	static HRESULT GetThemeRect(HANDLE hTheme, int iPartId, int iStateId, int iPropId,  RECT *pRect);
	static HRESULT GetThemePartSize(HANDLE hTheme, HDC dc, int iPartId, int iStateId, RECT * pRect, int eSize,  SIZE *psz);

public:

	static bool GetThemeString(LPCWSTR theme, int iPartId, 
		int iStateId, int iPropId,  LPWSTR pszBuff, int cchMaxBuffChars);

	static bool GetTextExtent(HWND hwnd, HDC hdc, LPCWSTR themeName, int iPartId, int iStateId,
		LPCSTR pszText, int iCharCount, DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect);

	static bool WriteText(HWND hwnd, HDC hdc, LPCTSTR theme, int partID, 
		int stateID, LPCTSTR text, int textLength, DWORD textFlags, const RECT *pRect, bool useCache = true);

	static bool DrawBackground(HWND hwnd, HDC hdc, LPCWSTR theme, int partID, 
		int stateID,  const RECT *pRect, const RECT *pClipRect, bool useCache = true);
	static bool DrawBackground(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state,
		const RECT *pRect, const RECT *pClipRect);
	static bool WriteText(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state,
		LPCTSTR text, int textLength, DWORD textFlags, const RECT *pRect);
	static bool DrawEdge(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state, 
		const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect);

	static bool GetThemeRect(LPCWSTR hTheme, int iPartId, int iStateId, RECT *pRect);

	static bool GetThemePartSize(LPCWSTR hTheme, HDC dc, int iPartId, int iStateId, RECT * pRect, int eSize,  SIZE *psz);

	static bool GetMargins(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state, int propID,
		RECT *prc, XPMARGINS *pMargins);


	static bool GetTextExtent(HWND hwnd, HDC hdc, UGXPCellType type, UGXPThemeState state,
		LPCTSTR pszText, int iCharCount, DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect);

	static bool UseThemes() { return useThemes; }
	static void UseThemes(bool use) { useThemes = use; }

	static void DrawBorderEdges(bool draw) { drawEdgeBorder = draw; }
	static bool DrawBorderEdges() { return drawEdgeBorder; }

	static UGXPThemeState GetState(bool selected, bool current);

	static void CleanUp();

	static void SetXPTheme(UGXPCellType type, UGXPThemeState state, LPCWSTR typeName, int iPartId, int iStateId);

	static bool DrawEdge(HWND hwnd, HDC hdc, LPCWSTR theme, int partID, int stateID, 
		const RECT *pRect, UINT uEdge, UINT uFlags, RECT *pContentRect);

private:
	// This class has only static methods
	UGXPThemes();

	// Calling LoadLibrary here means that our static HMODULE can be initialised to NULL 
	// ( i.e. it is only loaded if we need it )
	// The function returns true if the load succeeds.
	static bool LoadLibrary(); 

	// These are the function signatures used when calling GetProcAddress.
	typedef HANDLE(__stdcall *OPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
	typedef HRESULT(__stdcall *DRAWTHEMEBACKGROUND)(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, const RECT *pRect,  const RECT *pClipRect);
	typedef HRESULT(__stdcall *CLOSETHEMEDATA)(HANDLE hTheme);
	typedef HRESULT (__stdcall *DRAWTHEMETEXT)(HANDLE hTheme, HDC hdc, int iPartId, 
		int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
		DWORD dwTextFlags2, const RECT *pRect);
	typedef HRESULT (__stdcall *DRAWTHEMEEDGE)(HANDLE hTheme, HDC hdc, int iPartId, int iStateId, 
		const RECT *pDestRect, UINT uEdge, UINT uFlags,   RECT *pContentRect);
	typedef HRESULT (__stdcall *GETTHEMEMARGINS)(HANDLE hTheme,  HDC hdc, int iPartId, 
		int iStateId, int iPropId,  RECT *prc,  XPMARGINS *pMargins);
	typedef HRESULT (__stdcall *GETTHEMETEXTEXTENT)(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
		DWORD dwTextFlags,  const RECT *pBoundingRect, 
		RECT *pExtentRect);
	typedef HRESULT (__stdcall *GETTHEMESTRING)(HANDLE hTheme, int iPartId, 
		int iStateId, int iPropId,  LPWSTR pszBuff, int cchMaxBuffChars);
	typedef BOOL (__stdcall *ISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)(HANDLE hTheme, int iPartId, int iStateId);
	typedef HRESULT (__stdcall *DRAWTHEMEPARENTBACKGROUND)(HWND hwnd, HDC hdc, RECT * prc);
	typedef HRESULT (__stdcall *GETTHEMERECT)(HANDLE hTheme, int iPartId, 
		int iStateId, int iPropId,  RECT *pRect);
	typedef HRESULT(__stdcall *GETTHEMEPARTSIZE)(HANDLE hTheme, HDC hdc, 
		int iPartId, int iStateId, RECT * pRect, int eSize,  SIZE *psz);
	/* eSize is really this:
	typedef enum THEMESIZE
	{
		TS_MIN,             // minimum size
		TS_TRUE,            // size without stretching
		TS_DRAW,            // size that theme mgr will use to draw part
	};*/
};

#endif // !defined(AFX_UGXPThemes_H__5F3CE743_0206_482D_9C42_E408B983A3C6__INCLUDED_)
