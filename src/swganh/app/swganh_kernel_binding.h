#ifndef SWGANH_APP_SWGANH_KERNEL_BINDING_H_
#define SWGANH_APP_SWGANH_KERNEL_BINDING_H_

#include "swganh_kernel.h"

#include "anh/service/service_manager.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/combat/combat_service.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;

namespace swganh {
namespace app {

struct SwganhKernelWrapper : SwganhKernel, wrapper<SwganhKernel>
{
    swganh::simulation::SimulationService* GetSimulationService() { 
        auto simulation_service = GetServiceManager()->GetService<swganh::simulation::SimulationService>("SimulationService");
        return simulation_service.get();
    }

    swganh::combat::CombatService* GetCombatService() { 
        auto combat_service = GetServiceManager()->GetService<swganh::combat::CombatService>("CombatService");
        return combat_service.get();
    }
};

void exportSWGANHKernel()
{
    class_<SwganhKernelWrapper, boost::noncopyable>("Kernel", no_init)
        .def("app_config", &SwganhKernelWrapper::GetAppConfig, return_value_policy<copy_non_const_reference>(), "gets the app configuration")
        .def("service_manager", &SwganhKernelWrapper::GetServiceManager,return_internal_reference<>(), "Gets the apps service manager")
        .def("simulation_service", &SwganhKernelWrapper::GetSimulationService, return_internal_reference<>(), "returns an internal reference of the simulation service, see @SimulationService")
        .def("combat_service", &SwganhKernelWrapper::GetCombatService, return_internal_reference<>(), "returns an internal refrence of the combat service, see @CombatService")
        ;
}

}}
#endif // SWGANH_APP_SWGANH_KERNEL_BINDING_H_