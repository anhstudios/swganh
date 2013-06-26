// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/logger.h"
#include "swganh/scripting/python_shared_ptr.h"


#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "sui_service_interface.h"
#include "sui_window_interface.h"

#include "swganh_core/object/object.h"

#include "swganh/scripting/utilities.h"

using namespace swganh::sui;

namespace bp = boost::python;
using namespace std;

//Converter Functions 
struct PythonCallback
{
	PythonCallback(const bp::object obj, const std::string& callback)
	{
		if(obj)
		{
			callback_ = obj.attr(callback.c_str());
			if(callback_)
			{
				// success
			}
			else
			{
				// Error
				swganh::scripting::logPythonException();
			}
		}
	}

	virtual ~PythonCallback(){}

	bp::object module_;
	bp::object callback_;
};

//Subscription wrapper
std::shared_ptr<SUIWindowInterface> SubcribeWrapper(std::shared_ptr<SUIWindowInterface> self, uint32_t event_id, std::string event_source, InputTrigger input_trigger, 
													std::vector<std::string> result_list, PythonCallback funct)
{
	return self->SubscribeToEventCallback(event_id, event_source, input_trigger, result_list,
	[=](std::shared_ptr<swganh::object::Object> object, uint32_t event_type, std::vector<std::wstring> result_list) -> bool
	{ 
		swganh::scripting::ScopedGilLock lock;
		return bp::extract<bool>(funct.callback_(object, event_type, result_list));
	});
}

//CreateListBox Wrapper
std::shared_ptr<SUIWindowInterface> CreateListBoxWrapper(SUIServiceInterface* self, ListBoxType lstBox_type, 
			std::wstring title, std::wstring prompt, std::vector<std::wstring> dataList, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return self->CreateListBox(lstBox_type, title, prompt, dataList, owner, ranged_object, max_distance);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ListBoxOverloads, CreateListBoxWrapper, 6, 8)

//Custom Create Overload
boost::python::tuple CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(script_name, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateOverload, CreateSUIWindow, 2, 4)

