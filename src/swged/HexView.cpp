// HexView.cpp

#include "StdAfx.h"
#include "HexView.h"
#include "HexDoc.h"

IMPLEMENT_DYNCREATE(CHexView, CScrollView)

BEGIN_MESSAGE_MAP(CHexView, CScrollView)
    //{{AFX_MSG_MAP(CHexView)
	ON_WM_KEYDOWN()
	ON_WM_SIZING()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP			
END_MESSAGE_MAP()

void CHexView::OnInitialUpdate()
{
	// Methode der Oberklasse aufrufen
	CScrollView::OnInitialUpdate();

	// Schriftart Courier vorbereiten
    VERIFY(m_Font.CreatePointFont(12, "Courier"));

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

	// Größe des Dokuments in Pixel setzen
    SetScrollSizes(MM_TEXT, CSize(630, 16 * m_dwTotalLines - 1));
}

void CHexView::OnDraw(CDC* pDC)
{
	CRect rect;

	// Größe des Zeichenbereichs ermitteln
	GetClientRect(&rect);

	// Anzahl in Ansicht darstellbarer Zeilen
	if(rect.Height() % 16)
	{
		m_dwPageLines = rect.Height() / 16 + 1;
	}
	else
	{
		m_dwPageLines = rect.Height() / 16;
	}

	// 01-25-2006: prevents garbage being displayed
	if (m_dwTotalLines < m_dwPageLines)
		m_dwPageLines = m_dwTotalLines;

	// Größe des Dokuments (in Pixel) erneut setzen
    SetScrollSizes(MM_TEXT, CSize(630, 16 * m_dwTotalLines - 1),
  		                    CSize(630, 16 * (m_dwPageLines - 1)), CSize(630, 16));

	// Position of Scrollbalken
	m_dwOffset = (GetScrollPos(SB_VERT) & 0xfffffff0);

	// Hole Zeiger auf Dokumentobjekt
	CHexDoc* pDoc = (CHexDoc*) GetDocument();
	ASSERT_VALID(pDoc);

	// Jede Zeile ausgeben
	for(DWORD dwLine = 0, dwOffset = m_dwOffset; dwLine < m_dwPageLines; dwLine++, dwOffset += 16)
    {
		// Anzahl Zeichen in aktueller Zeile
		DWORD dwCount = 16 < (dwCount = pDoc->GetLength() - dwOffset) ? 16 : dwCount;

		CString strLeft, strRight, strFormat;

        for(register DWORD dwChar = 0; dwChar < dwCount; dwChar++)
        {
            // Einzelnes Zeichen hinzufügen
            strLeft  += " %02x";
            strRight += "%c";

            // Umwandlung in (hexadezimales) Zeichen
            auto data = pDoc->GetData();
			strLeft .Format(strFormat = strLeft,  BYTE(data[dwOffset + dwChar]));
            strRight.Format(strFormat = strRight, CHAR(data[dwOffset + dwChar]));

            // Offset einfügen
            strFormat.Format("%06x   ", dwOffset);

            // Ausgabezeile zusammenfügen
            strFormat += strLeft + CString(' ', 49 - 3 * dwChar) + strRight;

			// 01-25-2006: nicer output for '\n', '\r', '\t'
			strFormat.Replace("\r", ".");
			strFormat.Replace("\n", ".");
			strFormat.Replace("\t", ".");
        }

		// Schriftart zur Ausgabe wählen
		pDC->SelectObject(m_Font);
		
        // Zeile in Fenster ausgeben
		pDC->TextOut(10, dwOffset, strFormat);
     }
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
