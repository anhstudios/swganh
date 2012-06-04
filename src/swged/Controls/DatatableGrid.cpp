/***************************************************
****************************************************
Skeleton Class for a Derived MyCug
****************************************************
****************************************************/

#include "stdafx.h"
//#include "uglibsel.h"
#include "resource.h"
#include "DatatableGrid.h"
#include "swganh/tre/readers/datatable_reader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(DatatableGrid,CUGCtrl)
	//{{AFX_MSG_MAP(DatatableGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
****************************************************/
DatatableGrid::DatatableGrid()
    : datatable_doc_(nullptr)
{
}
/***************************************************
****************************************************/
DatatableGrid::~DatatableGrid()
{
	UGXPThemes::CleanUp();
}

/***************************************************
OnSetup
	This function is called just after the grid window 
	is created or attached to a dialog item.
	It can be used to initially setup the grid
****************************************************/
void DatatableGrid::OnSetup(){
    if (!datatable_doc_)
        return;
    
    swganh::tre::readers::DatatableReader reader(datatable_doc_->GetData());

    int rows = reader.CountRows();
    int columns = reader.CountColumns();
    int i = 0, j = 0;
    CString temp;
    CUGCell cell;

    SetNumberRows(rows);
    SetNumberCols(columns);

    GetCell(0, 1, &cell);
    
    while(reader.Next())
    {
        auto row = reader.GetRow();
        
        j = 0;

        for (auto& row_cell : row)
        {
            cell.SetText(_T(row_cell.second->ToString().c_str()));
            SetCell(j, i, &cell);
            ++j;
        }

        ++i;
    }

    auto column_names = reader.GetColumnNames();
    for (i = 0; i < columns; i++)
    {
        cell.SetText(_T(column_names[i].c_str()));
        SetCell(i, -1, &cell);
    }
    
	for (j = 0; j < rows; j++) {
		temp.Format("%d", (j+1));
		cell.SetText(temp);
		SetCell(-1, j, &cell);
	}

    //for_each(
    //    begin(row),
    //    end(row),
    //    [] (const std::map<std::string, DatatableCell*>::value_type& rowdata)
    //{
    //    cout << "[" << rowdata.first << "] = " << rowdata.second->ToString() << "\n";
    //});

}

void DatatableGrid::SetDatatableDoc(TreDocument* datatable_doc)
{
    datatable_doc_ = datatable_doc;
    OnSetup();
}