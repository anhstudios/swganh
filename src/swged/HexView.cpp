
// DatatableView.cpp : implementation of the CDatatableView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SWGEd.h"
#endif


#include "TreDoc.h"
#include "HexView.h"
#include "swganh/tre/readers/datatable_reader.h"

#include <cliext/vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ref class CHexViewWrapper
{
    CHexView* view_;

public:
    CHexViewWrapper(CHexView* view)
        : view_(view)
    {}
    
    void HexBoxResize(System::Object^ object, System::EventArgs^ e)
    {
        view_->OnHexBoxResize(object, e);
    }
};

// CDatatableView

IMPLEMENT_DYNCREATE(CHexView, Microsoft::VisualC::MFC::CWinFormsView)

BEGIN_MESSAGE_MAP(CHexView, Microsoft::VisualC::MFC::CWinFormsView)
	// Standard printing commands
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CHexView construction/destruction

CHexView::CHexView()
    : Microsoft::VisualC::MFC::CWinFormsView(Be::Windows::Forms::HexBox::typeid)
{
	// TODO: add construction code here

}

CHexView::~CHexView()
{
}

BOOL CHexView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDatatableView drawing

void CHexView::OnDraw(CDC* /*pDC*/)
{
	CTreDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
    GetControl()->Refresh();
}


// CDatatableView printing
Be::Windows::Forms::HexBox^ CHexView::GetControl()
{
	System::Windows::Forms::Control^ control = CWinFormsView::GetControl();
	return safe_cast<Be::Windows::Forms::HexBox^>(control);
}

void CHexView::OnInitialUpdate()
{
	CTreDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    auto document_data = pDoc->GetData();
    auto my_arr = gcnew cliext::vector<unsigned char>(std::begin(document_data), std::end(document_data));    

    Be::Windows::Forms::DynamicByteProvider^ collection = gcnew Be::Windows::Forms::DynamicByteProvider(my_arr->to_array());

    auto control = GetControl();
    control->Dock = System::Windows::Forms::DockStyle::Fill;
    control->Font = gcnew System::Drawing::Font("Consolas", 10.0F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 0);
    control->UseFixedBytesPerLine = true;
    control->VScrollBarVisible = true;
    control->ByteProvider = collection;
    control->StringViewVisible = true;

    auto wrapper = gcnew CHexViewWrapper(this);

    control->Resize += gcnew System::EventHandler(wrapper, &CHexViewWrapper::HexBoxResize);
}

void CHexView::OnHexBoxResize(System::Object^ object, System::EventArgs^ e)
{
    auto control = GetControl();

    control->BytesPerLine = (System::Int32)(((System::Double)control->ClientSize.Width - (System::Double)47) / (System::Double)32);
}

void CHexView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHexView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHexView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHexView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHexView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHexView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHexView diagnostics

#ifdef _DEBUG
void CHexView::AssertValid() const
{
	CView::AssertValid();
}

void CHexView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreDoc* CHexView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreDoc)));
	return (CTreDoc*)m_pDocument;
}
#endif //_DEBUG


// CHexView message handlers
