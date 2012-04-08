
#include "intangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::intangible;
using namespace swganh::messages;

string Intangible::GetStfDetailFile()
{
	std::lock_guard<std::mutex> lock(intangible_mutex_);
    return stf_detail_file_;
}

void Intangible::SetStfDetail(const string& stf_file_name, const string& stf_string)
{
    {
	    std::lock_guard<std::mutex> lock(intangible_mutex_);
        stf_detail_file_ = stf_file_name;
        stf_detail_string_ = stf_string;
    }
}

string Intangible::GetStfDetailString()
{
	std::lock_guard<std::mutex> lock(intangible_mutex_);
    return stf_detail_string_;
}

void Intangible::GetBaseline6()
{
    //return IntangibleMessageBuilder::BuildBaseline6(this);
}