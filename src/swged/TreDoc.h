// TreDoc.h

#ifndef TRE_DOC_H_
#define TRE_DOC_H_

#include <afxwin.h>
#include <cstdint>
#include <vector>

class CTreDoc : public CDocument
{
	DECLARE_DYNCREATE(CTreDoc)

protected:
	CTreDoc();
	~CTreDoc();

protected:
	//{{AFX_VIRTUAL(TreDocument)
    BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Serialize(CArchive& archive);
	//}}AFX_VIRTUAL
    
    std::vector<unsigned char> file_data_;
public:
    uint32_t GetLength() const;
    const std::vector<unsigned char>& GetData() const;

    DECLARE_MESSAGE_MAP()
};

#endif // TRE_DOC_H_