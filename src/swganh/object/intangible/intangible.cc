
#include "swganh/object/intangible/intangible.h"

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

    // Only build a message if there are observers.
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(1); // update type
        message.data.write(stf_detail_file_);
        message.data.write<uint32_t>(0);
        message.data.write(stf_detail_string_);

        AddDeltasUpdate(message);          
    }
}

string Intangible::GetStfDetailString() const
{
    return stf_detail_string_;
}

void Intangible::SetStfDetailString(string stf_detail_string)
{
    stf_detail_string_ = stf_detail_string;

    // Only build a message if there are observers.
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(1); // update type
        message.data.write(stf_detail_file_);
        message.data.write<uint32_t>(0);
        message.data.write(stf_detail_string_);

        AddDeltasUpdate(message);                
    }
}
