// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/python_shared_ptr.h"
#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

#include "sui_service_interface.h"
#include "sui_window_interface.h"

#include "swganh/object/object.h"

using namespace swganh::sui;

using namespace boost::python;
using namespace std;

boost::python::tuple GetNewSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(script_name, owner, ranged_object, max_distance);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addGetNewOverload, GetNewSUIWindow, 2, 4);

void exportSuiService()
{
	enum_<InputTrigger>("InputTrigger")
		.value("update", InputTrigger::TRIGGER_UPDATE)
		.value("ok", InputTrigger::TRIGGER_OK)
		.value("cancel", InputTrigger::TRIGGER_CANCEL);

	class_<SUIWindowInterface, std::shared_ptr<SUIWindowInterface>, boost::noncopyable>("SUIWindow", "A SUI window class.", 
		no_init)
		.def("ClearDataSource", &SUIWindowInterface::ClearDataSource, "Instructs the client to clear a data source. Takes a location string (ie. FullPath:Name), and a new value")
		.def("SetProperty", &SUIWindowInterface::SetProperty, "Instructs the client to set a particular ui property. Takes a location string (ie. FullPath:Name), and a new value")
		.def("AddProperty", &SUIWindowInterface::AddProperty, "Instructs the client to add a particular ui property. Takes a location string (ie. FullPath:Name), and a new value")
		.def("SubscribeToEventCallback", &SUIWindowInterface::SubscribeToEventCallback, "Subscribes the SUI system to a particular client ui event. This can be used to return values of properties when the client clicks a button or exits a window.")
		.def("AddDataSourceContainer", &SUIWindowInterface::AddDataSourceContainer, "Adds a data source container to the UI.")
		.def("ClearComponents", &SUIWindowInterface::ClearComponents, "Clears the components of this UI. This is typically for internal use only.")
		.def("GetOwner", &SUIWindowInterface::GetOwner, "Returns the owner of this window.")
		.def("GetRangedObject", &SUIWindowInterface::GetRangedObject, "Returns the ranged object this window is attached to.")
		.def("SetRangedObject", &SUIWindowInterface::SetRangedObject, "Sets the ranged object for this window.")
		.def("GetMaxDistance", &SUIWindowInterface::GetMaxDistance, "Returns the distance from the ranged object at which to automatically destroy this window.")
		.def("SetMaxDistance", &SUIWindowInterface::SetMaxDistance, "Sets the distance from the ranged object at which to automatically destroy this window.")
		.def("GetWindowId", &SUIWindowInterface::GetWindowId, "Returns this window's unique id.");

	class_<SUIServiceInterface, std::shared_ptr<SUIServiceInterface>, boost::noncopyable>("SUIService", "The SUI service can be used to display custom interfaces on connected clients. It is used for several core game systems.", no_init)
		.def("GetNewSUIWindow", &SUIServiceInterface::GetNewSUIWindow, addGetNewOverload(args("script_name", "owner", "ranged_object", "max_distance"), "Creates a new SUIWindow."))
		.def("CreateSUIWindow", &SUIServiceInterface::CreateSUIWindow, "Creates the given window for the window's current owner.")
		.def("UpdateSUIWindow", &SUIServiceInterface::UpdateSUIWindow, "Updates the given window for the window's current owner.")
		.def("GetSUIWindowById", &SUIServiceInterface::GetSUIWindowById, "Gets the SUI window associated with the given id from the given owner.")
		.def("CloseSUIWindow", &SUIServiceInterface::CloseSUIWindow, "Forcefully closes the SUI window with the given id for the given owner");
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_