/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <anh/component/object_manager.h>

namespace anh {
namespace component {

ObjectManager::ObjectManager()
{ }

ObjectManager::~ObjectManager()
{ }

void ObjectManager::AttachComponent(const ObjectId& id, std::shared_ptr<ComponentInterface> component)
{
    // See if the object already has attached components
    // if not, create a new object id entry and attach the
    // new component.
    ObjectComponentMapIterator i = object_components_map_.find(id);

    if(i == object_components_map_.end())
	{
		std::list<std::shared_ptr<ComponentInterface>> component_list;
		component_list.push_back(component);
		object_components_map_.insert(ObjectComponentMapPair(id, component_list));

		if(component->component_info().update_every_frame)
			update_components_.push_back(component);
	}
	else
	{
		// Iterate through each of the objects
		// components to make sure we dont have
		// a duplicate componented added.
		//
		// If there is a duplicate entry we will ignore
		// the addition.
		bool component_exists = false;
		std::for_each((*i).second.begin(), (*i).second.end(), [=, &component_exists](std::shared_ptr<ComponentInterface> comp) {
			if(comp->component_info().type == component->component_info().type)
			{
				component_exists = true;
				return;
			}
		});

		if(!component_exists) {
			(*i).second.push_back(component);

			if(component->component_info().update_every_frame)
				update_components_.push_back(component);
		}
	}
    
}

void ObjectManager::DetachComponent(const ObjectId& id, const ComponentType& type)
{
	// Find the objects components
	// if the object id does not exist
	// in the map, ignore the detach.
	ObjectComponentMapIterator i = object_components_map_.find(id);
	if(i != object_components_map_.end())
	{
		// Find the specific component and remove it.
		(*i).second.remove_if( [=](std::shared_ptr<ComponentInterface> comp) {
			return (comp->component_info().type == type);
		});

		// Lookup whether this component is in the update list.
		update_components_.remove_if( [=](std::shared_ptr<ComponentInterface> comp) {
			return ((comp->object_id()) == id && (comp->component_info().type == type));
		});
	}
}
void ObjectManager::DetatchAllComponents(const ObjectId& id)
{
    // loop through each match and remove it.
    ObjectComponentMapIterator i = object_components_map_.find(id);
	if(i != object_components_map_.end())
	{
        (*i).second.remove_if( [=](std::shared_ptr<ComponentInterface> comp) {
            return(comp->object_id() == id);
        });
    }
}

bool ObjectManager::HasInterface(const ObjectId& id, const ComponentType& type)
{
	bool result = false;
	// Find out objects components
	// if no object components are found
	// return false by default.
	ObjectComponentMapIterator i = object_components_map_.find(id);
    if(i != object_components_map_.end())
	{
		std::for_each((*i).second.begin(), (*i).second.end(), [=, &result](std::shared_ptr<ComponentInterface> comp) {
			if(comp->component_info().type == type)
			{
				result = true;
				return;
			}
		});
	}

	return result;
}

void ObjectManager::BroadcastMessage(const ObjectId& object_id, const Message message)
{
	// Find out objects components
	ObjectComponentMapIterator i = object_components_map_.find(object_id);
	if(i != object_components_map_.end())
	{
		std::for_each((*i).second.begin(), (*i).second.end(), [=](std::shared_ptr<ComponentInterface> comp) {
			comp->HandleMessage(message);
		});
	}
}

void ObjectManager::Tick(const float timeout)
{
	std::for_each(update_components_.begin(), update_components_.end(), [=](std::shared_ptr<ComponentInterface> comp) {
		comp->Update(timeout);
	});
}

}
}