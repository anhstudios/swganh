#include "cell.h"

#include "swganh/scene/scene.h"
#include "swganh/scene/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::scene;
using namespace messages;

void Cell::SetCell(uint32_t cell_number) {
    cell_number_ = cell_number;
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
        message.data.write<uint16_t>(1);
        message.data.write(cell_number_);

        AddDeltasUpdate(move(message));
    }
}

boost::optional<BaselinesMessage> Cell::GetBaseline3()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_3, 5);
    
    // base data
    message.data.append(BaseObject::GetBaseline3().get().data);
    message.data.write(GetCell());

    return boost::optional<BaselinesMessage>(std::move(message));
}