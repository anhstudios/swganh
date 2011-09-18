#include "cell.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace swganh::object;
using namespace swganh::scene;
using namespace messages;

void Cell::SetCell(uint32_t cell_number) {
    cell_number_ = cell_number;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(1);
        message.data.write(cell_number_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
    }
}