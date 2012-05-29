// UMLEditorDemoTemp.cpp : implementation file
//

#include "stdafx.h"
#include "SWGEd.h"
#include "TreDocumentTemplate.h"


// CUMLEditorDemoTemp

IMPLEMENT_DYNAMIC(TreDocumentTemplate, CMultiDocTemplate)
TreDocumentTemplate::TreDocumentTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)

 : CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

TreDocumentTemplate::~TreDocumentTemplate()
{
}

BEGIN_MESSAGE_MAP(TreDocumentTemplate, CMultiDocTemplate)
END_MESSAGE_MAP()

CDocument* TreDocumentTemplate::OpenDocumentFile(LPCTSTR lpszPathName,BOOL )
{
	// We make visible later
	CDocument* pDoc = CMultiDocTemplate::OpenDocumentFile(lpszPathName,FALSE);
	
	CSWGEdApp* pApp = (CSWGEdApp*)AfxGetApp(); 
	//pApp->m_IFramework->SetOpenedDocument(pDoc);	

	return pDoc;
}

