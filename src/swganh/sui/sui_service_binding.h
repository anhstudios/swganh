// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/logger.h"
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

//Converter Functions
template <typename T>
std::vector<T> vectorConvert(boost::python::list& list)
{
	std::vector<T> result_vector;
	for(int i=0; i < len(list); ++i)
	{
		result_vector.push_back(extract<T>(list[i]));
	}
	return result_vector;
}

WindowCallbackFunction callbackConvert(boost::python::object funct)
{
	try {
		WindowCallbackFunction callback_func = boost::python::extract<WindowCallbackFunction>(funct);
		return callback_func;
	}
	catch(...)
	{
		PyErr_Print();
	}
	return nullptr;
	
	//
	//return [funct] (std::shared_ptr<swganh::object::Object> object, uint32_t event_type, std::vector<std::wstring> result_list) -> bool
	//{
	//	try {
	//		boost::python::list l;
	//		for (auto& v : result_list)
	//		{
	//			l.append(v);
	//		}
	//		swganh::scripting::ScopedGilLock lock;
	//		
	//		boost::python::object result = funct()(;

	//		if (!result.is_none())
	//		{
	//			return result(object, event_type, l);
	//		}
	//		
	//		//return funct(object, event_type, result_list);
	//	}
	//	catch(boost::python::error_already_set& e)
 //       {
	//		LOG(warning) << "Error in callback ";
 //           PyErr_Print();
 //       }
	//	return false;
	//};
}

//Subscription wrapper
std::shared_ptr<SUIWindowInterface> SubcribeWrapper(std::shared_ptr<SUIWindowInterface> self, uint32_t event_id, std::string event_source, InputTrigger input_trigger, 
													boost::python::list result_list, boost::python::object funct)
{
	return self->SubscribeToEventCallback(event_id, event_source, input_trigger, vectorConvert<std::string>(result_list), callbackConvert(funct));
}

//CreateListBox Wrapper
std::shared_ptr<SUIWindowInterface> CreateListBoxWrapper(SUIServiceInterface* self, ListBoxType lstBox_type, 
			std::wstring title, std::wstring prompt, boost::python::list dataList, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return self->CreateListBox(lstBox_type, title, prompt, vectorConvert<std::wstring>(dataList), owner, ranged_object, max_distance);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ListBoxOverloads, CreateListBoxWrapper, 6, 8);

//Custom Create Overload
boost::python::tuple CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(script_name, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateOverload, CreateSUIWindow, 2, 4);

//Message Box Create Overload
boost::python::tuple CreateMessageBox(MessageBoxType msgBox_type, std::wstring title, std::wstring caption,
			std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> ranged_object = nullptr, 
			float max_distance = 0)
{
	return boost::python::make_tuple(msgBox_type, title, caption, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateMessageOverload, CreateMessageBox, 4, 6);

boost::python::tuple CreateInputBox(InputBoxType iptBox_type, std::wstring title, std::wstring prompt, 
			uint32_t input_max_length, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
{
	return boost::python::make_tuple(iptBox_type, title, prompt, input_max_length, owner, ranged_object, max_distance);
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateInputOverload, CreateInputBox, 5, 7);


void exportSuiService()
{
	enum_<InputTrigger>("InputTrigger")
		.value("update", InputTrigger::TRIGGER_UPDATE)
		.value("ok", InputTrigger::TRIGGER_OK)
		.value("cancel", InputTrigger::TRIGGER_CANCEL);

	enum_<MessageBoxType>("MessageBoxType")
		.value("ok", MessageBoxType::MESSAGE_BOX_OK)
		.value("okcancel", MessageBoxType::MESSAGE_BOX_OK_CANCEL)
		.value("yesno", MessageBoxType::MESSAGE_BOX_YES_NO);

	enum_<InputBoxType>("InputBoxType")
		.value("ok", InputBoxType::INPUT_BOX_OK)
		.value("okcancel", InputBoxType::INPUT_BOX_OKCANCEL);

	enum_<ListBoxType>("ListBoxType")
		.value("ok", ListBoxType::LIST_BOX_OK)
		.value("okcancel", ListBoxType::LIST_BOX_OKCANCEL);

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
		.def("CreateMessageBox", &SUIServiceInterface::CreateMessageBox, CreateMessageOverload(args("msgBox_type", "title", "caption", "owner", "ranged_object", "max_distance"),"Creates a SUIWindow and fills in properties for a basic message box."))
		.def("CreateListBox", CreateListBoxWrapper, ListBoxOverloads(args("self", "lstBox_type", "title", "prompt", "dataList", "owner", "ranged_object", "range"),"Creates a SUIWindow and fills in properties for a basic list box."))
		.def("CreateInputBox", &SUIServiceInterface::CreateInputBox, CreateInputOverload(args("iptBox_type", "title", "caption", "max_input_length", "owner", "ranged_object", "max_distance"), "Creates a SUIWindow and fills in properties for a basic input box."))
		
		.def("OpenSUIWindow", &SUIServiceInterface::OpenSUIWindow, "Creates the given window for the window's current owner.")
		.def("UpdateSUIWindow", &SUIServiceInterface::UpdateSUIWindow, "Updates the given window for the window's current owner.")
		.def("GetSUIWindowById", &SUIServiceInterface::GetSUIWindowById, "Gets the SUI window associated with the given id from the given owner.")
		.def("CloseSUIWindow", &SUIServiceInterface::CloseSUIWindow, "Forcefully closes the SUI window with the given id for the given owner");
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_