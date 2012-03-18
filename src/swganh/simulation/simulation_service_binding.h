#ifndef SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_
#define SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_

#include "anh/python_shared_ptr.h"
#include "simulation_service.h"

#include <boost/python.hpp>

using namespace swganh::simulation;
using namespace boost::python;
using namespace std;

void exportSimulationService()
{
    class_<SimulationService, shared_ptr<SimulationService>, boost::noncopyable>("SimulationService", "The simulation service handles the current scenes aka planets", no_init)
        .def("persist", &SimulationService::PersistObject, "persists the specified object and it's containing objects")

        ;
}

#endif //SWGANH_SIMULATION_SIMULATION_SERVICE_BINDING_H_