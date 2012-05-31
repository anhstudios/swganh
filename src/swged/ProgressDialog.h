#pragma once

// ProgressDialog dialog

class ProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProgressDialog)

public:
	ProgressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProgressDialog();

    void DisableAbort();
    void EnableAbort();

    void SetProgress(int progress, CString current_task);
    void SetProgress(int progress);

    BOOL IsAborted() const;
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CProgressCtrl progress_bar_;
	CButton	button_abort_;
    CStatic	text_task_;
    bool aborted_;
    
	virtual BOOL OnInitDialog();
	afx_msg void OnAbort();
	DECLARE_MESSAGE_MAP()
};
