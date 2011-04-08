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

#ifdef WIN32

#include <anh/module_manager/win32_module.h>

namespace anh {
namespace module_manager {

Win32Module::Win32Module(void) 
{ 
}

Win32Module::~Win32Module(void)
{
}

bool Win32Module::Load(const std::string& filename, std::shared_ptr<PlatformServices> services, std::shared_ptr<ModuleApiVersion> api_version)
{
	handle_ = LoadLibrary(filename.c_str());
	if(!handle_)
		return false;

	load_func_ = (LoadFunc)GetProcAddress(handle_, "Load");
	unload_func_ = (UnloadFunc)GetProcAddress(handle_, "Unload");
	get_name_func_ = (GetNameFunc)GetProcAddress(handle_, "GetModuleName");
	get_version_func_ = (GetVersionFunc)GetProcAddress(handle_, "GetModuleVersion");
	get_description_func_ = (GetDescriptionFunc)GetProcAddress(handle_, "GetModuleDescription");

    if(*api_version.get() != get_version_func_())
        return false;

	if(!load_func_(services))
		return false;
	else
		return true;
}

void Win32Module::Unload(std::shared_ptr<PlatformServices> services)
{
	unload_func_(services);
	FreeLibrary(handle_);
}

} // namespace module_manager
} // namespace anh

#endif