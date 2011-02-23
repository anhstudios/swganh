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

namespace anh {
namespace component {

BaseComponent::BaseComponent(ObjectId id)
	: object_id_(id)
{
}

BaseComponent::~BaseComponent()
{
}

const ObjectId& BaseComponent::object_id(void)
{
	return object_id_;
}

void BaseComponent::HandleMessage(const Message message)
{
	event_dispatcher_.trigger(message);
}

void BaseComponent::RegisterMessageHandler(const MessageType& type, MessageHandler handler)
{
	event_dispatcher_.registerEventType(type);
    event_dispatcher_.subscribe(type, anh::event_dispatcher::EventListenerCallback(MessageType("component"), handler));
}

void BaseComponent::UnregisterMessageHandler(const MessageType& type)
{
	event_dispatcher_.unsubscribe(type);
}

}
}