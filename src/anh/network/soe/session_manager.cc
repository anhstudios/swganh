/*
 This file is part of SWGANH. For more information, visit http://swganh.com

 Copyright (c) 2006 - 2012 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "session_manager.h"

#include "packet_utilities.h"
#include "session.h"

using namespace anh::network::soe;

SessionManager::SessionManager() {}

SessionManager::~SessionManager() {}

void SessionManager::Update(void) {
    std::for_each(sessions_.begin(), sessions_.end(), [=](SessionMap::value_type& type) {
        type.second->Update();
    });
}

bool SessionManager::AddSession(const std::shared_ptr<Session>& session) {
    SessionMap::accessor a;

    auto address_hash = CreateEndpointHash(session->remote_endpoint());

    if(!sessions_.find(a, address_hash)) {
        sessions_.insert(a, address_hash);
        a->second = session;
        return true;
    }

    return false;
}

bool SessionManager::RemoveSession(const std::shared_ptr<Session>& session) {
    SessionMap::accessor a;
    if (sessions_.find(a, CreateEndpointHash(session->remote_endpoint()))) {
        sessions_.erase(a);
        return true;
    }

    return false;
}

std::shared_ptr<Session> SessionManager::GetSession(boost::asio::ip::udp::endpoint& endpoint) {
    SessionMap::accessor a;
    if (sessions_.find(a, CreateEndpointHash(endpoint))) {
        return a->second;
    }

    return nullptr;
}
