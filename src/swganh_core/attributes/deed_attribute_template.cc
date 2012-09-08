// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "deed_attribute_template.h"

#include <sstream>

#include "swganh/event_dispatcher.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

DeedAttributeTemplate::DeedAttributeTemplate(swganh::EventDispatcher* dispatcher)
	: BaseAttributeTemplate(dispatcher)
{
	RegisterEventHandlers();
}

swganh::messages::AttributeListMessage DeedAttributeTemplate::BuildAttributeTemplate(shared_ptr<Object> object)
{
	uint64_t object_id = object->GetObjectId();
	if (HasAttributeListMessage(object_id))
		return object_attribute_list_messages_[object_id];

	swganh::messages::AttributeListMessage attribute_list_message;
	
	std::vector<Attribute> attribue_list;
	attribute_list_message.object_id = object->GetObjectId();

	attribue_list.push_back(Attribute("@obj_attr_n:original_name", object->GetAttributeAsString("original_name")));
	attribue_list.push_back(Attribute("@obj_attr_n:volume", object->GetAttributeAsString("volume")));
	attribue_list.push_back(Attribute("@obj_attr_n:crafter", object->GetAttributeAsString("crafter")));
	attribue_list.push_back(Attribute("@obj_attr_n:serial_number", object->GetAttributeAsString("serial_number")));
    // TODO: Break apart scenes and add one for each.
    attribue_list.push_back(Attribute("@obj_attr_n:examine_scene", object->GetAttributeAsString("examine_scene")));
    
	attribute_list_message.attributes = move(attribue_list);

	object_attribute_list_messages_[object_id] = move(attribute_list_message);
	return attribute_list_message;
}

