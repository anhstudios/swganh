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

#ifndef ANH_SERVICE_MOCK_SERVICE_DIRECTORY_H_
#define ANH_SERVICE_MOCK_SERVICE_DIRECTORY_H_

#include <gmock/gmock.h>

#include "anh/service_directory/service.h"
#include "anh/service_directory/service_directory_interface.h"

namespace anh {
namespace service {

class MockServiceDirectory : public ServiceDirectoryInterface
{
public:
    MOCK_METHOD3(joinGalaxy, void (const std::string& galaxy_name, const std::string& version, bool create_galaxy));
    MOCK_METHOD7(registerService, bool(
        const std::string& name, 
        const std::string& service_type, 
        const std::string& version, 
        const std::string& address, 
        uint16_t tcp_port, 
        uint16_t udp_port, 
        uint16_t ping));
    MOCK_METHOD1(removeService, bool(const ServiceDescription& service));
    MOCK_METHOD1(updateServiceStatus, void(int32_t new_status));
    MOCK_METHOD1(makePrimaryService, bool(const ServiceDescription& service));
    MOCK_METHOD0(pulse, void());
    MOCK_METHOD0(updateGalaxyStatus, void());
    MOCK_CONST_METHOD0(galaxy, Galaxy());
    MOCK_CONST_METHOD0(service, ServiceDescription());
    MOCK_METHOD0(getGalaxySnapshot, GalaxyList());
    MOCK_METHOD1(getServiceSnapshot, ServiceList(
       const Galaxy& galaxy));
};

} // end service_directory
} // end anh
#endif // end ANH_SERVICE_MOCK_SERVICE_DIRECTORY_H_