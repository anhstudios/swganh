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
        /*.def("service_manager", &anh::app::KernelWrapper::GetServiceManager,return_internal_reference<>(), "Gets the apps service manager")
        .def("simulation_service", &anh::app::KernelWrapper::GetSimulationService, return_internal_reference<>(), "returns an internal reference of the simulation service, see @SimulationService")
        .def("combat_service", &anh::app::KernelWrapper::GetCombatService, return_internal_reference<>(), "returns an internal refrence of the combat service, see @CombatService")*/
        ;
    class_<SwganhKernel, bases<KernelInterface>, boost::noncopyable>("SWGKernel")
        .def("app_config", &swganh::app::SwganhKernel::GetAppConfig, return_value_policy<copy_non_const_reference>(), "gets the app configuration")
        .def("service_manager", &swganh::app::SwganhKernel::GetServiceManager,return_internal_reference<>(), "Gets the apps service manager")
        //.def("simulation_service", &swganh::app::SwganhKernel::GetSimulationService, return_internal_reference<>(), "returns an internal reference of the simulation service, see @SimulationService")
        ;

    class_<anh::service::ServiceManager, boost::noncopyable>("ServiceManager", no_init)
        .def("combat_service", make_function(
                bind(&anh::service::ServiceManager::GetService<swganh::combat::CombatService>, std::placeholders::_1, "CombatService"),
                 default_call_policies(),
                 boost::mpl::vector<std::shared_ptr<swganh::combat::CombatService>, anh::service::ServiceManager*>()),
                 "returns an internal refrence of the combat service, see @CombatService")
        ;
}
#endif // SWGANH_APP_SWGANH_KERNEL_BINDING_H_