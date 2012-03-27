
#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "swganh/tre/readers/datatable_reader.h"
#include "swganh/tre/tre_archive.h"
#include "swganh/tre/tre_reader.h"

using namespace std;
using namespace swganh::tre;
using namespace swganh::tre::readers;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <path to swg live file> <datatable file>" << endl;
        exit(0);
    }
    
    auto start_time = chrono::high_resolution_clock::now();

    string swg_live_file(argv[1]);
    string datatable_file(argv[2]);

    {
        TreArchive archive(swg_live_file);
        
        DatatableReader reader(archive.GetResource(datatable_file));
        
        cout << "\nLoaded datatable file (" << datatable_file << "):\n";
        cout << "    Rows: " << reader.CountRows() << "\n";
        
        auto row = reader.GetRow();

        for_each(
            begin(row),
            end(row),
            [] (const std::map<std::string, DatatableCell*>::value_type& rowdata)
        {
            cout << "[" << rowdata.first << "] = " << rowdata.second->ToString() << "\n";
        });

        cout << "\nAccess row cell directly:\n";
        cout << "    [commandName] = " << row["commandName"]->GetValue<string>() << "\n\n";
    }

    auto stop_time = chrono::high_resolution_clock::now();

    cout << "Duration: " << 
        chrono::duration_cast<chrono::milliseconds>
            (stop_time - start_time).count() << "ms" << endl;

    return 0;
}
