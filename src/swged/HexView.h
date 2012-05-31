// HexView.h

#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <afxwin.h>
#include <cstdint>

#include "HexDoc.h"

class CHexView : public CScrollView
{
    DECLARE_DYNCREATE(CHexView)

protected:
	CHexView() : m_dwTotalLines(0), m_dwPageLines(0) {}

protected:
    //{{AFX_VIRTUAL(CHexView)
   	virtual void OnInitialUpdate();
    virtual void OnDraw(CDC*);
    BOOL PreCreateWindow(CREATESTRUCT& cs);
    void UpdateScrollbars();
    CHexDoc* GetDocument()
    {
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHexDoc)));
	    return (CHexDoc*)m_pDocument;
    }
    //}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CHexView)
	afx_msg void OnSizing(UINT, LPRECT);
	afx_msg void OnVScroll(UINT, UINT, CScrollBar*);
	//{{AFX_MSG

private:
	DWORD m_dwOffset;
	DWORD m_dwTotalLines;
	DWORD m_dwPageLines;
	CFont m_Font;

	unsigned int m_nCurSel, m_nCurSelEnd;
	unsigned int m_nLineLength;
	unsigned int m_nLineCount;
	unsigned int m_nVisStart, m_nVisEnd;

    uint32_t line_length_;
    uint32_t line_width_;
    uint32_t total_lines_;
    uint32_t page_bytes_offset_;

    DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif // HEXVIEW_H
