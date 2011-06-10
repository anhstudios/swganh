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

#include <gtest/gtest.h>

#include "anh/event_dispatcher/basic_event.h"
#include "packets/Login/LoginClientId.h"

using anh::event_dispatcher::make_shared_event;
using packets::LoginClientId;

namespace {

TEST(PacketTest, BaseSwgPacketsAreSerializable) {
    LoginClientId login;
    login.username = "testuser";
    login.password = "testpass";
    login.client_version = "012345";

    auto login_event = make_shared_event(LoginClientId::opcode, login);

    EXPECT_EQ("testuser", login_event->username);
    EXPECT_EQ("testpass", login_event->password);
    EXPECT_EQ("012345", login_event->client_version);
         
    anh::ByteBuffer buffer;
    login_event->serialize(buffer);
    
    EXPECT_EQ(0x0004, buffer.read<uint16_t>());
    EXPECT_EQ(0x41131F96, buffer.read<uint32_t>());
    EXPECT_EQ("testuser", buffer.read<std::string>());
    EXPECT_EQ("testpass", buffer.read<std::string>());
    EXPECT_EQ("012345", buffer.read<std::string>());

}

}  // namespace
