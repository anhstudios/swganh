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
#include <boost/python/call.hpp>

#include "sui_service_interface.h"
#include "sui_window_interface.h"

#include "swganh/object/object.h"

#include "swganh/scripting/utilities.h"

using namespace swganh::sui;

using namespace boost::python;
using namespace std;

//Fix GetNewSUIWindow
boost::python::tuple CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(script_name, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateOverload, CreateSUIWindow, 2, 4);

std::vector<std::string> vectorConvert(boost::python::list& list)
{
	std::vector<std::string> result_vector;
	for(int i=0; i < len(list); ++i)
	{
		result_vector.push_back(extract<std::string>(list[i]));
	}
	return result_vector;
}

WindowCallbackFunction callbackConvert(boost::python::object& funct)
{
	return [&funct] (std::shared_ptr<swganh::object::Object> object, uint32_t event_type, std::vector<std::wstring> result_list)
	{
		swganh::scripting::ScopedGilLock lock;
		funct(object, event_type, result_list);
	};
}

std::shared_ptr<SUIWindowInterface> SubcribeWrapper(std::shared_ptr<SUIWindowInterface> self, uint32_t event_id, std::string event_source, InputTrigger input_trigger, 
													boost::python::list result_list, boost::python::object funct)
{
	return self->SubscribeToEventCallback(event_id, event_source, input_trigger, vectorConvert(result_list), callbackConvert(funct));
}

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
		.def("SubscribeToEventCallback", SubcribeWrapper, "Subscribes the SUI system to a particular client ui event. This can be used to return values of properties when the client clicks a button or exits a window.")
		.def("AddDataSourceContainer", &SUIWindowInterface::AddDataSourceContainer, "Adds a data source container to the UI.")
		.def("ClearComponents", &SUIWindowInterface::ClearComponents, "Clears the components of this UI. This is typically for internal use only.")
		.def("GetOwner", &SUIWindowInterface::GetOwner, "Returns the owner of this window.")
		.def("GetRangedObject", &SUIWindowInterface::GetRangedObject, "Returns the ranged object this window is attached to.")
		.def("SetRangedObject", &SUIWindowInterface::SetRangedObject, "Sets the ranged object for this window.")
		.def("GetMaxDistance", &SUIWindowInterface::GetMaxDistance, "Returns the distance from the ranged object at which to automatically destroy this window.")
		.def("SetMaxDistance", &SUIWindowInterface::SetMaxDistance, "Sets the distance from the ranged object at which to automatically destroy this window.")
		.def("GetWindowId", &SUIWindowInterface::GetWindowId, "Returns this window's unique id.");

	class_<SUIServiceInterface, std::shared_ptr<SUIServiceInterface>, boost::noncopyable>("SUIService", "The SUI service can be used to display custom interfaces on connected clients. It is used for several core game systems.", no_init)
		.def("CreateSUIWindow", &SUIServiceInterface::CreateSUIWindow, CreateOverload(args("script_name", "owner", "ranged_object", "max_distance"), "Creates a new SUIWindow."))
		.def("OpenSUIWindow", &SUIServiceInterface::OpenSUIWindow, "Creates the given window for the window's current owner.")
		.def("UpdateSUIWindow", &SUIServiceInterface::UpdateSUIWindow, "Updates the given window for the window's current owner.")
		.def("GetSUIWindowById", &SUIServiceInterface::GetSUIWindowById, "Gets the SUI window associated with the given id from the given owner.")
		.def("CloseSUIWindow", &SUIServiceInterface::CloseSUIWindow, "Forcefully closes the SUI window with the given id for the given owner");
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_