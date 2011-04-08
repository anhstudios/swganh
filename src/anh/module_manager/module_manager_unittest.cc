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
#include <gmock/gmock.h>
#include <iostream>
#include <fstream>

using namespace testing;
using namespace anh::module_manager;
using namespace anh;
using namespace std;

namespace {

class MockModuleManager : public ModuleManager
{
public:
    MockModuleManager(std::shared_ptr<PlatformServices> services) : ModuleManager(services) {}
    MOCK_METHOD0(CreateModule, std::shared_ptr<ModuleInterface>() );
    MOCK_METHOD2(LoadModule, bool( std::string name, std::shared_ptr<ModuleInterface> module));
    MOCK_METHOD1(LoadModule, bool( anh::HashString module_name ));
    MOCK_METHOD1(LoadModules, void ( ModulesMap modules ));
    MOCK_METHOD1(LoadModules, void ( ModulesVec modules_vec ));
    MOCK_METHOD1(LoadModules, void ( const std::string& file_name ));
    MOCK_METHOD1(UnloadModule, void ( anh::HashString module_name ));
    MOCK_METHOD1(isLoaded, bool ( anh::HashString module_name ));
};

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
    auto mod_manager = make_shared<MockModuleManager>(services);
    EXPECT_CALL(*mod_manager, LoadModule(_));
    mod_manager->LoadModule("testModule");
}

TEST_F(ModuleManagerTests, LoadModuleNotFound) {
    auto mod_manager = make_shared<MockModuleManager>(services);
    EXPECT_CALL(*mod_manager, LoadModule(_));
    EXPECT_FALSE(mod_manager->LoadModule("notFound"));
}

TEST_F(ModuleManagerTests, LoadModuleFromPlainTextFile_Success)
{  
    module_manager->LoadModules("modules.txt");
}
TEST_F(ModuleManagerTests, LoadModuleFromPlainTextFile_NotExists)
{
    EXPECT_ANY_THROW(
        module_manager->LoadModules("notfound.txt")
        );
}
} // anon namespace