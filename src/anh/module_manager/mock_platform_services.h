/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

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

#ifndef ANH_MOCK_MODULE_MANAGER_PLATFORM_SERVICES_H_
#define ANH_MOCK_MODULE_MANAGER_PLATFORM_SERVICES_H_

#include <gmock/gmock.h>

#include <anh/module_manager/platform_services.h>

namespace anh {
namespace module_manager {

class MockPlatformServices : public PlatformServices
{
public:
    MOCK_METHOD2( addService, bool (
        Service service_name, boost::any service));
    MOCK_METHOD1( removeService, void (
        Service service_name));
    MOCK_METHOD0( hasService, Service service_name);
};

} // end module_manager
} // end anh
#endif // end ANH_MOCK_MODULE_MANAGER_PLATFORM_SERVICES_H_