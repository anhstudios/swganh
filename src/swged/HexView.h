
// DatatableView.h : interface of the CDatatableView class
//

#pragma once

#include <afxwinforms.h>
#include <cstdint>
#include <memory>

#include "TreDoc.h"

#using <Be.Windows.Forms.HexBox.dll>

class CHexView : public Microsoft::VisualC::MFC::CWinFormsView
{
protected: // create from serialization only
	CHexView();
	DECLARE_DYNCREATE(CHexView)

// Attributes
public:
	CTreDoc* GetDocument() const;
	Be::Windows::Forms::HexBox^ GetControl();

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
   	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CHexView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    void OnHexBoxResize(System::Object^ object, System::EventArgs^ e);

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CHexView.cpp
inline CTreDoc* CHexView::GetDocument() const
   { return reinterpret_cast<CTreDoc*>(m_pDocument); }
#endif
