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

#include "character/main.h"

#include <iostream>

#include <glog/logging.h>
#include <boost/thread.hpp>

#include "character/character_service.h"

using namespace anh;
using namespace swganh::character;
using namespace character;
using namespace module_manager;
using namespace std;
using boost::any_cast;

static std::shared_ptr<CharacterService> character_service;

bool API Load(shared_ptr<PlatformServices> services) {
    cout << GetModuleName() << " Loading..." << endl;

    character_service = make_shared<CharacterService>();

    services->addService("CharacterService", static_pointer_cast<CharacterServiceInterface>(character_service));

    cout << GetModuleName() << " Loaded!" << endl;

    return true;
}

void API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
    services->removeService("CharacterService");
}

void API Start(std::shared_ptr<anh::module_manager::PlatformServices> services) {    
    boost::thread t([] () { character_service->Start(); });
}

void API Stop(std::shared_ptr<anh::module_manager::PlatformServices> services) {  
    cout << GetModuleName() << " Stopping..." << endl;

    character_service->Stop();

    while (character_service->IsRunning()) { }
    
    cout << GetModuleName() << " Stopped!" << endl;
}

const std::string API GetModuleName(void) {
    return "ANH Character Service";
}

const std::string API GetModuleType(void) {
    return "CharacterService";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
    return anh::module_manager::ModuleApiVersion(1, 0, "1.0");
}

const std::string API GetModuleDescription(void) {
    return "Provides character management services to the cluster.";
}
