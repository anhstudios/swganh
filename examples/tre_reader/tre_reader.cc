#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "swganh/tre/tre_reader.h"

using namespace std;
using namespace swganh::tre;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to tre file>" << endl;
        exit(0);
    }

    string resource_name(argv[1]);

    {
        TreReader tre_reader(resource_name);

        cout << "Loaded resource from archive:\n\n"
             << "   Name: " << resource_name << "\n"
             << "   File Count: " << tre_reader.GetResourceCount() << "\n" << endl;

        cout << "Finished indexing\n" << endl;
    }

    return 0;
}
