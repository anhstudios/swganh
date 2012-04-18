#ifndef SWGANH_APP_SWGANH_KERNEL_BINDING_H_
#define SWGANH_APP_SWGANH_KERNEL_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh_kernel.h"

#include "anh/python_shared_ptr.h"

#include "anh/event_dispatcher.h"
#include "anh/app/kernel_interface.h"
#include "anh/service/service_manager.h"
#include "swganh/combat/combat_service.h"
#include "swganh/social/social_service.h"
#include "swganh/simulation/simulation_service.h"

#include <boost/python.hpp>

using namespace anh::app;
using namespace swganh::app;
using namespace boost::python;
using namespace std;

void exportSWGANHKernel()
{
    class_<KernelInterface, boost::noncopyable>("Kernel", no_init)
        ;
    class_<SwganhKernel, bases<KernelInterface>, boost::noncopyable>("SWGKernel", "Provides an interface to access the Service Manager and App Configuration")
        .def("app_config", &swganh::app::SwganhKernel::GetAppConfig, return_value_policy<copy_non_const_reference>(), "gets the app configuration")
        .def("service_manager", &swganh::app::SwganhKernel::GetServiceManager,return_internal_reference<>(), "Gets the application's :class:`.ServiceManager`")
        .def("event_dispatcher", &swganh::app::SwganhKernel::GetEventDispatcher, return_internal_reference<>(), "gets the applications :class:`.EventDispatcher'")
        ;

    class_<anh::service::ServiceManager, boost::noncopyable>("ServiceManager", "provides an interface to common services", no_init)
       .def("combat_service", make_function(
               std::bind(&anh::service::ServiceManager::GetService<swganh::combat::CombatService>, std::placeholders::_1, "CombatService"),
               return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::combat::CombatService*, anh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.CombatService`")
        .def("simulation_service", make_function(
               std::bind(&anh::service::ServiceManager::GetService<swganh::simulation::SimulationService>, std::placeholders::_1, "SimulationService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::simulation::SimulationService*, anh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.SimulationService`")
        .def("social_service", make_function(
               std::bind(&anh::service::ServiceManager::GetService<swganh::social::SocialService>, std::placeholders::_1, "SocialService"),
                return_value_policy<reference_existing_object>(),
                boost::mpl::vector<swganh::social::SocialService*, anh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.SocialService`")
       ;
       
}
void exportEventDispatcher()
{
    class_<anh::EventDispatcher, boost::noncopyable>("EventDispatcher", no_init)
        .def("dispatch",
            make_function(
                std::bind(&anh::EventDispatcher::Dispatch, std::placeholders::_1, std::placeholders::_2),
                default_call_policies(),
                boost::mpl::vector<void, anh::EventDispatcher*, std::shared_ptr<anh::EventInterface>>()), 
            "dispatches an event to be processed later")
        .def("subscribe", &anh::EventDispatcher::Subscribe, "subscribes to an event and attaches a callback") 
        ;
}
#endif // SWGANH_APP_SWGANH_KERNEL_BINDING_H_
