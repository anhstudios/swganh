// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "config_reader.h"

#include <array>
#include <fstream>
#include <stdexcept>

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include <boost/filesystem.hpp>

#include <anh/logger.h>

using namespace swganh::tre;

using std::ifstream;
using std::ios_base;
using std::move;
using std::invalid_argument;
using std::string;
using std::vector;

#ifdef WIN32
using std::regex;
using std::smatch;
using std::regex_match;
#else
using boost::regex;
using boost::smatch;
using boost::regex_match;
#endif

ConfigReader::ConfigReader(std::string filename)
    : config_filename_(move(filename))
{
    ParseConfig();
}

const std::vector<std::string>& ConfigReader::GetTreFilenames()
{
    return tre_filenames_;
}

void ConfigReader::ParseConfig()
{    
    ifstream input_stream(config_filename_.c_str());
    
    if (!input_stream.is_open())
    {
        LOG(fatal) << "Invalid tre configuration file: " << config_filename_;
        throw invalid_argument("Invalid tre configuration file: " + config_filename_);
    }

    boost::filesystem::path p(config_filename_);
    boost::filesystem::path dir = p.parent_path();

    regex rx("searchTree_([0-9]{2})_([0-9]{1,2})=(.*)");
    smatch match;
    string line;

    while(!input_stream.eof()) 
    {
        getline(input_stream, line);

        if (regex_search(line, match, rx))
        {
            boost::filesystem::path tmp = dir;
            tmp /= match[3].str();

            auto native_path = boost::filesystem::system_complete(tmp).native();
            tre_filenames_.push_back(string(begin(native_path), end(native_path)));
        }
    }
}
