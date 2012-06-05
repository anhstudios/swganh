// UMLEditorDemoTemp.cpp : implementation file
//

#include "stdafx.h"
#include "SWGEd.h"
#include "TreDocTemplate.h"


// CUMLEditorDemoTemp

IMPLEMENT_DYNAMIC(CTreDocTemplate, CMultiDocTemplate)
CTreDocTemplate::CTreDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)

 : CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CTreDocTemplate::~CTreDocTemplate()
{
}

BEGIN_MESSAGE_MAP(CTreDocTemplate, CMultiDocTemplate)
END_MESSAGE_MAP()

CDocument* CTreDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName,BOOL )
{
	// We make visible later
	CDocument* pDoc = CMultiDocTemplate::OpenDocumentFile(lpszPathName,FALSE);
	
	CSWGEdApp* pApp = (CSWGEdApp*)AfxGetApp(); 
	//pApp->m_IFramework->SetOpenedDocument(pDoc);	

	return pDoc;
}

