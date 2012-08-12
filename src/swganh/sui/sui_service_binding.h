// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/python_shared_ptr.h"

#include "sui_service_interface.h"
#include "page.h"

#include <boost/python.hpp>

using namespace swganh::sui;
using namespace boost::python;
using namespace std;

void exportSocialService()
{
	class_<Page, shared_ptr<UIElement>, boost::noncopyable>("Page", "The main container used in ui windows.", no_init)
		;

    class_<SuiServiceInterface, shared_ptr<SuiServiceInterface>, boost::noncopyable>("SuiService", "The sui service handles operations with client ui.", no_init)
		.def("FindPageByName", &SuiServiceInterface::FindPageByName, "Finds a page based on it's name.")
		.def("CreatePage", &SuiServiceInterface::CreatePage, "Creates a new page and returns the window id")
		.def("ForceClose", &SuiServiceInterface::ForceClose, "Forcefully closes a created sui window by window id")
       ;
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_