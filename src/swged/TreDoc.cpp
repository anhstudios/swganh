// TreDoc.cpp

#include "StdAfx.h"
#include "TreDoc.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CTreDoc, CDocument)

BEGIN_MESSAGE_MAP(CTreDoc, CDocument)
   //{{AFX_MSG_MAP(TreDocument)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTreDoc::CTreDoc() 
{}

CTreDoc::~CTreDoc()
{}

BOOL CTreDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    auto tre_archive = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd())->GetTreArchive();
    if (tre_archive)
    {
        file_data_ = tre_archive->GetResource(lpszPathName);
    }

    return TRUE;
}

void CTreDoc::Serialize(CArchive& archive)
{}

uint32_t CTreDoc::GetLength() const
{
    return file_data_.size();
}
    
const std::vector<char>& CTreDoc::GetData() const
{
    return file_data_;
}