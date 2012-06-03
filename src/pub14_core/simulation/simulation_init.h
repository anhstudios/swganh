// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CORE_SIMULATION_INITIALIZATION_H_
#define SWGANH_CORE_SIMULATION_INITIALIZATION_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "quadtree_spatial_provider.h"
#include "scene.h"
#include "scene_manager.h"
#include "simulation_service.h"
#include "movement_manager.h"

#include "version.h"

namespace swganh_core {
namespace simulation {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;

	SimulationService* simulation_service = nullptr;
	
	// Register Simulation Service
	{
		// Register
		registration.CreateObject = [kernel, simulation_service] (anh::plugin::ObjectParams* params) -> void* {
			return new SimulationService(kernel);
		};
		
		registration.DestroyObject = [] (void * object) {
			if (object) {
				delete static_cast<SimulationService*>(object);
			}
		};
		kernel->GetPluginManager()->RegisterObject("Simulation::SimulationService", &registration);
	}
	// Register Movement Manager
	{
		registration.CreateObject = [kernel, simulation_service] (anh::plugin::ObjectParams* params) -> void* {
			return new MovementManager(kernel);
		};
		registration.DestroyObject = [] (void  * object) {
			if (object) {
				delete static_cast<MovementManager*>(object);
			}
		};
		kernel->GetPluginManager()->RegisterObject("Simulation::MovementManager", &registration);
	}

	// Register Scene Manager
	{
		registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void* {
			return new SceneManager();
		};
		registration.DestroyObject = [] (void * object) {
			if (object) {
				delete static_cast<SceneManager*>(object);
			}
		};
		kernel->GetPluginManager()->RegisterObject("Simulation::SceneManager", &registration);
	}
	// Register Quadtree
	{
		// Register
		registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
			return new QuadtreeSpatialProvider(kernel);
		};

		registration.DestroyObject = [] (void * object) {
			if (object) {
				delete static_cast<QuadtreeSpatialProvider*>(object);
			}
		};

		kernel->GetPluginManager()->RegisterObject("Simulation::SpatialProvider", &registration);  
	}
}

}}  // namespace swganh_core::galaxy

#endif  // SWGANH_CORE_SIMULATION_INITIALIZATION_H_
