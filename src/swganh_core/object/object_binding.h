// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif
#include "swganh/scripting/python_shared_ptr.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/cell/cell.h"
#include "swganh_core/object/intangible/intangible.h"
#include "swganh_core/object/object_controller_binding.h"
#include "swganh_core/object/permissions/container_permissions_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "swganh_core/sui/radial_binding.h"

#include "swganh_core/messages/system_message.h"

#include "swganh_core/messages/update_transform_message.h"
#include "swganh_core/messages/update_transform_with_parent_message.h"

using namespace boost::python;
using namespace std;
using namespace swganh::object;

boost::python::tuple AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> newObject, int32_t arrangement_id=-2)
{
	return boost::python::make_tuple(requester, newObject,  arrangement_id);
}

boost::python::tuple TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, glm::vec3 new_position, int32_t arrangement_id=-2)
{
	return boost::python::make_tuple(requester, object, newContainer, new_position, arrangement_id);
}

void SendSystemMessage1(std::shared_ptr<Object> requester, std::string filename, std::string label)
{
	SystemMessage::Send(requester, filename, label);
}

void SendSystemMessage2(std::shared_ptr<Object> requester, std::wstring custom_message, bool chatbox_only=true, bool send_to_inrange=true)
{
	SystemMessage::Send(requester, custom_message, chatbox_only, send_to_inrange);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(SysMsg2Overloads, SendSystemMessage2, 2, 4);

void SendSystemMessage3(std::shared_ptr<Object> requester, swganh::messages::OutOfBand out_of_band, bool chatbox_only=true, bool send_to_inrange=true)
{
	SystemMessage::Send(requester, out_of_band, chatbox_only, send_to_inrange);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(SysMsg3Overloads, SendSystemMessage3, 2, 4);

void SendSystemMessage4(std::shared_ptr<Object> requester, std::wstring custom_message, swganh::messages::OutOfBand out_of_band, bool chatbox_only=true, bool send_to_inrange=true)
{
	SystemMessage::Send(requester, custom_message, out_of_band, chatbox_only, send_to_inrange);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(SysMsg4Overloads, SendSystemMessage4, 3, 5);

void SendFlyText(std::shared_ptr<Object> requester, std::string fly_text, controllers::FlyTextColor color)
{
	SystemMessage::FlyText(requester, fly_text, color);
}

void SendClientEffect1(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service,
		std::string client_effect_file, std::string planet_name, glm::vec3 location_coordinates, float range, uint64_t cell_id=0)
{
	SystemMessage::ClientEffect(simulation_service, client_effect_file, planet_name, location_coordinates, range, cell_id);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEffect1Overload, SendClientEffect1, 5, 6);

void SendClientEffect2(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service,
		std::string client_effect_file, uint32_t planet_id, glm::vec3 location_coordinates, float range, uint64_t cell_id=0)
{
	SystemMessage::ClientEffect(simulation_service, client_effect_file, planet_id, location_coordinates, range, cell_id);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEffect2Overload, SendClientEffect2, 5, 6);

void SendClientEffect3(std::shared_ptr<swganh::object::Object> object, std::string client_effect_file, 
		std::string auxiliary_string="head", bool send_in_range=true)
{
	SystemMessage::ClientEffect(object, client_effect_file, auxiliary_string, send_in_range);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEffect3Overload, SendClientEffect3, 2, 4);

void SendClientEffect4(std::shared_ptr<swganh::object::Object> object, std::string client_effect_file, 
		glm::quat orientation, glm::vec3 offset, bool send_in_range=true)
{
	SystemMessage::ClientEffect(object, client_effect_file, orientation, offset, send_in_range);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEffect4Overload, SendClientEffect4, 4, 5);

void ClientEvent1(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, std::string event_group_string,
    std::string event_string, std::string planet_name_string, glm::vec3 location_coordinates, float range, uint64_t cell_id=0)
{
	SystemMessage::ClientEvent(simulation_service, event_group_string, event_string, planet_name_string, location_coordinates, range, cell_id);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEvent1Overload, ClientEvent1, 6, 7);

void ClientEvent2(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, std::string event_group_string,
    std::string event_string, uint32_t planet_id, glm::vec3 location_coordinates, float range, uint64_t cell_id=0)
{
	SystemMessage::ClientEvent(simulation_service, event_group_string, event_string, planet_id, location_coordinates, range, cell_id);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEvent2Overload, ClientEvent2, 6, 7);

void ClientEvent3(std::shared_ptr<swganh::object::Object> object, std::string event_string,
	std::string hardpoint_string="head", bool send_in_range=true)
{
	SystemMessage::ClientEvent(object, event_string, hardpoint_string, send_in_range);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientEvent3Overload, ClientEvent3, 2, 4);

void PlayMusic1(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, std::string planet_name,
				glm::vec3 point, float range, std::string music_name, uint32_t channel)
{
	SystemMessage::PlayMusic(simulation_service, planet_name, point, range, music_name, channel);
}

void PlayMusic2(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, uint32_t planet_id,
				glm::vec3 point, float range, std::string music_name, uint32_t channel)
{
	SystemMessage::PlayMusic(simulation_service, planet_id, point, range, music_name, channel);
}

void PlayMusic3(std::shared_ptr<swganh::object::Object> object, std::string music_name, uint32_t channel, bool send_in_range=false)
{
	SystemMessage::PlayMusic(object, music_name, channel, send_in_range);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(PlayMusic3Overload, PlayMusic3, 3, 4);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addObjectOverload, AddObject, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(transferObjectOverload, TransferObject, 4, 5)

void UpdatePosition(std::shared_ptr<Object> self, glm::vec3 position, glm::quat orientation, std::shared_ptr<Object> parent=nullptr)
{
	self->UpdatePosition(position, orientation, parent);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(UpdatePosOverload, UpdatePosition, 3, 4)


std::shared_ptr<swganh::object::Creature> ToCreature(std::shared_ptr<swganh::object::Object> obj)
{
	return static_pointer_cast<swganh::object::Creature>(obj);
}

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
		.def("add", &ContainerInterface::AddObject, addObjectOverload(args("requester", "newObject", "arrangement_id"), "Adds an object to the current object"))
		.def("remove", RemoveObject, "Removes an object fomr the current object")
		.def("transfer", &ContainerInterface::TransferObject, transferObjectOverload(args("object", "newContainer", "arrangement_id"), "Transfer an object to a different object"))
		.def("swapSlots", &Object::SwapSlots, "Change an objects current arrangement")	
		.def("container", &Object::GetContainer, "Gets the :class:`ContainerInterface` object of the current object")
		.def("hasContainedObjects", &Object::HasContainedObjects, "Checks to see if container has any objects in it")
		.def_readonly("id", &ContainerInterface::GetObjectId, "Gets the object id of the container")
		;
	
	class_<SystemMessage, boost::noncopyable>("SystemMessage", "Static class that deals with system messages.", no_init)
		.def("sendSystemMessage", SendSystemMessage1, "Send a system message to the requester, see :class:`")
		.def("sendSystemMessage", SendSystemMessage2, SysMsg2Overloads(args("requester", "custom_message", "chatbox_only", "send_to_inrange"), "Send a system message to the requester"))
		.def("sendSystemMessage", SendSystemMessage3, SysMsg3Overloads(args("requester", "out_of_band", "chatbox_only", "send_to_inrange"), "Send a system message to the requester"))
		.def("sendSystemMessage", SendSystemMessage4, SysMsg4Overloads(args("requester", "custom_message", "out_of_band", "chatbox_only", "send_to_inrange"), "Send a system message to the requester"))
		.def("sendFlyText", SendFlyText, "Sends Fly Text to the player, see :class:`.FlyTextColor`")
		.def("sendEffect", SendClientEffect1, ClientEffect1Overload(args("simulation_service", "client_effect_file", "planet_name", "location_coordinates", "range", "cell_id"), "Sends client effect to a particular location (planet as name)"))
		.def("sendEffect", SendClientEffect2, ClientEffect2Overload(args("simulation_service", "client_effect_file", "planet_id", "location_coordinates", "range", "cell_id"), "Sends client effect to a particular location (planet as id)"))
		.def("sendEffect", SendClientEffect3, ClientEffect3Overload(args("object", "client_effect_file", "auxiliary_string", "send_in_range"), "Sends client effect for a particular object"))
		.def("sendEffect", SendClientEffect4, ClientEffect4Overload(args("object", "client_effect_file", "orientation", "offset", "send_in_range"), "Sends client effect for a particular object"))
		.def("sendEvent", ClientEvent1, ClientEvent1Overload(args("simulation_service", "event_group_string", "event_string", "planet_name_string", "location_coordinates", "range", "cell_id"), "Sends client effect for a particular object"))
		.def("sendEvent", ClientEvent2, ClientEvent2Overload(args("simulation_service", "event_group_string", "event_string", "planet_id", "location_coordinates", "range", "cell_id"), "Sends client effect for a particular object"))
		.def("sendEvent", ClientEvent3, ClientEvent3Overload(args("object", "event_string", "hardpoint_string", "send_in_range"), "Sends client effect for a particular object"))
		.def("playMusic", PlayMusic1, "Plays music for objects in a particular range")
		.def("playMusic", PlayMusic2, "Plays music for objects in a particular range")
		.def("playMusic", PlayMusic3, PlayMusic3Overload(args("object", "music_name", "channel", "send_in_range"), "Plays music for a particular object (or in range of a particular object"))
		.staticmethod("sendSystemMessage")
		.staticmethod("sendFlyText")
		.staticmethod("sendEffect")
		.staticmethod("sendEvent")
		.staticmethod("playMusic")
		;

    class_<Object, bases<ContainerInterface>, std::shared_ptr<Object>, boost::noncopyable>("Object", "The Base SWG Object that all Objects inherit from")
		.add_property("id", &Object::GetObjectId, &Object::SetObjectId, "Gets or sets The id of the object")
		.add_property("scene_id", &Object::GetSceneId, &Object::SetSceneId, "Gets and Sets the scene id the object is in")
		.add_property("type", &Object::GetType, "Gets the type of the object")
		.add_property("position", &Object::GetPosition, &Object::SetPosition, "Gets and Sets the position of the object, using :class:`.Vec3`")
		.add_property("heading", &Object::GetHeading, "Gets the heading as an int of the object")
		.add_property("orientation", &Object::GetOrientation, &Object::SetOrientation, "Property to get or set the orientation of the object")
		.add_property("template", &Object::GetTemplate, &Object::SetTemplate, "the .iff file associated with this object"					)
		.add_property("volume", &Object::GetVolume, &Object::SetVolume, "Property to get or set the volume of the object (how much it can store)")
		.add_property("stf_name_file", &Object::GetStfNameFile, "gets the stf name file of the object")
		.add_property("stf_name_string", &Object::GetStfNameString, "gets the stf name file of the object")
		.def("stfName", &Object::SetStfName, "sets the full stf name, takes stf_name_file and stf_name_string as parameters")
		.add_property("custom_name", &Object::GetCustomName, &Object::SetCustomName, "Property to get and set the custom name")
        .def("controller", &Object::GetController, "Get the :class:`.ObjectController` of the object")
        .def("hasFlag", &Object::HasFlag, "Checks if the object has a specific flag set on it")
        .def("setFlag", &Object::SetFlag, "Sets a flag on the object")
        .def("removeFlag", &Object::RemoveFlag, "Removes a flag from the object")
		.def("hasAttribute", &Object::HasAttribute, "Returns true if the object has the given attribute.")
		.def("getFloatAttribute", &Object::GetAttribute<float>, "Gets the float attribute value")
		.def("setFloatAttribute", &Object::SetAttribute<float>, "Sets the float attribute value")
		.def("getIntAttribute", &Object::GetAttribute<int64_t>, "Gets the int attribute value")
		.def("setIntAttribute", &Object::SetAttribute<int64_t>, "Sets the int attribute value")
		.def("getStringAttribute", &Object::GetAttributeAsString, "Gets the string attribute value")
		.def("setStringAttribute", &Object::SetAttribute<wstring>, "sets the string attribute value")
		.def("getAttributeRecursive", &Object::GetAttributeRecursiveAsString, "Gets the attribute as a string recursively")
		.def("getIntAttributeRecursive", &Object::GetAttributeRecursive<int>, "Gets an integer attribute recursively")
		.def("getFloatAttributeRecursive", &Object::GetAttributeRecursive<float>, "Gets a float attribute recursively")
		.add_property("attribute_template_id", &Object::GetAttributeTemplateId, &Object::SetAttributeTemplateId, "Gets and Sets the attribute template_id")
		.def("eventDispatcher", &Object::SetEventDispatcher, "Sets the event dispatcher pointer")
		.def("updatePosition", &Object::UpdatePosition, "Updates the position and sends an update to the player")
		.def("toCreature", ToCreature)
		.def("rangeTo", &Object::RangeTo, "Gets the range from the object to the given target")
		.def("updatePosition", UpdatePosition, UpdatePosOverload(args("self", "position", "orientation", "parent"),"Updates the position and sends an update to the player"))
		;

	bp::class_<std::vector<int>>("IntVector")
		.def(bp::vector_indexing_suite<std::vector<int>>());

	class_<Cell, bases<Object>, std::shared_ptr<Cell>, boost::noncopyable>("Cell");
	class_<Intangible, bases<Object>, std::shared_ptr<Intangible>, boost::noncopyable>("Intangible");

	implicitly_convertible<std::shared_ptr<Intangible>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Intangible>, std::shared_ptr<ContainerInterface>>();
	
	implicitly_convertible<std::shared_ptr<Cell>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Cell>, std::shared_ptr<Intangible>>();
	implicitly_convertible<std::shared_ptr<Cell>, std::shared_ptr<ContainerInterface>>();
	implicitly_convertible<std::shared_ptr<Object>, std::shared_ptr<ContainerInterface>>();
}
