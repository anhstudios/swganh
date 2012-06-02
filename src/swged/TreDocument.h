// HexDoc.h

#ifndef TRE_DOCUMENT_H_
#define TRE_DOCUMENT_H_

#include <afxwin.h>
#include <cstdint>
#include <vector>

class TreDocument : public CDocument
{
	DECLARE_DYNCREATE(TreDocument)

protected:
	TreDocument();
	~TreDocument();

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

#endif // TRE_DOCUMENT_H_
