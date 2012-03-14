
#ifndef SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_BINDING_H_
#define SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_BINDING_H_

#include "swganh/messages/controllers/combat_action_message.h"

#include <boost/python.hpp>
#include <cstdint>
#include <string>

using namespace boost::python;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace std;

//struct ByteBufferWrapper : anh::ByteBuffer, wrapper<anh::ByteBuffer>
//{
//};
//
//
//struct ObjControllerMessageWrapper : ObjControllerMessage, wrapper<ObjControllerMessage>
//{
//};
//
//void exportObjControllerMessage()
//{
//	class_<ByteBufferWrapper, boost::noncopyable>("ByteBuffer", no_init)
//		;
//	class_<ObjControllerMessageWrapper, boost::noncopyable>("ObjControllerMessage")
//		.def(init<uint32_t, const CombatActionMessageWrapper&>())
//		.def_readwrite("uknown", &ObjControllerMessageWrapper::unknown)
//		.def_readwrite("header", &ObjControllerMessageWrapper::header)
//		.def_readwrite("object_id", &ObjControllerMessageWrapper::object_id)
//		.def_readwrite("tick_count", &ObjControllerMessageWrapper::tick_count)
//		.def_readwrite("data", &ObjControllerMessageWrapper::data)
//		;
//
//}

struct CombatActionMessageWrapper : CombatActionMessage, wrapper<CombatActionMessage>
{
};
void exportCombatActionMessage()
{
	class_<CombatActionMessageWrapper, boost::noncopyable>("CombatActionMessage", no_init)
		.def_readwrite("action_crc", &CombatActionMessageWrapper::action_crc, "the action crc for the combat message")
		.def_readwrite("attacker_id", &CombatActionMessageWrapper::attacker_id, "the attacker id for the combat message")
		.def_readwrite("weapon_id", &CombatActionMessageWrapper::weapon_id, "the weapon id for the combat message")
		.def_readwrite("attacker_end_posture", &CombatActionMessageWrapper::attacker_end_posture, "the attacker the posture is to be at the end of the attack")
		.def_readwrite("trails_bit_flag", &CombatActionMessageWrapper::trails_bit_flag, "the trails bit that displays combat trails for the combat message")
		.def_readwrite("combat_special_move_effect", &CombatActionMessageWrapper::combat_special_move_effect, "the combat special move effect for the combat message")
		; 
}
#endif  // SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_BINDING_H_
