
#include "TreDocManager.h"

IMPLEMENT_DYNCREATE(CTreDocManager, CDocManager) 

CTreDocManager::CTreDocManager()   
{}   
   
   
CTreDocManager::~CTreDocManager()   
{}   

CDocument* CTreDocManager::OpenDocumentFile(LPCTSTR lpszFileName)
{
	if (lpszFileName == NULL)
	{
		AfxThrowInvalidArgException();
	}
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	TCHAR szPath[_MAX_PATH];
	ASSERT(AtlStrLen(lpszFileName) < _countof(szPath));
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	Checked::tcsncpy_s(szPath, _countof(szPath), lpszFileName, _TRUNCATE);
	LPTSTR lpszLast = _tcsrchr(szPath, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;
	
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		CDocTemplate::Confidence match;
		ASSERT(pOpenDocument == NULL);
		match = pTemplate->MatchDocType(szPath, pOpenDocument);
		if (match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if (match == CDocTemplate::yesAlreadyOpen)
			break;      // stop here
	}

	if (pOpenDocument != NULL)
	{
		POSITION posOpenDoc = pOpenDocument->GetFirstViewPosition();
		if (posOpenDoc != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(posOpenDoc); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame == NULL)
				TRACE(traceAppMsg, 0, "Error: Can not find a frame for document to activate.\n");
			else
			{
			    pFrame->ActivateFrame();

			    if (pFrame->GetParent() != NULL)
			    {
			    	CFrameWnd* pAppFrame;
			    	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
			    	{
			    		ASSERT_KINDOF(CFrameWnd, pAppFrame);
			    		pAppFrame->ActivateFrame();
			    	}
			    }
			}
		}
		else
			TRACE(traceAppMsg, 0, "Error: Can not find a view for document to activate.\n");

		return pOpenDocument;
	}

	if (pBestTemplate == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return NULL;
	}

	return pBestTemplate->OpenDocumentFile(szPath);
}
