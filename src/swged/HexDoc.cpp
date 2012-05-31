// HexDoc.h

#include "StdAfx.h"
#include "HexDoc.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CHexDoc, CDocument)

BEGIN_MESSAGE_MAP(CHexDoc, CDocument)
   //{{AFX_MSG_MAP(CHexDoc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CHexDoc::CHexDoc() 
{}

CHexDoc::~CHexDoc()
{}

BOOL CHexDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    auto tre_archive = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd())->GetTreArchive();
    if (tre_archive)
    {
        file_data_ = tre_archive->GetResource(lpszPathName);
    }

    return TRUE;
}

void CHexDoc::Serialize(CArchive& archive)
{}

uint32_t CHexDoc::GetLength() const
{
    return file_data_.size();
}
    
const std::vector<unsigned char>& CHexDoc::GetData() const
{
    return file_data_;
}