#include "UltimateGrid/ugctrl.h"

#include <vector>

#include <memory>
#include "TreDocument.h"

namespace swganh {
namespace tre {
namespace readers {
    class DatatableReader;
}}}

class DatatableGrid : public CUGCtrl
{

public:


	DatatableGrid();
	~DatatableGrid();
    
    void SetDatatableDoc(TreDocument* datatable_doc);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(MyCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	virtual void OnSetup();
    
private:
    TreDocument* datatable_doc_;
    std::shared_ptr<swganh::tre::readers::DatatableReader> datatable_reader_;
};
