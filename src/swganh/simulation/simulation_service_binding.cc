#include "swganh/simulation/simulation_service_binding.h"

BOOST_PYTHON_MODULE(py_simulation) 
{
    docstring_options local_docstring_options(true, true, false);

    exportSimulationService();
}