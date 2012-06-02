#include "UltimateGrid/ugctrl.h"


class DatatableGrid : public CUGCtrl
{

public:


	DatatableGrid();
	~DatatableGrid();

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
};
