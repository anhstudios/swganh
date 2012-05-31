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
	CScrollView::OnInitialUpdate();
    UpdateScrollbars();
}

void CHexView::OnDraw(CDC* pDC)
{
    // Get the surface the view will be drawn on
	CRect rect;
	GetClientRect(&rect);

    uint32_t page_lines = 0;
	if(rect.Height() % 16)
	{
		page_lines = rect.Height() / 16 + 1;
	}
	else
	{
		page_lines = rect.Height() / 16;
	}

	if (total_lines_ < page_lines)
		page_lines = total_lines_;
        
    SetScrollSizes(MM_TEXT, CSize(line_width_, 16 * total_lines_ - 1),
  		                    CSize(line_width_, 16 * (page_lines - 1)), CSize(line_width_, 16));
        
	page_bytes_offset_ = (GetScrollPos(SB_VERT) & 0xfffffff0);
    uint32_t page_line_offset = page_bytes_offset_ / 16;

	CHexDoc* pDoc = GetDocument();
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

    // The byte buffer should be printed out in lines of 16 characters and display
    // both hex and ascii values for each character, see most hex editors for
    // reference.
    for (int32_t i = 0, bytes_offset = page_bytes_offset_; i < (int32_t)page_lines; i++, bytes_offset += 16) {
        // Print out a line number.
        ss << std::setw(4) << (bytes_offset) << ":   ";
        
        // Loop through the characters of this line (max 16)
        for (int16_t j = 0; j < 16; ++j) {
            // For the last line there may not be 16 characters. In this case filler
            // whitespace should be added to keep column widths consistent.
            if (i + page_line_offset == lines && j >= extra) {
                ss << "   ";
            } else {
                ss << std::setw(2)
                    << static_cast<unsigned>(data[(bytes_offset)+j]) << " ";
            }
        }
        
        ss << "  ";
        
        for (int16_t k = 0; k < 16; ++k) {
            // For the last line there may not be 16 characters. In this case
            // print a ' ' for these characters.
            if ((i + page_line_offset == lines) & (k >= extra)) {
                ss << " ";
            } else if (data[(bytes_offset)+k] < ' ' || data[(bytes_offset)+k] > '~') {
                // Else if it's not an ascii value print a '.'
                ss << '.';
            } else {
                ss << data[(bytes_offset)+k];
            }
        }
        
        pDC->SelectObject(m_Font);
        pDC->TextOut(10, bytes_offset, _T(ss.str().c_str()));

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
	SetScrollPos(SB_VERT, page_bytes_offset_);

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

    // line length = 74 character width * 20px per character
    line_length_ = 16;
    line_width_ = 78 * 8;
    total_lines_ = GetDocument()->GetLength() / line_length_;
    
    if (GetDocument()->GetLength() % line_length_)
        ++total_lines_;

    CSize size_total;
    size_total.cx = line_width_;
    size_total.cy = total_lines_ * 20;

    SCROLLINFO scroll_info;
    scroll_info.fMask = SIF_RANGE;
    scroll_info.nMin = 0;
    scroll_info.nMax = size_total.cy;

    SetScrollSizes(MM_TEXT, size_total);
    //SetScrollInfo(SB_VERT, &scroll_info);
}
