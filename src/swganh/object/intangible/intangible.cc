
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

void Intangible::SetStfDetailFile(string stf_detail_file)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    stf_detail_file_ = stf_detail_file;
	IntangibleMessageBuilder::BuildStfDetailDelta(this);
}

string Intangible::GetStfDetailString()
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return stf_detail_string_;
}

void Intangible::SetStfDetailString(string stf_detail_string)
{
	boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    stf_detail_string_ = stf_detail_string;
    
    IntangibleMessageBuilder::BuildStfDetailDelta(this);
}
boost::optional<BaselinesMessage> Intangible::GetBaseline6()
{
    return IntangibleMessageBuilder::BuildBaseline6(this);
}