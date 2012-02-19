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

#ifndef ANH_NETWORK_SOE_SESSION_MANAGER_H_
#define ANH_NETWORK_SOE_SESSION_MANAGER_H_

#include <cstdint>
#include <boost/asio.hpp>

#include <tbb/concurrent_hash_map.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DECLARATION
class Session;

class SessionManager
{
public:
    SessionManager();
    ~SessionManager();

    /**
     * @brief Updates each session in the manager.
     */
    void Update();

    bool AddSession(const std::shared_ptr<Session>& session);
    bool RemoveSession(const std::shared_ptr<Session>& session);
    bool SessionExists();
    const std::shared_ptr<Session>& GetSession(boost::asio::ip::udp::endpoint& endpoint);
private:
    typedef tbb::concurrent_hash_map<uint32_t, std::shared_ptr<Session>> SessionMap;

    SessionMap sessions_;
};

} // namespace soe
} // namespace network
} // namespace soe

#endif // ANH_NETWORK_SOE_SESSION_MANAGER_H_