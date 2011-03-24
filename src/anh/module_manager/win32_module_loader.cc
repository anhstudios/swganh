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

#include <Windows.h>
#include <anh/module_manager/win32_module_loader.h>

namespace anh {
namespace module_manager {

Win32ModuleLoader::Win32ModuleLoader(void)
{
}

Win32ModuleLoader::~Win32ModuleLoader(void)
{
}

std::shared_ptr<Module> Win32ModuleLoader::Load(const std::string& filename, void* params)
{
	HINSTANCE dll = LoadLibrary(filename.c_str());
	if(!dll)
		return nullptr;

	LoadFunction load_func = (LoadFunction)GetProcAddress(dll, "Load");
	if(!load_func(params))
		return nullptr;
}

bool Win32ModuleLoader::Unload(std::shared_ptr<Module> module)
{
	return true;
}

}
}