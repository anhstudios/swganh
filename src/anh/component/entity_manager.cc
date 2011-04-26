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
#include <anh/component/entity_manager.h>

namespace anh {
namespace component {

EntityManager::EntityManager()
{ 
}

EntityManager::~EntityManager()
{
	RemoveAllEntities();
}

std::shared_ptr<Entity> EntityManager::GetEntity(const EntityId& entity_id)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		return i->second;
	else
		return nullptr;

}

void EntityManager::RemoveAllEntities(void)
{
	entities_.clear();
}

void EntityManager::AddTag(const EntityId& entity_id, const Tag& tag)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		i->second->AddTag(tag);
}

void EntityManager::RemoveTag(const EntityId& entity_id, const Tag& tag)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		i->second->RemoveTag(tag);
}

bool EntityManager::HasTag(const EntityId& entity_id, const Tag& tag)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		return i->second->HasTag(tag);
	else
		return false;
}

bool EntityManager::HasInterface(const EntityId& entity_id, const ComponentType& type)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		return i->second->HasInterface(type);
	else
		return false;
}

void EntityManager::AttachComponent(const EntityId& entity_id, std::shared_ptr<ComponentInterface> component)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		i->second->AttachComponent(component);
}

void EntityManager::DetachComponent(const EntityId& entity_id, const ComponentType& type)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
		i->second->DetachComponent(type);
}

void EntityManager::BroadcastMessage(const EntityId& entity_id, const Message message)
{
	EntityMapIterator i = entities_.find(entity_id);
	if(i != entities_.end())
	{
		i->second->BroadcastMessage(message);
	}
}

void EntityManager::Update(const float deltaMilliseconds)
{
	std::for_each(entities_.begin(), entities_.end(), [=](EntityMap::value_type& i) {
		i.second->Update(deltaMilliseconds);
	});
}

bool EntityManager::AddEntity(std::shared_ptr<Entity> entity)
{
	EntityMapIterator i = entities_.find(entity->id());
	if(i == entities_.end())
	{
		entities_.insert(EntityMap::value_type(entity->id(), entity));
		return true;
	}
	else
	{
		return false;
	}
}
void EntityManager::RemoveEntity(const EntityId& object_id)
{
	EntityMapIterator i = entities_.find(object_id);
	if(i != entities_.end())
		entities_.erase(i);
}

}
}