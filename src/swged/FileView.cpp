
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "SWGEd.h"

#include <map>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
    : archive_(nullptr)
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
    std::map<int, std::map<std::string, HTREEITEM>> directory_map;
    
    for(auto& file : file_listing_)
    {
        std::vector<std::string> path_data;
        boost::split(path_data, file, boost::is_any_of("/"));
    
        HTREEITEM previous = NULL;
        for (uint32_t i = 0; i < path_data.size(); ++i)
        {
            if (i + 1 != path_data.size())
            {
                auto find_iter = directory_map[i].find(path_data[i]);
                if (find_iter == directory_map[i].end())
                {
                    if (previous)
                    {
                        previous = m_wndFileView.InsertItem(_T(path_data[i].c_str()), 0, 0, previous);
                    } else {
                        previous = m_wndFileView.InsertItem(_T(path_data[i].c_str()), 0, 0);
                    }
    
                    directory_map[i].insert(std::make_pair(path_data[i], previous));
                }
                else
                {
                    previous = find_iter->second;
                }
            } else {                
                if (previous)
                {
                    m_wndFileView.InsertItem(_T(path_data[i].c_str()), 2, 2, previous);
                } else {
                    m_wndFileView.InsertItem(_T(path_data[i].c_str()), 2, 2);
                }
    
                previous = NULL;
            }
        }
    }
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CFileView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CFileView::OnFileOpenWith()
{    
	// TODO: Add your command handler code here
}


void CFileView::OnFileExport()
{    
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
    HTREEITEM selected_item = pWndTree->GetSelectedItem();
    auto selected_item_text = pWndTree->GetItemText(selected_item);
    auto selected_item_path = selected_item_text;
    
    BuildPath(selected_item_path, pWndTree->GetParentItem(selected_item));

    CFileDialog file_dialog(FALSE, NULL, selected_item_text, OFN_HIDEREADONLY, _T("All files (*.*)|*.*||"));
    
    file_dialog.m_ofn.Flags |= OFN_ENABLETEMPLATE;  
    file_dialog.m_ofn.hInstance = AfxGetInstanceHandle();  
    file_dialog.m_ofn.lpTemplateName = _T("DIALOG_PREVIEW");  
    file_dialog.m_ofn.lpstrTitle = "Export file";
  
    if (file_dialog.DoModal() == IDOK)
    {
        auto resource = archive_->GetResource(selected_item_path.GetString());
        auto filename = file_dialog.GetPathName();

        std::ofstream ofs(filename, std::ios::out | std::ios::binary);

        if (resource.size() > 0)
        {
            ofs.write(&resource[0], resource.size());
        }
    }
}

void CFileView::OnOpenEnvironment()
{
	AfxMessageBox(_T("Open Environment...."));

}

void CFileView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::SetTreArchive(swganh::tre::TreArchive* archive)
{
    archive_ = archive;

    std::async(std::launch::async, [=] () {
        file_listing_ = archive_->GetAvailableResources();
        std::sort(std::begin(file_listing_), std::end(file_listing_));
        FillFileView();    
    });
}

void CFileView::BuildPath(CString& path, HTREEITEM node)
{    
	CTreeCtrl* tree_control = (CTreeCtrl*) &m_wndFileView;
    
    HTREEITEM parent = tree_control->GetParentItem(node);
    CString buffer = tree_control->GetItemText(node);

    buffer += "/";

    buffer += path;
    path = buffer;

    if (parent)
    {
        BuildPath(path, parent);
    }
}
