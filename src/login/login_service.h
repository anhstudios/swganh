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

#ifndef LOGIN_LOGIN_SERVICE_H_
#define LOGIN_LOGIN_SERVICE_H_

#include <memory>

#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/network/soe/service.h"

namespace login {
    
class LoginService {
public:
    explicit LoginService(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface>);
    ~LoginService();

    void Run();

    bool IsRunning() const;
private:
    LoginService();

	anh::network::soe::Service soe_service_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    bool running_;
};

}  // namespace login
#endif  // LOGIN_LOGIN_SERVICE_H_