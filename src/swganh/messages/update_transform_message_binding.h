// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/messages/update_transform_message.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::messages;

struct UpdateTransformMessageWrapper : UpdateTransformMessage, wrapper<UpdateTransformMessage>
{

};

void exportUpdateTransformMessage()
{
    class_<UpdateTransformMessageWrapper, boost::noncopyable>("UpdateTransformMessage");
}
