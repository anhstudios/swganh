// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/object/object.h"
#include "swganh/object/object_controller_binding.h"
#include "swganh/object/permissions/container_permissions_interface.h"
#include "swganh/object/permissions/default_permission.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "swganh/sui/radial_binding.h"

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

void SetDefaultPermissions(std::shared_ptr<Object> requester)
{
	requester->SetPermissions(make_shared<DefaultPermission>());	
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addObjectOverload, AddObject, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(transferObjectOverload, TransferObject, 3, 4)

void exportObject()
{
    //std::shared_ptr<ObjectController> (ObjectWrapper::*GetControllerPtr)() = &ObjectWrapper::GetController;

    typedef void (Object::*NotifyObserversFunc)(const anh::ByteBuffer& message);
	
	void (ContainerInterface::*RemoveObject)(shared_ptr<Object>, shared_ptr<Object>) = &ContainerInterface::RemoveObject;
	

	class_<ContainerInterface, std::shared_ptr<ContainerInterface>, boost::noncopyable>("ContainerInterface", "Container interface", no_init)
		.def("Add", &ContainerInterface::AddObject, addObjectOverload(args("requester", "newObject", "arrangement_id"), "Adds an object to the current object"))
		.def("Remove", RemoveObject, "Removes an object fomr the current object")
		.def("Transfer", &ContainerInterface::TransferObject, transferObjectOverload(args("object", "newContainer", "arrangement_id"), "Transfer an object to a different object"))
		.def("SwapSlots", &Object::SwapSlots, "Change an objects current arrangement")		
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
		.def("NotifyObservers", NotifyObserversFunc(&Object::NotifyObservers), "Notifies Observers of the passed in message")
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
		
		;

	implicitly_convertible<std::shared_ptr<Object>, std::shared_ptr<ContainerInterface>>();
}
