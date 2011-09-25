
#include "swganh/object/intangible/intangible.h"

#include "swganh/object/intangible/intangible_message_builder.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::intangible;
using namespace swganh::messages;

string Intangible::GetStfDetailFile() const
{
    return stf_detail_file_;
}

void Intangible::SetStfDetailFile(string stf_detail_file)
{
    stf_detail_file_ = stf_detail_file;

    IntangibleMessageBuilder::BuildStfDetailDelta(this);
}

string Intangible::GetStfDetailString() const
{
    return stf_detail_string_;
}

void Intangible::SetStfDetailString(string stf_detail_string)
{
    stf_detail_string_ = stf_detail_string;
    
    IntangibleMessageBuilder::BuildStfDetailDelta(this);
}
