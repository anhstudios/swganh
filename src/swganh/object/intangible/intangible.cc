// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::intangible;
using namespace swganh::messages;

string Intangible::GetStfDetailFile()
{
    boost::lock_guard<boost::mutex> lock(intangible_mutex_);
    return stf_detail_file_;
}

void Intangible::SetStfDetail(const string& stf_file_name, const string& stf_string)
{
    {
        boost::lock_guard<boost::mutex> lock(intangible_mutex_);
        stf_detail_file_ = stf_file_name;
        stf_detail_string_ = stf_string;
    }
}

string Intangible::GetStfDetailString()
{
    boost::lock_guard<boost::mutex> lock(intangible_mutex_);
    return stf_detail_string_;
}

void Intangible::GetBaseline6()
{
    //return IntangibleMessageBuilder::BuildBaseline6(this);
}