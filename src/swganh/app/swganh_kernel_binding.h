// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh_kernel.h"

#include "swganh/python_shared_ptr.h"

#include "swganh/event_dispatcher.h"
#include "swganh/app/kernel_interface.h"
#include "swganh/service/service_manager.h"
#include "swganh/combat/combat_service_interface.h"
#include "swganh/command/command_service_interface.h"
#include "swganh/social/social_service_interface.h"
#include "swganh/simulation/simulation_service_interface.h"
#include "swganh/sui/sui_service_interface.h"
#include "swganh/terrain/terrain_service_interface.h"
#include "swganh/equipment/equipment_service_interface.h"
#include "swganh/weather/weather_service_interface.h"
#include "swganh/simulation/spatial_provider_interface.h"

#include <boost/python.hpp>

using namespace swganh::app;
using namespace swganh::app;
using namespace boost::python;
using namespace std;

void exportSWGANHKernel()
{
    class_<KernelInterface, boost::noncopyable>("Kernel", no_init)
        ;
    class_<SwganhKernel, bases<KernelInterface>, boost::noncopyable>("SWGKernel", "Provides an interface to access the Service Manager and App Configuration", no_init)
        .def("app_config", &swganh::app::SwganhKernel::GetAppConfig, return_value_policy<copy_non_const_reference>(), "gets the app configuration")
        .def("service_manager", &swganh::app::SwganhKernel::GetServiceManager,return_internal_reference<>(), "Gets the application's :class:`.ServiceManager`")
        .def("event_dispatcher", &swganh::app::SwganhKernel::GetEventDispatcher, return_value_policy<reference_existing_object>(), "gets the applications :class:`.EventDispatcher`")
        ;

    class_<swganh::service::ServiceManager, boost::noncopyable>("ServiceManager", "provides an interface to common services", no_init)
       .def("combat_service", make_function(
               std::bind(&swganh::service::ServiceManager::GetService<swganh::combat::CombatServiceInterface>, std::placeholders::_1, "CombatService"),
               return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::combat::CombatServiceInterface*, swganh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.CombatServiceInterface`")
       .def("command_service", make_function(
               std::bind(&swganh::service::ServiceManager::GetService<swganh::command::CommandServiceInterface>, std::placeholders::_1, "CommandService"),
               return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::command::CommandServiceInterface*, swganh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.CommandService`")
        .def("simulation_service", make_function(
               std::bind(&swganh::service::ServiceManager::GetService<swganh::simulation::SimulationServiceInterface>, std::placeholders::_1, "SimulationService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::simulation::SimulationServiceInterface*, swganh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.SimulationService`")
        .def("social_service", make_function(
               std::bind(&swganh::service::ServiceManager::GetService<swganh::social::SocialServiceInterface>, std::placeholders::_1, "SocialService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::social::SocialServiceInterface*, swganh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.SocialService`")
		.def("sui_service", make_function(
				std::bind(&swganh::service::ServiceManager::GetService<swganh::sui::SUIServiceInterface>, std::placeholders::_1, "SuiService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::sui::SUIServiceInterface*, swganh::service::ServiceManager*>()),
				"returns an internal refrence of the :class:`.SUIService`")
		.def("terrain_service", make_function(
				std::bind(&swganh::service::ServiceManager::GetService<swganh::terrain::TerrainServiceInterface>, std::placeholders::_1, "TerrainService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::terrain::TerrainServiceInterface*, swganh::service::ServiceManager*>()),
				"returns an internal refrence of the :class:`.TerrainService`")
		.def("equipment_service", make_function(
				std::bind(&swganh::service::ServiceManager::GetService<swganh::equipment::EquipmentServiceInterface>, std::placeholders::_1, "EquipmentService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::equipment::EquipmentServiceInterface*, swganh::service::ServiceManager*>()),
				"returns an internal refrence of the :class:`.EquipmentService`")
		.def("weather_service", make_function(
				std::bind(&swganh::service::ServiceManager::GetService<swganh::weather::WeatherServiceInterface>, std::placeholders::_1, "WeatherService"),
				return_value_policy<reference_existing_object>(),
				boost::mpl::vector<swganh::weather::WeatherServiceInterface*, swganh::service::ServiceManager*>()),
				"returns an internal refrence of the :class:`.WeatherService`")
       ;
       
}
void exportEventDispatcher()
{
    class_<swganh::EventDispatcher, boost::noncopyable>("EventDispatcher", no_init)
        .def("dispatch",
            make_function(
                std::bind(&swganh::EventDispatcher::Dispatch, std::placeholders::_1, std::placeholders::_2),
                default_call_policies(),
                boost::mpl::vector<void, swganh::EventDispatcher*, std::shared_ptr<swganh::EventInterface>>()), 
            "dispatches an event to be processed later")
        .def("subscribe", &swganh::EventDispatcher::Subscribe, "subscribes to an event and attaches a callback") 
        ;
}