//Message Box Create Overload
boost::python::tuple CreateMessageBox(MessageBoxType msgBox_type, std::wstring title, std::wstring caption,
			std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> ranged_object = nullptr, 
			float max_distance = 0)
{
	return boost::python::make_tuple(msgBox_type, title, caption, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateMessageOverload, CreateMessageBox, 4, 6)

boost::python::tuple CreateInputBox(InputBoxType iptBox_type, std::wstring title, std::wstring prompt, 
			uint32_t input_max_length, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(iptBox_type, title, prompt, input_max_length, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateInputOverload, CreateInputBox, 5, 7)


void exportSuiService()
{
	bp::enum_<InputTrigger>("InputTrigger")
		.value("UPDATE", InputTrigger::TRIGGER_UPDATE)
		.value("OK", InputTrigger::TRIGGER_OK)
		.value("CANCEL", InputTrigger::TRIGGER_CANCEL);

	bp::enum_<MessageBoxType>("MessageBoxType")
		.value("OK", MessageBoxType::MESSAGE_BOX_OK)
		.value("OK_CANCEL", MessageBoxType::MESSAGE_BOX_OK_CANCEL)
		.value("YES_NO", MessageBoxType::MESSAGE_BOX_YES_NO);

	bp::enum_<InputBoxType>("InputBoxType")
		.value("OK", InputBoxType::INPUT_BOX_OK)
		.value("OK_CANCEL", InputBoxType::INPUT_BOX_OKCANCEL);

	bp::enum_<ListBoxType>("ListBoxType")
		.value("OK", ListBoxType::LIST_BOX_OK)
		.value("OK_CANCEL", ListBoxType::LIST_BOX_OKCANCEL);

	bp::class_<SUIWindowInterface, std::shared_ptr<SUIWindowInterface>, boost::noncopyable>("SUIWindow", "A SUI window class.", 
		bp::no_init)
		.def("clearDataSource", &SUIWindowInterface::ClearDataSource, "Instructs the client to clear a data source. Takes a location string (ie. FullPath:Name), and a new value")
		.def("setProperty", &SUIWindowInterface::SetProperty, "Instructs the client to set a particular ui property. Takes a location string (ie. FullPath:Name), and a new value")
		.def("addProperty", &SUIWindowInterface::AddProperty, "Instructs the client to add a particular ui property. Takes a location string (ie. FullPath:Name), and a new value")
		.def("subscribeToEventCallback", SubcribeWrapper, "Subscribes the SUI system to a particular client ui event. This can be used to return values of properties when the client clicks a button or exits a window.")
		.def("addDataSourceContainer", &SUIWindowInterface::AddDataSourceContainer, "Adds a data source container to the UI.")
		.def("clearComponents", &SUIWindowInterface::ClearComponents, "Clears the components of this UI. This is typically for internal use only.")
		.def("getOwner", &SUIWindowInterface::GetOwner, "Returns the owner of this window.")
		.def("getRangedObject", &SUIWindowInterface::GetRangedObject, "Returns the ranged object this window is attached to.")
		.def("setRangedObject", &SUIWindowInterface::SetRangedObject, "Sets the ranged object for this window.")
		.def("getMaxDistance", &SUIWindowInterface::GetMaxDistance, "Returns the distance from the ranged object at which to automatically destroy this window.")
		.def("setMaxDistance", &SUIWindowInterface::SetMaxDistance, "Sets the distance from the ranged object at which to automatically destroy this window.")
		.def("getWindowId", &SUIWindowInterface::GetWindowId, "Returns this window's unique id.");

	bp::class_<SUIServiceInterface, std::shared_ptr<SUIServiceInterface>, boost::noncopyable>("SUIService", "The SUI service can be used to display custom interfaces on connected clients. It is used for several core game systems.", bp::no_init)
		.def("createSUIWindow", &SUIServiceInterface::CreateSUIWindow, CreateOverload(bp::args("script_name", "owner", "ranged_object", "max_distance"), "Creates a new SUIWindow."))
		.def("createMessageBox", &SUIServiceInterface::CreateMessageBox, CreateMessageOverload(bp::args("msgBox_type", "title", "caption", "owner", "ranged_object", "max_distance"),"Creates a SUIWindow and fills in properties for a basic message box."))
		.def("createListBox", CreateListBoxWrapper, ListBoxOverloads(bp::args("self", "lstBox_type", "title", "prompt", "dataList", "owner", "ranged_object", "range"),"Creates a SUIWindow and fills in properties for a basic list box."))
		.def("createInputBox", &SUIServiceInterface::CreateInputBox, CreateInputOverload(bp::args("iptBox_type", "title", "caption", "max_input_length", "owner", "ranged_object", "max_distance"), "Creates a SUIWindow and fills in properties for a basic input box."))
		.def("openSUIWindow", &SUIServiceInterface::OpenSUIWindow, "Creates the given window for the window's current owner.")
		.def("updateSUIWindow", &SUIServiceInterface::UpdateSUIWindow, "Updates the given window for the window's current owner.")
		.def("getSUIWindowById", &SUIServiceInterface::GetSUIWindowById, "Gets the SUI window associated with the given id from the given owner.")
		.def("getSUIWindowByScriptName", &SUIServiceInterface::GetSUIWindowByScriptName, "Gets the SUI window associated with the given script name from the given owner")
		.def("closeSUIWindow", &SUIServiceInterface::CloseSUIWindow, "Forcefully closes the SUI window with the given id for the given owner");

	bp::class_<std::vector<std::string>>("ResultList", "list for results of a SUI window")
		.def(bp::vector_indexing_suite<std::vector<std::string>, true>());

	bp::class_<std::vector<std::wstring>>("EventResultList", "list for results the event of a SUI window")
		.def(bp::vector_indexing_suite<std::vector<std::wstring>, true>());

	bp::class_<PythonCallback>("PythonCallback", bp::init<bp::object, std::string>());
}
