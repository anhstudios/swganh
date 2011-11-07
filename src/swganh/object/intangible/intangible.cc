
#include "swganh/object/intangible/intangible.h"

#include "swganh/object/intangible/intangible_message_builder.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::intangible;
using namespace swganh::messages;

string Intangible::GetStfDetailFile()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return stf_detail_file_;
}

void Intangible::SetStfDetail(const string& stf_file_name, const string& stf_string)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    stf_detail_file_ = stf_file_name;
    stf_detail_string_ = stf_string;
	IntangibleMessageBuilder::BuildStfDetailDelta(this);
}

string Intangible::GetStfDetailString()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return stf_detail_string_;
}

boost::optional<BaselinesMessage> Intangible::GetBaseline6()
{
    return IntangibleMessageBuilder::BuildBaseline6(this);
}