#include "swganh/social/social_service_binding.h"

BOOST_PYTHON_MODULE(py_social) 
{
    docstring_options local_docstring_options(true, true, false);

    exportSocialService();
}