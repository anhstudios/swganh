// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "anh/python_shared_ptr.h"
#include "equipment_service_interface.h"

#include <boost/python.hpp>

using namespace swganh::equipment;
using namespace boost::python;
using namespace std;

void exportEquipmentService()
{
    class_<EquipmentServiceInterface, shared_ptr<EquipmentServiceInterface>, boost::noncopyable>("EquipmentService", "The equipment service is a utility class for helping with equipping.", no_init)
        .def("GetSlotIdByName", &EquipmentServiceInterface::GetSlotIdByName,"Returns the slot id associated with a slot name. This slot_id can then be used by objects to do equipment operations.")
		.def("GetSlotNameById", &EquipmentServiceInterface::GetSlotNameById,"Returns the slot name by the slot_id. This can be used to 'undo' GetSlotIdByName")
		.def("ClearSlot", &EquipmentServiceInterface::ClearSlot,"Utility method that gets the slot id for a slot name and then clears that slot from an object.")
		.def("GetEquippedObject", &EquipmentServiceInterface::GetEquippedObject,"Utility method that gets the slot id for a slot name and then returns the object in that slot for a given object.")
        ;
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_