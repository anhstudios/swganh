#ifndef TRE_DOC_TEMPLATE_H_
#define TRE_DOC_TEMPLATE_H_

// CUMLEditorDemoTemp
#include <afxwin.h>

class CTreDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CTreDocTemplate)

public:
	CTreDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CTreDocTemplate();
	HWND CreateView(int nID);
protected:
// Overrides
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName,BOOL bMakeVisible = TRUE);
	DECLARE_MESSAGE_MAP()
};

#endif
