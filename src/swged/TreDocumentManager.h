
#ifndef TRE_DOCUMENT_MANAGER_H_
#define TRE_DOCUMENT_MANAGER_H_

#include <afxwin.h>

class TreDocumentManager : public CDocManager
{
public:
    DECLARE_DYNCREATE(TreDocumentManager) 

    TreDocumentManager();
    virtual ~TreDocumentManager();

	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

#endif
