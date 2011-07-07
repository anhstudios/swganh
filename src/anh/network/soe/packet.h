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

#ifndef ANH_NETWORK_SOE_PACKET_H_
#define ANH_NETWORK_SOE_PACKET_H_

#include <memory>

// FORWARD DECLARATIONS
namespace anh { class ByteBuffer; }

namespace anh {
namespace network {
namespace soe {

class Session;

class Packet {
public:
    Packet(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message) 
        : session_(session)
        , message_(message) {}

    std::shared_ptr<Session> session() { return session_; }
    std::shared_ptr<anh::ByteBuffer> message() { return message_; }

private:
    Packet();

    std::shared_ptr<Session> session_;
    std::shared_ptr<anh::ByteBuffer> message_;
};

}}}  // namespace anh::network::soe

#endif // ANH_NETWORK_SOE_INCOMING_PACKET_H_
