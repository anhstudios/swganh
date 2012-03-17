#include "swganh_kernel_binding.h"

BOOST_PYTHON_MODULE(py_app) 
{
    docstring_options local_docstring_options(true, true, false);

    exportSWGANHKernel();
}