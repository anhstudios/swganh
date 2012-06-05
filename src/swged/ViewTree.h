
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CViewTree : public CTreeCtrl
{
// Construction
public:
	CViewTree();

    virtual BOOL OpenSelectedItem();
    virtual CString GetSelectedItemPath();

    virtual void OpenItem(CString item);

    virtual bool IsDatatable(CString& item) const;

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CViewTree();
    void BuildPath(CString& path, HTREEITEM node);

protected:
    void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};