// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/python_shared_ptr.h"
#include "social_service_interface.h"

#include <boost/python.hpp>

using namespace swganh::social;
using namespace boost::python;
using namespace std;

void exportSocialService()
{
    class_<SocialServiceInterface, shared_ptr<SocialServiceInterface>, boost::noncopyable>("SocialService", "The social service handles services that involve social actions", no_init)
        .def("add_friend", &SocialServiceInterface::AddFriend, "Checks the database to see if the character name exists and then adds the friend to the player")
        .def("add_ignore", &SocialServiceInterface::AddIgnore, "Checks the database to see if the character name exists and then adds to the ignore list")
        ;
}
