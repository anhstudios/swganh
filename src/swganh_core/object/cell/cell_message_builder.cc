// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell_message_builder.h"

#include <algorithm>

#include "cell.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void CellMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register Handlers for Cells
}

boost::optional<BaselinesMessage> CellMessageBuilder::BuildBaseline3(const shared_ptr<Cell>& cell, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(cell, lock, Object::VIEW_3, 5);
    message.data.append((*IntangibleMessageBuilder::BuildBaseline3(cell, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> CellMessageBuilder::BuildBaseline6(const shared_ptr<Cell>& cell, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(cell, lock, Object::VIEW_6, 5);
    message.data.append((*IntangibleMessageBuilder::BuildBaseline6(cell, lock)).data);
    return BaselinesMessage(std::move(message));
}