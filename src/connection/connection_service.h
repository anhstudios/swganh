/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

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

#ifndef CONNECTION_CONNECTION_SERVICE_H_
#define CONNECTION_CONNECTION_SERVICE_H_

#include <boost/program_options/options_description.hpp>

#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/network/soe/server.h"

namespace connection {
    
class ConnectionService {
public:
    explicit ConnectionService(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
    ~ConnectionService();
    
    void DescribeConfigOptions(boost::program_options::options_description& description);

    void Start();
    void Stop();

    bool IsRunning() const;
    
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher();
    void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);

private:
    bool HandleCmdSceneReady_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    bool HandleClientIdMsg_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    bool HandleSelectCharacter_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;

    tbb::atomic<bool> running_;

    uint16_t listen_port_;
};

}  // namespace connection
#endif  // CONNECTION_CONNECTION_SERVICE_H_