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

#ifndef ANH_MODULE_MANAGER_WIN32_MODULE_LOADER_H_
#define ANH_MODULE_MANAGER_WIN32_MODULE_LOADER_H_

#include <anh/module_manager/module_loader_interface.h>

namespace anh {
namespace module_manager {

/**
 *
 */
class Win32ModuleLoader : public ModuleLoaderInterface
{
public:
	Win32ModuleLoader(void);
	~Win32ModuleLoader(void);

	/**
	 *
	 */
	std::shared_ptr<Module> Load(const std::string& filename, void* params);

	/**
	 *
	 */
	bool Unload(std::shared_ptr<Module> module);

protected:
private:
};

}
}

#endif