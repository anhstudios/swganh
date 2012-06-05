
// DatatableView.cpp : implementation of the CDatatableView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SWGEd.h"
#endif

#include "TreDoc.h"
#include "DatatableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDatatableView

IMPLEMENT_DYNCREATE(CDatatableView, Microsoft::VisualC::MFC::CWinFormsView)

BEGIN_MESSAGE_MAP(CDatatableView, Microsoft::VisualC::MFC::CWinFormsView)
	// Standard printing commands
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDatatableView construction/destruction

CDatatableView::CDatatableView()
    : Microsoft::VisualC::MFC::CWinFormsView(System::Windows::Forms::DataGridView::typeid)
{
	// TODO: add construction code here

}

CDatatableView::~CDatatableView()
{
}

BOOL CDatatableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDatatableView drawing

void CDatatableView::OnDraw(CDC* /*pDC*/)
{
	CTreDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDatatableView printing
System::Windows::Forms::DataGridView^ CDatatableView::GetControl()
{
	System::Windows::Forms::Control^ control = CWinFormsView::GetControl();
	return safe_cast<System::Windows::Forms::DataGridView^>(control);
}

void CDatatableView::OnInitialUpdate()
{
    
    System::Windows::Forms::DataGridViewColumn^ dgvCol1 = 
        gcnew System::Windows::Forms::DataGridViewColumn(gcnew System::Windows::Forms::DataGridViewTextBoxCell());
    dgvCol1->HeaderText = L"Column1";
    dgvCol1->Name = L"Column1";


    GetControl()->Columns->Add(dgvCol1); 
}

void CDatatableView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDatatableView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDatatableView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDatatableView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDatatableView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDatatableView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDatatableView diagnostics

#ifdef _DEBUG
void CDatatableView::AssertValid() const
{
	CView::AssertValid();
}

void CDatatableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreDoc* CDatatableView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreDoc)));
	return (CTreDoc*)m_pDocument;
}
#endif //_DEBUG


// CDatatableView message handlers
