
#include "stdafx.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    HTREEITEM hTreeItem = NULL;
    hTreeItem = GetSelectedItem();
    if(NULL != hTreeItem)
    {
        CString strText = GetItemText(hTreeItem);
        if (strText.FindOneOf(_T(".")) >= 0)
        {
            HTREEITEM selected_item = GetSelectedItem();
            auto selected_item_text = GetItemText(selected_item);
            auto selected_item_path = selected_item_text;
    
            BuildPath(selected_item_path, GetParentItem(selected_item));

            AfxGetApp()->OpenDocumentFile(selected_item_path);
        }
        else
        {
            CTreeCtrl::OnLButtonDblClk(nFlags, point);
        }
    }
}

void CViewTree::BuildPath(CString& path, HTREEITEM node)
{
    if (!node)
        return;

    HTREEITEM parent = GetParentItem(node);
    CString buffer = GetItemText(node);

    buffer += "/";

    buffer += path;
    path = buffer;

    if (parent)
    {
        BuildPath(path, parent);
    }
}
