// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif


#include "anh/python_shared_ptr.h"
#include "equipment_service_interface.h"
#include <swganh/object/object.h>
#include "swganh/object/player/player.h"

#include <boost/python.hpp>

using namespace swganh::object;
using namespace swganh::equipment;
using namespace boost::python;
using namespace std;

void exportEquipmentService()
{

	void (EquipmentServiceInterface::*ClearFunc)(std::shared_ptr<Object>, std::string) = &EquipmentServiceInterface::ClearSlot;
	std::shared_ptr<Object> (EquipmentServiceInterface::*GetFunc)(std::shared_ptr<Object>, std::string) = &EquipmentServiceInterface::GetEquippedObject;
	//std::shared_ptr<player::Player> (EquipmentServiceInterface::*GetPlayer)(std::shared_ptr<Object>, "ghost") = &EquipmentServiceInterface::GetEquippedObject<player::Player>;

    class_<EquipmentServiceInterface, shared_ptr<EquipmentServiceInterface>, boost::noncopyable>("EquipmentService", "The equipment service is a utility class for helping with equipping.", no_init)
        .def("GetSlotIdByName", &EquipmentServiceInterface::GetSlotIdByName,"Returns the slot id associated with a slot name. This slot_id can then be used by objects to do equipment operations.")
		.def("GetSlotNameById", &EquipmentServiceInterface::GetSlotNameById,"Returns the slot name by the slot_id. This can be used to 'undo' GetSlotIdByName")
		.def("ClearSlot", ClearFunc,"Utility method that gets the slot id for a slot name and then clears that slot from an object.")
		.def("GetEquippedObject", GetFunc,"Utility method that gets the slot id for a slot name and then returns the object in that slot for a given object.")
		.def("GetPlayerObject", make_function(
			std::bind(&EquipmentServiceInterface::GetEquippedObject<player::Player>, std::placeholders::_1, std::placeholders::_2, "ghost"),
			return_value_policy<return_by_value>(),
            boost::mpl::vector<shared_ptr<player::Player>, EquipmentServiceInterface*, shared_ptr<Object>>()),
			"returns the :class:`.Player` equipped by this creature object")
        ;
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_