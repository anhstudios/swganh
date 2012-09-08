// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_attribute_template.h"

#include <sstream>

#include "swganh/event_dispatcher.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

BaseAttributeTemplate::BaseAttributeTemplate(swganh::EventDispatcher* dispatcher)
	: swganh::attributes::AttributeTemplateInterface(dispatcher)
{

}
void BaseAttributeTemplate::RegisterEventHandlers()
{
	event_dispatcher_->Subscribe("Object::UpdateAttribute", [this](const shared_ptr<EventInterface>& incoming_event)
	{
		const auto& object = static_pointer_cast<ObjectEvent>(incoming_event)->Get();
		
		// Clear out attributes if they have been updated
		if (HasAttributeListMessage(object->GetObjectId()))
			object_attribute_list_messages_[object->GetObjectId()].attributes.clear();
	});
}
swganh::messages::AttributeListMessage BaseAttributeTemplate::BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object)
{
	return swganh::messages::AttributeListMessage();
}
bool BaseAttributeTemplate::HasAttributeListMessage(uint64_t object_id)
{	
	bool hasMessage = false;
	auto found = find_if(object_attribute_list_messages_.begin(), object_attribute_list_messages_.end(),[object_id](ObjectMapAttributeListMessages::value_type entry)
	{
		return entry.first == object_id;
	});
	if (found != object_attribute_list_messages_.end())
	{
		if (found->second.attributes.size() > 0)
		{
			hasMessage = true;
		}
	}
	return hasMessage;
}

