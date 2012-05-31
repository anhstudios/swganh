#ifndef TRE_DOCUMENT_TEMPLATE_H_
#define TRE_DOCUMENT_TEMPLATE_H_

// CUMLEditorDemoTemp
#include <afxwin.h>

class TreDocumentTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(TreDocumentTemplate)

public:
	TreDocumentTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~TreDocumentTemplate();
	HWND CreateView(int nID);
protected:
// Overrides
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName,BOOL bMakeVisible = TRUE);
	DECLARE_MESSAGE_MAP()
};

#endif
