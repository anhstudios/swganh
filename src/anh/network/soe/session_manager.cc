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

#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/session.h>

namespace anh {
namespace network {
namespace soe {

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

void SessionManager::Update(void)
{
	std::for_each(sessions_.begin(), sessions_.end(), [=](SessionMap::value_type& type) {
		type.second->Update();
	});
}

bool SessionManager::AddSession(std::shared_ptr<Session> session)
{
	SessionMapIterator i = sessions_.find(session->remote_endpoint());
	if(i == sessions_.end()) {
		sessions_.insert(SessionMap::value_type(session->remote_endpoint(), session));
        return true;
    }

    return false;
}

void SessionManager::RemoveSession(std::shared_ptr<Session> session)
{
	SessionMapIterator i = sessions_.find(session->remote_endpoint());
	if(i != sessions_.end())
		sessions_.erase(i);
}

std::shared_ptr<Session> SessionManager::GetSession(boost::asio::ip::udp::endpoint& endpoint)
{
	SessionMapIterator i = sessions_.find(endpoint);
	if(i == sessions_.end())
		return nullptr;
	else
		return i->second;
}

} // namespace soe
} // namespace network
} // namespace soe