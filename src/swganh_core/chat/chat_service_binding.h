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
        ;
}
