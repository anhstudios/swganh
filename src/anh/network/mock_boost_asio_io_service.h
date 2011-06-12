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

#include <gmock/gmock.h>
#include <boost/asio/io_service.hpp>

namespace boost {
namespace asio {
class MockAsioIOService : public io_service
{
public:
    MockAsioIOService() {}
    MOCK_METHOD0(run, std::size_t());
    MOCK_METHOD1(run, std::size_t(boost::system::error_code&));
    MOCK_METHOD0(run_one, std::size_t());
    MOCK_METHOD1(run_one, std::size_t(boost::system::error_code&));
    MOCK_METHOD0(poll, std::size_t());
    MOCK_METHOD1(poll, std::size_t(boost::system::error_code&));
    MOCK_METHOD0(poll_one, std::size_t());
    MOCK_METHOD1(poll_one, std::size_t(boost::system::error_code&));
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(reset, void());
};
}
}