// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "swged.h"
#include "ProgressDialog.h"
#include "afxdialogex.h"


// ProgressDialog dialog

IMPLEMENT_DYNAMIC(ProgressDialog, CDialogEx)

ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROGRESS, pParent)
{

}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ABORT, button_abort_);
    DDX_Control(pDX, IDC_TASK, text_task_);
    DDX_Control(pDX, IDC_PROGRESSBAR, progress_bar_);
}


BEGIN_MESSAGE_MAP(ProgressDialog, CDialogEx)
	ON_BN_CLICKED(IDC_ABORT, OnAbort)
END_MESSAGE_MAP()

void ProgressDialog::SetProgress(int progress, CString current_task)
{
    progress_bar_.SetPos(progress);
    text_task_.SetWindowText(current_task);
}

void ProgressDialog::SetProgress(int progress)
{
    progress_bar_.SetPos(progress);
}

BOOL ProgressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CenterWindow();

	// set ranges etc
    progress_bar_.SetRange( 0, 100 );
    text_task_.SetWindowText( "" );
	aborted_ = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ProgressDialog::OnAbort() 
{
	aborted_ = TRUE;	
}

BOOL ProgressDialog::IsAborted() const
{
	return aborted_;	
}

void ProgressDialog::DisableAbort()
{
    button_abort_.EnableWindow(false);
}

void ProgressDialog::EnableAbort()
{
    button_abort_.EnableWindow(true);
}

// ProgressDialog message handlers
