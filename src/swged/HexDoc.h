// HexDoc.h

#ifndef HEXDOC_H
#define HEXDOC_H

#include <afxwin.h>
#include <cstdint>
#include <vector>

class CHexDoc : public CDocument
{
	DECLARE_DYNCREATE(CHexDoc)

protected:
	CHexDoc();
	~CHexDoc();

protected:
	//{{AFX_VIRTUAL(CHexDoc)
    BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Serialize(CArchive& archive);
	//}}AFX_VIRTUAL
    
    std::vector<char> file_data_;
public:
    uint32_t GetLength() const;
    const std::vector<char>& GetData() const;

    DECLARE_MESSAGE_MAP()
};

#endif // HEXDOC_H
