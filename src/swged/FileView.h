
#pragma once
#include "ProgressDialog.h"
#include "ViewTree.h"
#include <thread>
#include "swganh/tre/tre_archive.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// Construction
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
    void SetTreArchive(swganh::tre::TreArchive* archive);

// Attributes
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

// Implementation
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnFileExport();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnOpenEnvironment();
	DECLARE_MESSAGE_MAP()

private:

    void UpdateProgressBar(double total, double completed);

    std::unique_ptr<ProgressDialog> dlg_progress_;
    swganh::tre::TreArchive* archive_;
    std::vector<std::string> file_listing_;

    std::unique_ptr<std::thread> file_listing_loader_;
};

