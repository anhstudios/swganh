
// DatatableView.h : interface of the CDatatableView class
//

#pragma once

#include <afxwinforms.h>
#using <System.Windows.Forms.dll>

class CDatatableView : public Microsoft::VisualC::MFC::CWinFormsView
{
protected: // create from serialization only
	CDatatableView();
	DECLARE_DYNCREATE(CDatatableView)

// Attributes
public:
	CTreDoc* GetDocument() const;
	System::Windows::Forms::DataGridView^ GetControl();

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
	virtual ~CDatatableView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CDatatableView.cpp
inline CTreDoc* CDatatableView::GetDocument() const
   { return reinterpret_cast<CTreDoc*>(m_pDocument); }
#endif

