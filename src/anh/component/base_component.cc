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

#include <anh/component/base_component.h>
#include <glog/logging.h>

namespace anh {
namespace component {

BaseComponent::BaseComponent(const ComponentType& interface, const ComponentType& type)
    : entity_id_(0)
    , type_(type)
    , interface_(interface)
    , dirty_(false)
{
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::HandleMessage(const Message message)
{
    event_dispatcher_.trigger(message);
}

bool BaseComponent::RegisterMessageHandler(const MessageType& type, MessageHandler handler)
{
    try {
        event_dispatcher_.subscribe(type, handler);
    }
    catch(anh::event_dispatcher::InvalidEventType) {
        LOG(INFO) << "invalid event type for component: " << this->component_type();
        return false;
    }
    return true;
}

void BaseComponent::UnregisterMessageHandler(const MessageType& type)
{
    event_dispatcher_.unsubscribe(type);
}

}
}