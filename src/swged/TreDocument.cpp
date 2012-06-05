// HexDoc.h

#include "StdAfx.h"
#include "TreDocument.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(TreDocument, CDocument)

BEGIN_MESSAGE_MAP(TreDocument, CDocument)
   //{{AFX_MSG_MAP(TreDocument)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

TreDocument::TreDocument() 
{}

TreDocument::~TreDocument()
{}

BOOL TreDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    auto tre_archive = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd())->GetTreArchive();
    if (tre_archive)
    {
        file_data_ = tre_archive->GetResource(lpszPathName);
    }

    return TRUE;
}

void TreDocument::Serialize(CArchive& archive)
{}

uint32_t TreDocument::GetLength() const
{
    return file_data_.size();
}
    
const std::vector<unsigned char>& TreDocument::GetData() const
{
    return file_data_;
}