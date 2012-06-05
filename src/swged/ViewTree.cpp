
#include "stdafx.h"
#include "ViewTree.h"
#include "SWGEd.h"

#include <regex>

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

BOOL CViewTree::OpenSelectedItem()
{
    HTREEITEM hTreeItem = NULL;
    hTreeItem = GetSelectedItem();
    if(NULL != hTreeItem)
    {
        CString strText = GetItemText(hTreeItem);
        if (strText.FindOneOf(_T(".")) >= 0)
        {    
            BuildPath(strText, GetParentItem(hTreeItem));
            OpenItem(strText);
            return TRUE;
        }
    }

    return FALSE;
}

void CViewTree::OpenItem(CString item)
{
    if (IsDatatable(item))
    {
        static_cast<CSWGEdApp*>(AfxGetApp())->GetDatatableDocTemplate()->OpenDocumentFile(item);
    }
    else
    {
        static_cast<CSWGEdApp*>(AfxGetApp())->GetHexDocTemplate()->OpenDocumentFile(item);
    }
}
    
bool CViewTree::IsDatatable(CString& item) const
{
    const std::regex r("^datatables/.*$");

    return std::regex_match(std::string(CT2CA(item)), r);
}

CString CViewTree::GetSelectedItemPath()
{
    CString selected_item_path;
    HTREEITEM selected_item = GetSelectedItem();
    
    if (selected_item)
    {
        selected_item_path = GetItemText(selected_item);    
        BuildPath(selected_item_path, GetParentItem(selected_item));
    }

    return selected_item_path;
}

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
    OpenSelectedItem();
    CTreeCtrl::OnLButtonDblClk(nFlags, point);
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
