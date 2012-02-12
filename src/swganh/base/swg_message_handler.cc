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

#include "swganh/base/swg_message_handler.h"

#include "anh/byte_buffer.h"
#include "anh/event_dispatcher.h"
#include "anh/network/soe/packet.h"

using namespace anh;
using namespace anh::network::soe;

using namespace std;
using namespace swganh::base;

SwgMessageHandler::SwgMessageHandler(EventDispatcher* event_dispatcher) 
    : event_dispatcher_(event_dispatcher) {}

void SwgMessageHandler::operator() (shared_ptr<Packet> packet) const {        
    auto message = packet->message();

    uint16_t priority = message->peekAt<uint16_t>(message->read_position());
    uint32_t message_type = message->peekAt<uint32_t>(message->read_position() + sizeof(priority));

    event_dispatcher_->Dispatch(make_shared<ValueEvent<shared_ptr<Packet>>>(message_type, packet));
}
