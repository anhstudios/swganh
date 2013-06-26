// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "chat_service_interface.h"
#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

using namespace swganh::chat;
using namespace boost::python;
using namespace std;

void exportChatService()
{
    class_<ChatServiceInterface, shared_ptr<ChatServiceInterface>, boost::noncopyable>("ChatService", "The chat service processes in-game chat features.", no_init)
        .def("sendPersistentMessage", static_cast<bool(ChatServiceInterface::*)(
            const std::string& recipient,
            const std::string& sender,
            const std::string& sender_game,
            const std::string& sender_galaxy,
            const std::wstring& subject,
            const std::wstring& message,
            const std::vector<char>& attachments)>(&ChatServiceInterface::SendPersistentMessage),
            "Send a persistent message including attachments")
        .def("sendPersistentMessage", static_cast<bool(ChatServiceInterface::*)(
            const std::string& recipient,
            const std::string& sender,
            const std::wstring& subject, 
            const std::wstring& message)>(&ChatServiceInterface::SendPersistentMessage),
            "Send a persistent message without attachments")
		
		.def("getObjectIdByName", &ChatServiceInterface::GetObjectIdByCustomName, "Looks up the id of a player's creature by his name.")
		.def("getFullNameFromId", &ChatServiceInterface::GetFullNameFromId, return_value_policy<copy_const_reference>(), "Returns the full name of the player with the given id.")
		.def("getFirstNameFromId", &ChatServiceInterface::GetFirstNameFromId, "Returns the first name of the player with the given id.")
		.def("getFullNameFromFirstName", &ChatServiceInterface::GetFullNameFromFirstName, return_value_policy<copy_const_reference>(), "Returns the full name of the player with the given name. Equivalent to GetFullNameFromId(GetIdFromCustomName(name))")
		.def("getFirstNameFromFullName", &ChatServiceInterface::GetFirstNameFromFullName, "Returns the first name of the given set of names in lowercase.")

		.def("isOnline", (bool(ChatServiceInterface::*)(const std::string&)const) &ChatServiceInterface::IsOnline, "Determines player online status.")
		.def("isOnlineById", (bool(ChatServiceInterface::*)(uint64_t)const) &ChatServiceInterface::IsOnline, "Determines player online status.")

		.def("createRoom", &ChatServiceInterface::CreateRoom, "Creates a new room")
		.def("destroyRoom", &ChatServiceInterface::DestroyRoom, "Destroys a room")
		.def("forceJoinRoom", &ChatServiceInterface::ForceJoin, "Forces a player to join a channel.")
		.def("forceLeaveRoom", &ChatServiceInterface::ForceLeave, "Forces a player from a channel.")
		;
}
