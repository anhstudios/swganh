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

#ifndef SWGANH_LOGIN_BASE_LOGIN_SERVICE_H_
#define SWGANH_LOGIN_BASE_LOGIN_SERVICE_H_

#include <map>
#include <tuple>

#include "swganh/base/base_service.h"
#include "swganh/base/swg_message_router.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace swganh {
namespace login {

struct LoginClient;

class BaseLoginService 
    : public swganh::base::BaseService 
    , public swganh::base::SwgMessageRouter<LoginClient> {
public:
    explicit BaseLoginService(std::shared_ptr<anh::app::KernelInterface> kernel);
    
    virtual anh::service::ServiceDescription GetServiceDescription() = 0;

    virtual uint32_t GetAccountBySessionKey(const std::string& session_key) = 0;
    
    virtual std::shared_ptr<LoginClient> GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint) = 0;
};

}}  // namespace swganh::login
#endif  // SWGANH_LOGIN_BASE_LOGIN_SERVICE_H_