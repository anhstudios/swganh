#include "swganh/object/cell/cell.h"

#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::cell;
using namespace swganh::messages;
using boost::optional;

void Cell::SetCell(uint32_t cell_number) {
    cell_number_ = cell_number;
    if (HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
        message.data.write<uint16_t>(1);
        message.data.write(cell_number_);

        AddDeltasUpdate(move(message));
    }
}

optional<BaselinesMessage> Cell::GetBaseline3()
{
    auto message = CreateBaselinesMessage(Object::VIEW_3, 5);
    
    // base data
    message.data.append(Object::GetBaseline3().get().data);
    message.data.write(GetCell());

    return optional<BaselinesMessage>(move(message));
}