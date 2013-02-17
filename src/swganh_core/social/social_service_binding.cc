// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/social/social_service_binding.h"

BOOST_PYTHON_MODULE(py_social) 
{
    docstring_options local_docstring_options(true, true, false);

    exportSocialService();
}