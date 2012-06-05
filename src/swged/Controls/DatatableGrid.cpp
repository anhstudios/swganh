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
    , datatable_reader_(nullptr)
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
    if (!datatable_reader_)
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
    
    swganh::tre::readers::DatatableRow row;
    auto column_names = reader.GetColumnNames();

    // Print out the column names
    for (j = 0; j < columns; j++)
    {
        SetCellText(j, -1, column_names[j]);
    }

    while(reader.Next())
    {
        // Print the row count in the left margin
		temp.Format("%d", (i+1));
        QuickSetText(-1, i, temp);

        row = reader.GetRow();

        for (j = 0; j < columns; ++j)
        {
            SetCellText(j, i, row[column_names[j]]->ToString());
        }

        ++i;
    }
}

void DatatableGrid::SetDatatableDoc(TreDocument* datatable_doc)
{
    if (!datatable_doc)
        return;

    datatable_doc_ = datatable_doc;
    
    datatable_reader_ = std::make_shared<swganh::tre::readers::DatatableReader>(datatable_doc_->GetData());
    OnSetup();
}

void DatatableGrid::SetCellText(int column, int row, const std::string& text)
{    
    int col_width = text.length() * 8;
    
    if (GetColWidth(column) < col_width)
        SetColWidth(column, col_width);

    QuickSetText(column, row, _T(text.c_str()));
}