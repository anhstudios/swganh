/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include "anh/network/soe/session_request_filter.h"

#include "anh/byte_buffer.h"

#include "anh/network/soe/session.h"
#include "anh/network/soe/incoming_sessionless_packet.h"


using namespace anh::network::soe;
using namespace std;

SessionRequestFilter::SessionRequestFilter(list<shared_ptr<IncomingSessionlessPacket>>& message_queue)
    : message_queue_(message_queue) {}

void SessionRequestFilter::operator()(tbb::flow_control& fc) const {
    // No more packets to process.
    if(message_queue_.empty())
    {
        fc.stop();
        return;
    }

    auto sessionless_message = message_queue_.front();
    message_queue_.pop_front();

    if(sessionless_message->message()->peek<uint16_t>(true) == SESSION_REQUEST && sessionless_message->message()->size() == 14) {
        // @todo replace passing nullptr here with the SocketInterface instance in the service
        auto session = std::make_shared<Session>(sessionless_message->remote_endpoint(), nullptr);
        SessionRequest request(*sessionless_message->message());
        session->handleSessionRequest_(request);
    }
}
