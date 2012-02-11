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

#ifndef SWGANH_BASE_SWG_MESSAGE_HANDLER_H_
#define SWGANH_BASE_SWG_MESSAGE_HANDLER_H_

#include <memory>

namespace anh {
class ByteBuffer;
}  // namespace anh

namespace anh {
class EventDispatcher;
}  // namesapce anh

namespace anh {
namespace network {
namespace soe {
class Packet;
}}}  // namespace anh::network::soe


namespace swganh {
namespace base {
      
class SwgMessageHandler {
public:
    explicit SwgMessageHandler(anh::EventDispatcher* event_dispatcher);

    void operator() (std::shared_ptr<anh::network::soe::Packet> packet) const;

private:
    SwgMessageHandler();

    anh::EventDispatcher* event_dispatcher_;
};
        
}}  // namespace swganh::base

#endif  // SWGANH_BASE_SWG_MESSAGE_HANDLER_H_
