#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_

#include "anh/python_shared_ptr.h"
#include "social_service.h"

#include <boost/python.hpp>

using namespace swganh::social;
using namespace boost::python;
using namespace std;

void exportSocialService()
{
    class_<SocialService, shared_ptr<SocialService>, boost::noncopyable>("SocialService", "The social service handles services that involve social actions", no_init)
        .def("add_friend", &SocialService::AddFriend, "Checks the database to see if the character name exists and then adds the friend to the player")
        ;
}

#endif //SWGANH_SOCIAL_SOCIAL_SERVICE_BINDING_H_