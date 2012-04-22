/*
 This file is part of SWGANH. For more information, visit http://swganh.com

 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef  SWGANH_OBJECT_OBJECT_BINDING_H_
#define  SWGANH_OBJECT_OBJECT_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/object/object.h"
#include "swganh/object/object_controller_binding.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

void exportObject()
{
    //std::shared_ptr<ObjectController> (ObjectWrapper::*GetControllerPtr)() = &ObjectWrapper::GetController;

    typedef void (Object::*NotifyObserversFunc)(const anh::ByteBuffer& message);

    class_<Object, std::shared_ptr<Object>, boost::noncopyable>("Object", "The Base SWG Object that all Objects inherit from", no_init)
		.add_property("id", &Object::GetObjectId, "Gets The id of the object")
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
		;
}

#endif   //SWGANH_OBJECT_OBJECT_BINDING_H_