/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2011 ANH Studios

 SWGANH is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SWGANH is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SWGANH.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include <anh/module_manager/mock_module_loader.h>

using namespace anh::module_manager;
using namespace std;

namespace {

class ModuleManagerTests : public testing::Test
{
public:
    shared_ptr<ModuleManager> module_manager;
    MockModuleLoader loader;
protected:
    virtual void SetUp();
    virtual void TearDown(){}
};
void ModuleManagerTests::SetUp() 
{
    module_manager = make_shared<ModuleManager>(loader);
}
TEST_F(ModuleManagerTests, LoadModuleSuccess) {
    EXPECT_TRUE(module_manager->LoadModule("testModule"));
    EXPECT_CALL(loader, LoadModule());
}

TEST_F(ModuleManagerTests, LoadModuleNotFound) {
    //EXPECT_FALSE(module_manager->LoadModule("notFound"));
}

TEST_F(ModuleManagerTests, LoadModuleFromList) {
    
}
}