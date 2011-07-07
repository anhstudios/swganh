/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef MOD_ANH_CHARACTER_MAIN_H_
#define MOD_ANH_CHARACTER_MAIN_H_

#include <string>

#include "anh/module_manager/module_interface.h"
#include "anh/module_manager/platform_services.h"
#include "anh/module_manager/module_main.h"

#ifdef __cplusplus
extern "C" {
#endif

bool API Load(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Unload(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Start(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Stop(std::shared_ptr<anh::module_manager::PlatformServices>);
const std::string API GetModuleName(void);
const std::string API GetModuleType(void);
const anh::module_manager::ModuleApiVersion API GetModuleVersion(void);
const std::string API GetModuleDescription(void);

#ifdef __cplusplus
}
#endif

#endif  // MOD_ANH_CHARACTER_MAIN_H_
