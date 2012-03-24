
#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "swganh/tre/tre_archive.h"
#include "swganh/tre/tre_reader.h"

using namespace std;
using namespace swganh::tre;

const string resource_name = "appearance/mesh/ply_corl_house_lg_s01_fp1_r0_mesh_r0_l2.msh";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to swg live file>" << endl;
        exit(0);
    }
    
    auto start_time = chrono::high_resolution_clock::now();

    string swg_live_file(argv[1]);

    {
        TreArchive archive(swg_live_file);

        auto data = archive.GetResource(resource_name);

        cout << "\nLoaded resource from archive:\n\n"
             << "   Name: " << resource_name << "\n"
             << "   Filesize: " << data.size() << "\n"
             << "   MD5: " << archive.GetMd5Hash(resource_name) << "\n\n"
             << endl;
    }

    auto stop_time = chrono::high_resolution_clock::now();

    cout << "Duration: " <<
        chrono::duration_cast<chrono::milliseconds>
            (stop_time - start_time).count() << "ms" << endl;

    return 0;
}
