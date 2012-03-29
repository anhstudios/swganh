// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/messages/update_containment_message.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::messages;

struct UpdateContainmentMessageWrapper : UpdateContainmentMessage, wrapper<UpdateContainmentMessage>
{

};

void exportUpdateContainmentMessage()
{
    class_<UpdateContainmentMessageWrapper, boost::noncopyable>("UpdateContainmentMessage");
}
