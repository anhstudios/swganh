// DatatableView.cpp : implementation of the CDatatableView class
//

#include "stdafx.h"

#include "DatatableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatatableView

IMPLEMENT_DYNCREATE(CDatatableView, CView)

BEGIN_MESSAGE_MAP(CDatatableView, CView)
	//{{AFX_MSG_MAP(CDatatableView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatatableView construction/destruction

CDatatableView::CDatatableView()
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

	// do this to the view that will contain Ultimate Grid
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDatatableView drawing

void CDatatableView::OnDraw(CDC* pDC)
{
	CTreDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    
	//// TODO: add draw code for native data here
    //m_ctrl.SetDatatableDoc(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CEx2View diagnostics

#ifdef _DEBUG
void CDatatableView::AssertValid() const
{
	CView::AssertValid();
}

void CDatatableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreDoc* CDatatableView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreDoc)));
	return (CTreDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx2View message handlers

int CDatatableView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	////: Create the Grid control in the client rectangle
	//m_ctrl.CreateGrid(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,1234);
	
	return 0;
}

void CDatatableView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	////: Adjust the size of the Grid control
	//m_ctrl.MoveWindow( 0, 0, cx, cy );
	
}

void CDatatableView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	Invalidate();		// invalidate window
	UpdateWindow();	// force repaint
}