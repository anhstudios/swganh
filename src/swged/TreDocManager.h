
#ifndef TRE_DOC_MANAGER_H_
#define TRE_DOC_MANAGER_H_

#include <afxwin.h>

class CTreDocManager : public CDocManager
{
public:
    DECLARE_DYNCREATE(CTreDocManager) 

    CTreDocManager();
    virtual ~CTreDocManager();

	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

#endif
