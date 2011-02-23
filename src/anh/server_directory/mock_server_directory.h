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

#ifndef ANH_MOCK_SERVER_DIRECTORY_SERVER_DIRECTORY_H_
#define ANH_MOCK_SERVER_DIRECTORY_SERVER_DIRECTORY_H_

#include <gmock/gmock.h>

#include "anh/server_directory/process.h"
#include "anh/server_directory/server_directory_interface.h"

namespace anh {
namespace server_directory {

class MockServerDirectory : public ServerDirectoryInterface
{
public:
    MOCK_METHOD7(registerProcess, bool(
        const std::string& name, 
        const std::string& process_type, 
        const std::string& version, 
        const std::string& address, 
        uint16_t tcp_port, 
        uint16_t udp_port, 
        uint16_t ping));
    MOCK_METHOD1(removeProcess, bool(std::shared_ptr<Process>& process));
    MOCK_METHOD2(updateProcessStatus, void(
        std::shared_ptr<Process>& process,
        int32_t new_status));
    MOCK_METHOD1(makePrimaryProcess, bool(std::shared_ptr<Process> process));
    MOCK_METHOD0(pulse, void());
    MOCK_CONST_METHOD0(getClusterSnapshot, ClusterList());
    MOCK_CONST_METHOD1(getProcessSnapshot, ProcessList(
        std::shared_ptr<Cluster> cluster));
};

} // end server_directory
} // end anh
#endif // end ANH_MOCK_SERVER_DIRECTORY_SERVER_DIRECTORY_H_