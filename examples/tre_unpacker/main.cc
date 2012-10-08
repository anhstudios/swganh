// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <cmath>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include "anh/utilities.h"
#include "swganh/tre/tre_archive.h"

namespace bfs = boost::filesystem;
using boost::this_thread::sleep;
using Concurrency::make_task;
using Concurrency::task;
using swganh::tre::TreArchive;

std::tuple<std::string, std::string> ProcessInput(int argc, char *argv[]);

void ValidateSwgLiveConfig(const std::string& path_to_config);
void ValidateTargetOutputDirectory(const std::string& target_output_directory);

void SaveFile(std::string filename, const std::vector<unsigned char>& filedata);

void UpdateProgressBar(double total, double completed);

std::chrono::high_resolution_clock::time_point StartTimer();
void StopTimer(std::chrono::high_resolution_clock::time_point& start_time);

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    auto start_time = StartTimer();
#endif

    std::cout << "Tre Unpacker v1.0\n\n" << std::endl;
    
    try 
    {
        std::string swg_live_file;
        std::string output_path;

        std::tie(swg_live_file, output_path) = ProcessInput(argc, argv);

        TreArchive archive(swg_live_file);

        std::cout << "\nGetting the list of available resources\r" << std::endl;
        auto resources = archive.GetAvailableResources();        
        auto total = resources.size();
        std::cout << "Found (" << total << ") resources\n" << std::endl;
        
        int i = 0;
        for (auto& resource_name : resources)
        {
            auto target_filename = output_path + "/" + resource_name;
            auto resource = archive.GetResource(resource_name);

            SaveFile(target_filename, archive.GetResource(resource_name)); 

            if (i % 100)
            {
                UpdateProgressBar(total, i);
            }

            ++i;
        }
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "\nComplete!\n" << std::endl;

#ifdef _DEBUG
    StopTimer(start_time);
#endif

    std::cout << "Press any key to exit..." << std::endl;
    while (anh::KeyboardHit() == 0) sleep(boost::posix_time::milliseconds(1));

    return 0;
}


std::tuple<std::string, std::string> ProcessInput(int argc, char *argv[])
{
    std::string swg_live_file;
    std::string output_path;

    if (argc == 1)
    {
        std::cout << "Enter path to the swg live.cfg" << std::endl;

        std::cin >> swg_live_file;
        ValidateSwgLiveConfig(swg_live_file);

        std::cout << "\nEnter the target output directory" << std::endl;

        std::cin >> output_path;
        ValidateTargetOutputDirectory(output_path);
    }
    else if (argc == 3)
    {
        swg_live_file = std::string(argv[1]);
        ValidateSwgLiveConfig(swg_live_file);

        output_path = std::string(argv[2]);
        ValidateTargetOutputDirectory(output_path);
    }
    else
    {        
        throw std::runtime_error("Invalid number of parameters specified");
    }

    return std::make_tuple(swg_live_file, output_path);
}

void ValidateSwgLiveConfig(const std::string& path_to_config)
{
    if (!bfs::is_regular_file(path_to_config))
    {
        throw std::runtime_error("Invalid path specified: " + path_to_config);
    }
}

void ValidateTargetOutputDirectory(const std::string& target_output_directory)
{
    if (bfs::exists(target_output_directory) && !bfs::is_empty(target_output_directory))
    {
        char should_empty;
        do
        {
            std::cout << "The target output directory is not empty, clear its contents now? [y/n]" << std::endl;
            std::cin >> should_empty;
        } while (!std::cin.fail() && should_empty != 'y' && should_empty != 'n');

        if (should_empty == 'y')
        {
            bfs::remove_all(target_output_directory);
            bfs::create_directory(target_output_directory);
        }
        else
        {
            throw std::runtime_error("Target output directory is not empty");
        }
    }
}

void SaveFile(std::string filename, const std::vector<unsigned char>& filedata)
{
    auto path = bfs::path(filename);

    if (bfs::is_regular_file(path))
    {
        bfs::remove(path);
    }

    if (!bfs::is_directory(path.parent_path()))
    {
        bfs::create_directories(path.parent_path());
    }
    
    std::basic_ofstream<unsigned char> ofs(filename, std::ios::out | std::ios::binary);

    if (filedata.size() > 0)
    {
        ofs.write(&filedata[0], filedata.size());
    }

    ofs.close();
}

void UpdateProgressBar(double total, double completed)
{
    // how wide you want the progress meter to be
    int totaldots=50;
    double fraction_complete = completed / total;
    // part of the progressmeter that's already "full"
    int dots = static_cast<int>(std::ceil(fraction_complete * totaldots));

    // create the "meter"
    int i=0;
    printf("%3.0f%% [", fraction_complete * 100);
    // part  that's full already
    for ( ; i < dots; i++) {
        printf("=");
    }
    // remaining part (spaces)
    for ( ; i < totaldots; i++) {
        printf(" ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf("]\r");
    fflush(stdout);
}

std::chrono::high_resolution_clock::time_point StartTimer()
{
    return std::chrono::high_resolution_clock::now();
}

void StopTimer(std::chrono::high_resolution_clock::time_point& start_time)
{
    auto stop_time = std::chrono::high_resolution_clock::now();

    std::cout << "Duration: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() 
        << "ms" << std::endl;
}
