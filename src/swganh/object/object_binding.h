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

#include "swganh/object/object.h"
#include "swganh/object/object_controller_binding.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

namespace swganh {
namespace object {
struct ObjectWrapper : Object, wrapper<Object>
{
};
}}

void exportObject()
{
    //std::shared_ptr<ObjectController> (ObjectWrapper::*GetControllerPtr)() = &ObjectWrapper::GetController;

    typedef void (ObjectWrapper::*NotifyObserversFunc)(const anh::ByteBuffer& message);

    class_<ObjectWrapper, boost::noncopyable>("Object", "The Base SWG Object that all Objects inherit from", no_init)
		.add_property("id", &ObjectWrapper::GetObjectId, "Gets The id of the object")
		.add_property("scene_id", &ObjectWrapper::GetSceneId, "Gets the scene id the object is in")
		.add_property("type", &ObjectWrapper::GetType, "Gets the type of the object")
		.add_property("position", &ObjectWrapper::GetPosition, &ObjectWrapper::SetPosition, "Gets and Sets the position of the object, using :class:`.Vec3`")
		.add_property("heading", &ObjectWrapper::GetHeading, "Gets the heading as an int of the object")
		.add_property("orientation", &ObjectWrapper::GetOrientation, &ObjectWrapper::SetOrientation, "Property to get or set the orientation of the object")
		.add_property("template", make_function(
								&ObjectWrapper::GetTemplate, return_value_policy<copy_const_reference>()
							)
							, &ObjectWrapper::SetTemplate, "the .iff file associated with this object"
					)
		.add_property("volume", &ObjectWrapper::GetVolume, &ObjectWrapper::SetVolume,
						"Property to get or set the volume of the object (how much it can store)"
					)
		.add_property("stf_name_file", make_function(
									&ObjectWrapper::GetStfNameFile, return_value_policy<copy_const_reference>()
								), "gets the stf name file of the object"
					)
		.add_property("stf_name_string", make_function(
									&ObjectWrapper::GetStfNameString, return_value_policy<copy_const_reference>()
								), "gets the stf name file of the object"
					)
		.def("stf_name", &ObjectWrapper::SetStfName, "sets the full stf name, takes stf_name_file and stf_name_string as parameters")
		.add_property("custom_name", make_function(
								&ObjectWrapper::GetCustomName, return_value_policy<copy_const_reference>()
							), &ObjectWrapper::SetCustomName, "Property to get and set the custom name"
					)
		.def("NotifyObservers", NotifyObserversFunc(&ObjectWrapper::NotifyObservers), "Notifies Observers of the passed in message")
        .def("Controller", &ObjectWrapper::GetController, "Get the :class:`.ObjectController` of the object")
		;
}

#endif   //SWGANH_OBJECT_OBJECT_BINDING_H_