
#include "swganh/object/intangible.h"

#include "swganh/scene/scene.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::scene;
using namespace swganh::scene::messages;

string Intangible::GetStfDetailFile() const
{
    return stf_detail_file_;
}

void Intangible::SetStfDetailFile(std::string stf_detail_file)
{
    stf_detail_file_ = stf_detail_file;

    // Only build a message if there are observers.
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(1); // update type
        message.data.write(stf_detail_file_);
        message.data.write<uint32_t>(0);
        message.data.write(stf_detail_string_);

        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));            
    }
}

std::string Intangible::GetStfDetailString() const
{
    return stf_detail_string_;
}

void Intangible::SetStfDetailString(std::string stf_detail_string)
{
    stf_detail_string_ = stf_detail_string;

    // Only build a message if there are observers.
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(1); // update type
        message.data.write(stf_detail_file_);
        message.data.write<uint32_t>(0);
        message.data.write(stf_detail_string_);

        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));            
    }
}
