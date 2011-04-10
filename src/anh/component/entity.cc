/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

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

#include <anh/component/entity.h>

namespace anh {
namespace component {

Entity::Entity(const ObjectId& id, const std::string name, const TagSet& tags, const ObjectId& parent_id)
	: id_(id)
	, name_(name)
	, tags_(tags)
	, parent_id_(parent_id)
{
}

Entity::~Entity()
{
	DetachAllComponents();
}

void Entity::AttachComponent(std::shared_ptr<ComponentInterface> component)
{
	ComponentsMapIterator i = components_.find(component->component_info().type);
	if(i == components_.end())
		components_.insert(ComponentsMap::value_type(component->component_info().type, component));
}

void Entity::DetachComponent(const ComponentType& type)
{
	ComponentsMapIterator i = components_.find(type);
	if(i != components_.end())
	{
		i->second->Deinit();
		components_.erase(i);
	}
}

void Entity::DetachAllComponents(void)
{
	std::for_each(components_.begin(), components_.end(), [=](ComponentsMap::value_type& pair) {
		pair.second->Deinit();
	});

	components_.clear();
}

bool Entity::hasComponent(const ComponentType& type)
{
	ComponentsMapIterator i = components_.find(type);
	if(i != components_.end())
		return true;
	else
		return false;
}

void Entity::addTag(const Tag& tag)
{
	tags_.insert(tag);
}

void Entity::removeTag(const Tag& tag)
{
	TagSetIterator i = tags_.find(tag);
	if(i != tags_.end())
		tags_.erase(i);
}

bool Entity::hasTag(const Tag& tag)
{
	TagSetIterator i = tags_.find(tag);
	if(i != tags_.end())
		return true;
	else
		return false;
}

void Entity::Update(uint64_t ms_tick)
{
	std::for_each(components_.begin(), components_.end(), [=](ComponentsMap::value_type& pair) {
		pair.second->Update(ms_tick);
	});
}

void Entity::BroadcastMessage(Message message)
{
	std::for_each(components_.begin(), components_.end(), [=](ComponentsMap::value_type& pair) {
		pair.second->HandleMessage(message);
	});
}

}
}