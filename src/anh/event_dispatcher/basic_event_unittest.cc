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
 
#include <string>
#include <gtest/gtest.h>
 
#include "anh/event_dispatcher/basic_event.h"
 
using namespace std;
using namespace anh::event_dispatcher;

struct TestEventData {
    string test_string;
    int test_int;
};

TEST(BasicEventTest, CanMakeEvent) {
    TestEventData test_data;
    test_data.test_string = "test string";
    test_data.test_int = 12345;

    auto my_event = make_event("SomeEvent", std::move(test_data));
    
    EXPECT_EQ("test string", my_event.data().test_string);
    EXPECT_EQ(12345, my_event.data().test_int);
}

TEST(BasicEventTest, CanMakeSharedEvent) {
    TestEventData test_data;
    test_data.test_string = "test string";
    test_data.test_int = 12345;

    auto my_event = make_shared_event("SomeEvent", std::move(test_data));

    EXPECT_EQ("test string", my_event->data().test_string);
    EXPECT_EQ(12345, my_event->data().test_int);
}

TEST(BasicEventTest, CanMakeSimpleEvent) {
    auto my_event = make_event("SomeEvent");

    EXPECT_EQ("SomeEvent", my_event.type().ident_string());
}

TEST(BasicEventTest, CanMakeSimpleSharedEvent) {
    auto my_event = make_shared_event("SomeEvent");

    EXPECT_EQ("SomeEvent", my_event->type().ident_string());
}
