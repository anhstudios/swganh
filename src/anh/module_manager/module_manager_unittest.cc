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
#include <anh/module_manager/module_manager.h>
#include <anh/module_manager/platform_services.h>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

using namespace testing;
using namespace anh::module_manager;
using namespace anh;
using namespace std;

namespace {

class ModuleManagerTests : public testing::Test
{
public:
    shared_ptr<ModuleManager> module_manager;
    shared_ptr<anh::module_manager::PlatformServices> services;
protected:
    virtual void SetUp();
    virtual void TearDown(){}
};
void ModuleManagerTests::SetUp() 
{
    module_manager = make_shared<ModuleManager>(services);
    // create test files
    ofstream of("modules.txt");
    of << "test1" << endl;
    of << "test2" << endl;
    of << "test3" << endl;
    of << "test4" << endl;
    of << "test5" << endl;
    of.flush();
    of.close();
}
TEST_F(ModuleManagerTests, LoadModuleSuccess) {
    EXPECT_TRUE(module_manager->LoadModule("testModule", 0));
}

TEST_F(ModuleManagerTests, LoadModuleNotFound) {
    EXPECT_FALSE(module_manager->LoadModule("notFound", 0));
}

TEST_F(ModuleManagerTests, LoadModuleFromPlainTextFile_Success)
{  
    module_manager->LoadModules("modules.txt");
}
TEST_F(ModuleManagerTests, LoadModuleFromPlainTextFile_NotExists)
{
    EXPECT_DEATH(module_manager->LoadModules("notfound.txt"), "Could not load file");
}
} // anon namespace