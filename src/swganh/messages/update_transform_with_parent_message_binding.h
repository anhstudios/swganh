// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/messages/update_transform_with_parent_message.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::messages;

struct UpdateTransformWithParentMessageWrapper : UpdateTransformWithParentMessage, wrapper<UpdateTransformWithParentMessage>
{

};
void exportUpdateTransformWithParentMessage()
{
	class_<UpdateTransformWithParentMessageWrapper, boost::noncopyable>("UpdateTransformWithParentMessage");
}

