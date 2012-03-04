#ifndef SWGANH_APP_SWGANH_KERNEL_BINDING_H_
#define SWGANH_APP_SWGANH_KERNEL_BINDING_H_

#include "swganh_kernel.h"

#include "anh/app/kernel_interface.h"
#include "anh/service/service_manager.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/combat/combat_service.h"

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
        //.def("simulation_service", &swganh::app::SwganhKernel::GetSimulationService, return_internal_reference<>(), "returns an internal reference of the simulation service, see @SimulationService")
        ;

    class_<anh::service::ServiceManager, boost::noncopyable>("ServiceManager", "provides an interface to common services", no_init)
        .def("combat_service", make_function(
                bind(&anh::service::ServiceManager::GetService<swganh::combat::CombatService>, std::placeholders::_1, "CombatService"),
                 default_call_policies(),
                 boost::mpl::vector<std::shared_ptr<swganh::combat::CombatService>, anh::service::ServiceManager*>()),
                 "returns an internal refrence of the :class:`.CombatService`")
        ;
}
#endif // SWGANH_APP_SWGANH_KERNEL_BINDING_H_