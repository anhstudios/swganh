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

#ifndef SWGANH_LOGIN_SERVICE_INTERFACE_H_
#define SWGANH_LOGIN_SERVICE_INTERFACE_H_

#include <map>
#include <tuple>

#include "swganh/base/base_service.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace swganh {
namespace login {

class LoginServiceInterface : public swganh::base::BaseService {
public:
    explicit LoginServiceInterface(std::shared_ptr<anh::app::KernelInterface> kernel) 
        : swganh::base::BaseService(kernel) {}
    virtual ~LoginServiceInterface(){}
    
    virtual anh::service::ServiceDescription GetServiceDescription() = 0;

    virtual void DescribeConfigOptions(boost::program_options::options_description& description) = 0;
    virtual uint32_t GetAccountBySessionKey(const std::string& session_key) = 0;
};

}}  // namespace swganh::login
#endif  // SWGANH_LOGIN_SERVICE_INTERFACE_H_