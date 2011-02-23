/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ANH_EVENT_DISPATCHER_MOCK_EVENT_DISPATCHER_H_
#define ANH_EVENT_DISPATCHER_MOCK_EVENT_DISPATCHER_H_

#include <gmock/gmock.h>

#include "anh/event_dispatcher/event_dispatcher_interface.h"

namespace anh {
namespace event_dispatcher{

class MockEventDispatcher : public EventDispatcherInterface {
public:
    MOCK_CONST_METHOD1(hasListeners, bool(const EventType& event_type));
    MOCK_CONST_METHOD1(hasRegisteredEventType, bool(const EventType& event_type));
    MOCK_CONST_METHOD0(hasEvents, bool());
    MOCK_METHOD1(registerEventType, bool(EventType event_type));
    MOCK_CONST_METHOD0(registered_event_types, EventTypeSet());
    MOCK_METHOD2(subscribe, uint64_t(const EventType& event_type, EventListenerCallback listener));
    MOCK_METHOD1(unsubscribe, void(const EventType& event_type));
    MOCK_METHOD2(unsubscribe, void(const EventType& event_type, uint64_t listener_id));
    MOCK_METHOD1(trigger, bool(std::shared_ptr<EventInterface> incoming_event));
    MOCK_METHOD2(trigger, bool(std::shared_ptr<EventInterface> incoming_event, PostTriggerCallback callback));
    MOCK_METHOD2(triggerWhen, void(std::shared_ptr<EventInterface> incoming_event, TriggerCondition condition));
    MOCK_METHOD3(triggerWhen, void(std::shared_ptr<EventInterface> incoming_event, TriggerCondition condition, PostTriggerCallback callback));
    MOCK_METHOD1(triggerAsync, bool(std::shared_ptr<EventInterface> incoming_event));
    MOCK_METHOD2(triggerAsync, bool(std::shared_ptr<EventInterface> incoming_event, PostTriggerCallback callback));
    MOCK_METHOD2(abort, bool(const EventType& event_type, bool all_of_type));
    MOCK_METHOD1(tick, bool(uint64_t timeout_ms));
};

}  // namespace event_dispatcher
}  // namespace anh

#endif  // ANH_EVENT_DISPATCHER_MOCK_EVENT_DISPATCHER_H_
