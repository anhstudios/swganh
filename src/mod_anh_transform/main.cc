/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/
#include "main.h"
#include <mod_anh_transform/transform_component.h>
#include <anh/component/object_builder.h>
#include <anh/component/object_manager.h>
#include <anh/event_dispatcher/event_dispatcher_interface.h>
#include <anh/database/database_manager_interface.h>
//#include <glog/logging.h>

using namespace anh;
using namespace api::components;
using namespace transform;
using namespace std;
shared_ptr<event_dispatcher::EventDispatcherInterface> gEventDispatcher;
shared_ptr<component::ObjectManager> gObjManager;
// temp
uint64_t guid = 0;
bool DLL_EXPORT Load(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	// register component to object manager
    gObjManager = 
        boost::any_cast<shared_ptr<component::ObjectManager>>(services->getService("ObjectManager"));
    if (gObjManager != nullptr)
    {
        // get new GUID
        shared_ptr<TransformComponentInterface> component( new TransformComponent(1) );
        gObjManager->AttachComponent(guid+1, component);
    }
    // subscribe to events
    gEventDispatcher =
        boost::any_cast<shared_ptr<event_dispatcher::EventDispatcherInterface>>(services->getService("EventDispatcher"));
    if (gEventDispatcher != nullptr)
    {
     

    }
    // init specific logs

    // init db hooks
    
    // trigger 'loaded event'
    
    //gEventDispatcher->trigger(event_dispatcher::BasicEvent("TransformLoaded"));

    return true;
}

bool DLL_EXPORT Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	// unregister components
    //gObjManager->DetachComponent(1, ComponentType("TransformComponent"));

    // unsubscribes

    // triggers 'unload event'
    return true;
}

const std::string DLL_EXPORT GetModuleName(void) {
	return "ANH.Transform";
}

const anh::module_manager::ModuleApiVersion DLL_EXPORT GetModuleVersion(void) {
	return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string DLL_EXPORT GetModuleDescription(void) {
	return "ANH's Transform Module";
}