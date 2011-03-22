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

#ifndef ANH_MODULE_MANAGER_MOCK_MODULE_LOADER_H_
#define ANH_MODULE_MANAGER_MOCK_MODULE_LOADER_H_

#include <gmock/gmock.h>

#include "anh/module_manager/module_manager.h"

namespace anh {
namespace module_manager{

class MockModuleLoader : public ModuleLoader {
public:
    MOCK_METHOD1(LoadModule, std::shared_ptr<Module>(anh::HashString name));
    MOCK_METHOD1(FreeModule, void(anh::HashString name));
};

}  // namespace event_dispatcher
}  // namespace anh

#endif  // ANH_MODULE_MANAGER_MOCK_MODULE_LOADER_H_
