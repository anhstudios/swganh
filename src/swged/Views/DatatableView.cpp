
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
#include "swganh/tre/readers/datatable_reader.h"

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
    GetControl()->Refresh();
}


// CDatatableView printing
System::Windows::Forms::DataGridView^ CDatatableView::GetControl()
{
	System::Windows::Forms::Control^ control = CWinFormsView::GetControl();
	return safe_cast<System::Windows::Forms::DataGridView^>(control);
}

void CDatatableView::OnInitialUpdate()
{
	CTreDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    
    
    GetControl()->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
    GetControl()->Dock = System::Windows::Forms::DockStyle::Fill;

    swganh::tre::readers::DatatableRow row;
    swganh::tre::readers::DatatableReader reader(pDoc->GetData());
    auto column_names = reader.GetColumnNames();

    for(auto& column_name : column_names)
    {
        System::Windows::Forms::DataGridViewColumn^ dgvCol1 = 
            gcnew System::Windows::Forms::DataGridViewColumn(gcnew System::Windows::Forms::DataGridViewTextBoxCell());

        dgvCol1->HeaderText = gcnew System::String(column_name.c_str());
        dgvCol1->Name = gcnew System::String(column_name.c_str());


        GetControl()->Columns->Add(dgvCol1);
    }

    while(reader.Next())
    {
        row = reader.GetRow();
        
        System::Windows::Forms::DataGridViewRow^ data_row = 
            gcnew System::Windows::Forms::DataGridViewRow();

        for (uint32_t i = 0; i < column_names.size(); ++i)
        {
            auto cell = gcnew System::Windows::Forms::DataGridViewTextBoxCell();
            cell->Value = gcnew System::String(row[column_names[i]]->ToString().c_str());
            data_row->Cells->Add(cell);
        }

        GetControl()->Rows->Add(data_row);
    }
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
