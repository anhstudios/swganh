// HexView.cpp

#include "StdAfx.h"
#include "HexView.h"
#include "HexDoc.h"

#include <iomanip>
#include <sstream>
#include "anh/byte_buffer.h"

IMPLEMENT_DYNCREATE(CHexView, CScrollView)

BEGIN_MESSAGE_MAP(CHexView, CScrollView)
    //{{AFX_MSG_MAP(CHexView)
	ON_WM_KEYDOWN()
	ON_WM_SIZING()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP			
END_MESSAGE_MAP()

BOOL CHexView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// load font
	m_Font.CreatePointFont( 100, "Terminal" );

	return CScrollView::PreCreateWindow(cs);
}

void CHexView::OnInitialUpdate()
{
	// Methode der Oberklasse aufrufen
	CScrollView::OnInitialUpdate();
    
    // Hole Zeiger auf Dokument
    CHexDoc* pDoc = (CHexDoc*) GetDocument();
    ASSERT_VALID(pDoc);

	// Datei besteht aus so vielen Zeilen
    if(pDoc->GetLength() % 16)
	{
        m_dwTotalLines = pDoc->GetLength() / 16 + 1;
	}
    else
	{
        m_dwTotalLines = pDoc->GetLength() / 16;
	}
    
	CRect rect;
	GetClientRect(&rect);

	// Größe des Dokuments in Pixel setzen
    SetScrollSizes(MM_TEXT, CSize(rect.Width(), rect.Height()));
}

void CHexView::OnDraw(CDC* pDC)
{
    // Get the surface the view will be drawn on
	CRect rect;
	GetClientRect(&rect);

    // Get the height for the current font
    LOGFONT lf;
    m_Font.GetLogFont(&lf);

    uint32_t font_height = 16;

	// Anzahl in Ansicht darstellbarer Zeilen
	if(rect.Height() % font_height)
	{
		m_dwPageLines = rect.Height() / font_height + 1;
	}
	else
	{
		m_dwPageLines = rect.Height() / font_height;
	}

	// 01-25-2006: prevents garbage being displayed
	if (m_dwTotalLines < m_dwPageLines)
		m_dwPageLines = m_dwTotalLines;

	// Größe des Dokuments (in Pixel) erneut setzen
    SetScrollSizes(MM_TEXT, CSize(rect.Width(), rect.Height()),
  		                    CSize(rect.Width(), font_height * (m_dwPageLines - 1)), CSize(rect.Width(), font_height));

	// Position of Scrollbalken
	m_dwOffset = (GetScrollPos(SB_VERT) & 0xfffffff0);

	// Hole Zeiger auf Dokumentobjekt
	CHexDoc* pDoc = (CHexDoc*) GetDocument();
	ASSERT_VALID(pDoc);

    std::ostringstream ss;
    size_t length = pDoc->GetLength();
    auto data = pDoc->GetData();
    
    // Calculate the number of lines and extra bits.
    int16_t lines = static_cast<int16_t>(length / 16);
    int16_t extra = static_cast<int16_t>(length % 16);
    
    // Save the formatting state of the stream.
    std::ios_base::fmtflags flags = ss.flags(ss.hex);
    char fill = ss.fill('0');
    std::streamsize width = ss.width(2);
    int16_t offset_lines = (int16_t)m_dwOffset / 16;

    // The byte buffer should be printed out in lines of 16 characters and display
    // both hex and ascii values for each character, see most hex editors for
    // reference.
    for (int32_t i = 0, dwOffset = m_dwOffset; i < (int32_t)m_dwPageLines; i++, dwOffset += 16) {
        // Print out a line number.
        ss << std::setw(4) << (dwOffset) << ":   ";
        
        // Loop through the characters of this line (max 16)
        for (int16_t j = 0; j < 16; ++j) {
            // For the last line there may not be 16 characters. In this case filler
            // whitespace should be added to keep column widths consistent.
            if (i + offset_lines == lines && j >= extra) {
                ss << "   ";
            } else {
                ss << std::setw(2)
                    << static_cast<unsigned>(data[(dwOffset)+j]) << " ";
            }
        }
        
        ss << "  ";
        
        for (int16_t k = 0; k < 16; ++k) {
            // For the last line there may not be 16 characters. In this case
            // print a ' ' for these characters.
            if ((i + offset_lines == lines) & (k >= extra)) {
                ss << " ";
            } else if (data[(dwOffset)+k] < ' ' || data[(dwOffset)+k] > '~') {
                // Else if it's not an ascii value print a '.'
                ss << '.';
            } else {
                ss << data[(dwOffset)+k];
            }
        }
        
        pDC->SelectObject(m_Font);
        pDC->TextOut(10, dwOffset, _T(ss.str().c_str()));

        ss.str("");
        ss.clear();
    }
    
    // Return formatting of stream to its previous state.
    ss.flags(flags);
    ss.fill(fill);
    ss.width(width);
}

void CHexView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Fix für Fehler Q152252
    if(nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
    {
        SCROLLINFO info;

		info.cbSize = sizeof(SCROLLINFO);
        info.fMask  = SIF_TRACKPOS;

		::GetScrollInfo(m_hWnd, SB_VERT, &info);

        nPos = info.nTrackPos;
    }

	// Ansicht wird nur zeilenweise bewegt
	nPos &= 0xfffffff0;

	// Methode der Oberklasse aufrufen
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CHexView::OnSizing(UINT, LPRECT)
{
	// Scrollbalken neu positionieren
	SetScrollPos(SB_VERT, m_dwOffset);

	// Veranlasse Neuzeichnen der Ansicht
	Invalidate();
}

void CHexView::OnKeyDown(UINT nChar, UINT, UINT)
{
	switch(nChar)
	{
		case VK_PRIOR:
		{
			OnVScroll(SB_PAGEUP, 0, 0);
			break;
		}

		case VK_NEXT:
		{
			OnVScroll(SB_PAGEDOWN, 0, 0);
			break;
		}

		case VK_END:
		{
			OnVScroll(SB_BOTTOM, 0, 0);
			break;
		}

		case VK_HOME:
		{
			OnVScroll(SB_TOP, 0, 0);
			break;
		}

		case VK_UP:
		{
			OnVScroll(SB_LINEUP, 0, 0);
			break;
		}

		case VK_DOWN:
		{
			OnVScroll(SB_LINEDOWN, 0, 0);
			break;
		}
	}
}

void CHexView::UpdateScrollbars()
{	
	// calculate how many bytes can be displayed across
	CRect rcClient;
	GetClientRect( &rcClient );
	m_nLineLength = ( rcClient.Width() - 80 ) / 40;

	// make calculations
	CSize sizeTotal;
	sizeTotal.cx = 1;
    sizeTotal.cy = GetDocument()->GetLength() / m_nLineLength * 20 + 100;
	m_nLineCount = GetDocument()->GetLength() / m_nLineLength;

	// set scrollbar sizes
	SCROLLINFO siInfo;
	siInfo.cbSize = sizeof( SCROLLINFO );
	siInfo.fMask = SIF_RANGE;
	siInfo.nMin = 0; siInfo.nMax = sizeTotal.cy;
	SetScrollSizes(MM_TEXT, sizeTotal); // this manages most of the tedious stuff
	SetScrollInfo( SB_VERT, &siInfo ); // here we actually set the size
}
