// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh_core/object/object.h"
#include "swganh/object/object_controller_binding.h"
#include "swganh/object/permissions/container_permissions_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "swganh/sui/radial_binding.h"

#include "swganh_core/messages/system_message.h"

using namespace boost::python;
using namespace std;
using namespace swganh::object;

boost::python::tuple AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> newObject, int32_t arrangement_id=-2)
{
	return boost::python::make_tuple(newObject, requester, arrangement_id);
}

boost::python::tuple TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2)
{
	return boost::python::make_tuple(requester, object, newContainer, arrangement_id);
}

void SendSystemMessage1(std::shared_ptr<Object> requester, std::string filename, std::string label)
{
	SystemMessage::Send(requester, filename, label);
}
void SendSystemMessage2(std::shared_ptr<Object> requester, std::wstring& custom_message, bool chatbox_only, bool send_to_inrange)
{
	SystemMessage::Send(requester, custom_message, chatbox_only, send_to_inrange);
}
void SendSystemMessage3(std::shared_ptr<Object> requester, swganh::messages::OutOfBand& out_of_band, bool chatbox_only, bool send_to_inrange)
{
	SystemMessage::Send(requester, out_of_band, chatbox_only, send_to_inrange);
}
void SendSystemMessage4(std::shared_ptr<Object> requester, std::wstring custom_message, swganh::messages::OutOfBand& out_of_band, bool chatbox_only, bool send_to_inrange)
{
	SystemMessage::Send(requester, custom_message, out_of_band, chatbox_only, send_to_inrange);
}
void SendFlyText(std::shared_ptr<Object> requester, const std::string& fly_text, controllers::FlyTextColor color)
{
	SystemMessage::FlyText(requester, fly_text, color);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addObjectOverload, AddObject, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(transferObjectOverload, TransferObject, 3, 4)

struct ProsePackageWrapper : ProsePackage, wrapper<ProsePackage>
{
    ProsePackageWrapper() : ProsePackage(){}
};

void exportOutOfBand()
{
    enum_<ProseType>("ProseType", "Prose type of the :class:`.OutOfBand` Message")
        .value("TU", TU)
        .value("TT", TT)
        .value("TO", TO)
        .value("DI", DI)
        .value("DF", DF)
        ;
    class_<OutOfBand, boost::noncopyable>("OutOfBand","object that is used in the help with sending system stf messages")
        .def(init<std::string, std::string, ProseType, uint64_t>())
        .def(init<std::string, std::string, ProseType, std::wstring>())
        .def(init<std::string, std::string>())
    ;
}

void exportObject()
{
	enum_<controllers::FlyTextColor>("FlyTextColor", "Object that describes the different colors Fly Text Can be")
    .value("RED", controllers::RED)
    .value("GREEN", controllers::GREEN)
    .value("BLUE", controllers::BLUE)
    .value("WHITE", controllers::WHITE)
    .value("MIX", controllers::MIX)
    ;
	void (ContainerInterface::*RemoveObject)(shared_ptr<Object>, shared_ptr<Object>) = &ContainerInterface::RemoveObject;

	class_<ContainerInterface, std::shared_ptr<ContainerInterface>, boost::noncopyable>("ContainerInterface", "Container interface", no_init)
		.def("Add", &ContainerInterface::AddObject, addObjectOverload(args("requester", "newObject", "arrangement_id"), "Adds an object to the current object"))
		.def("Remove", RemoveObject, "Removes an object fomr the current object")
		.def("Transfer", &ContainerInterface::TransferObject, transferObjectOverload(args("object", "newContainer", "arrangement_id"), "Transfer an object to a different object"))
		.def("SwapSlots", &Object::SwapSlots, "Change an objects current arrangement")	
		.def("Container", &Object::GetContainer, "Gets the :class:`ContainerInterface` object of the current object")
		;
	
    class_<Object, bases<ContainerInterface>, std::shared_ptr<Object>, boost::noncopyable>("Object", "The Base SWG Object that all Objects inherit from")
		.add_property("id", &Object::GetObjectId, &Object::SetObjectId, "Gets or sets The id of the object")
		.add_property("scene_id", &Object::GetSceneId, "Gets the scene id the object is in")
		.add_property("type", &Object::GetType, "Gets the type of the object")
		.add_property("position", &Object::GetPosition, &Object::SetPosition, "Gets and Sets the position of the object, using :class:`.Vec3`")
		.add_property("heading", &Object::GetHeading, "Gets the heading as an int of the object")
		.add_property("orientation", &Object::GetOrientation, &Object::SetOrientation, "Property to get or set the orientation of the object")
		.add_property("template", &Object::GetTemplate, &Object::SetTemplate, "the .iff file associated with this object"					)
		.add_property("volume", &Object::GetVolume, &Object::SetVolume, "Property to get or set the volume of the object (how much it can store)")
		.add_property("stf_name_file", &Object::GetStfNameFile, "gets the stf name file of the object")
		.add_property("stf_name_string", &Object::GetStfNameString, "gets the stf name file of the object")
		.def("stf_name", &Object::SetStfName, "sets the full stf name, takes stf_name_file and stf_name_string as parameters")
		.add_property("custom_name", &Object::GetCustomName, &Object::SetCustomName, "Property to get and set the custom name")
        .def("Controller", &Object::GetController, "Get the :class:`.ObjectController` of the object")
        .def("HasFlag", &Object::HasFlag, "Checks if the object has a specific flag set on it")
        .def("SetFlag", &Object::SetFlag, "Sets a flag on the object")
        .def("RemoveFlag", &Object::RemoveFlag, "Removes a flag from the object")
		.def("SetMenuResponse", &Object::SetMenuResponse, "Sets the radial menu response from a python list")
		.def("get_float_attribute", &Object::GetAttribute<float>, "Gets the float attribute value")
		.def("set_float_attribute", &Object::SetAttribute<float>, "Sets the float attribute value")
		.def("get_int_attribute", &Object::GetAttribute<int32_t>, "Gets the int attribute value")
		.def("set_int_attribute", &Object::SetAttribute<int32_t>, "Sets the int attribute value")
		.def("get_string_attribute", &Object::GetAttribute<wstring>, "Gets the string attribute value")
		.def("set_string_attribute", &Object::SetAttribute<wstring>, "sets the string attribute value")
		.add_property("attribute_template_id", &Object::GetAttributeTemplateId, &Object::SetAttributeTemplateId, "Gets and Sets the attribute template_id")
		.def("event_dispatcher", &Object::SetEventDispatcher, "Sets the event dispatcher pointer")
		.def("SendSystemMessage", SendSystemMessage1, "Send a system message to the requester, see :class:`")
		.def("SendSystemMessage", SendSystemMessage2, "Send a system message to the requester")
		.def("SendSystemMessage", SendSystemMessage3, "Send a system message to the requester")
		.def("SendSystemMessage", SendSystemMessage4, "Send a system message to the requester")
		.def("SendFlyText", SendFlyText, "Sends Fly Text to the player, see :class:`.FlyTextColor`")
		
		;

	implicitly_convertible<std::shared_ptr<Object>, std::shared_ptr<ContainerInterface>>();
}
