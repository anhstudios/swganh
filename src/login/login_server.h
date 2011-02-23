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

#ifndef LOGIN_LOGIN_SERVER_H_
#define LOGIN_LOGIN_SERVER_H_

#include <list>

#include "anh/memory.h"

#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/server_directory/server_directory_interface.h"

namespace anh {
namespace event_dispatcher {
    class EventDispatcherInterface;
}
}

namespace login {

class LoginServer {
public:
    LoginServer();
    ~LoginServer();

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher();
    void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
    
    std::shared_ptr<anh::server_directory::ServerDirectoryInterface> server_directory();
    void server_directory(std::shared_ptr<anh::server_directory::ServerDirectoryInterface> server_directory);

    void startup();

    void updateClusterList();
    
    std::list<std::string> getAvailableVersions() const;
    std::list<std::string> getAvailableVersions(const anh::server_directory::ClusterList& cluster_list) const;

private:
    bool handleLoginClientId(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    bool handleDeleteCharacterMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<anh::server_directory::ServerDirectoryInterface> server_directory_;

    anh::server_directory::ClusterList cluster_list_;
    std::list<std::string> version_list_;
};

}

#endif  // LOGIN_LOGIN_SERVER_H_
