/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include <gtest/gtest.h>
#include <anh/module_manager/win32_module.h>
#include <anh/module_manager/platform_services.h>

#ifdef WIN32

using namespace anh::module_manager;

TEST(Win32ModuleTest, Load_Success)
{
    auto module_version = std::make_shared<ModuleApiVersion>(0, 1, "0.1");
	Win32Module module;
	std::shared_ptr<PlatformServices> services = std::make_shared<PlatformServices>();
	EXPECT_TRUE(module.Load("./test_module.dll", services, module_version));
}

TEST(Win32ModuleTest, Unload_Success)
{
    auto module_version = std::make_shared<ModuleApiVersion>(0, 1, "0.1");
	Win32Module module;
	std::shared_ptr<PlatformServices> services = std::make_shared<PlatformServices>();
	module.Load("./test_module.dll", services, module_version);
	module.Unload(services);
}

TEST(Win32ModuleTest, GetName_Success)
{
    auto module_version = std::make_shared<ModuleApiVersion>(0, 1, "0.1");
	Win32Module module;
	std::shared_ptr<PlatformServices> services = std::make_shared<PlatformServices>();
	module.Load("./test_module.dll", services, module_version);

	EXPECT_EQ(std::string("DLLTest"), module.name());
}

TEST(Win32ModuleTest, GetVersion_Success)
{
    auto module_version = std::make_shared<ModuleApiVersion>(0, 1, "0.1");
	Win32Module module;
	std::shared_ptr<PlatformServices> services = std::make_shared<PlatformServices>();
	module.Load("./test_module.dll", services, module_version);

	EXPECT_EQ(module_version->major, module.version().major);
	EXPECT_EQ(module_version->minor, module.version().minor);
}

TEST(Win32ModuleTest, GetDescription_Success)
{
    auto module_version = std::make_shared<ModuleApiVersion>(0, 1, "0.1");
	Win32Module module;
	std::shared_ptr<PlatformServices> services = std::make_shared<PlatformServices>();
	module.Load("./test_module.dll", services, module_version);

	std::string module_description = "A test module.";
	EXPECT_EQ(module_description, module.description());
}

#endif