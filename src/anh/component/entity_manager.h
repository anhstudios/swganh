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

#ifndef LIBANH_COMPONENT_ENTITY_MANAGER_H_
#define LIBANH_COMPONENT_ENTITY_MANAGER_H_

#include <map>
#include <anh/component/entity.h>

namespace anh {
namespace component {

/**
 * @brief Proxys function calls to entitys by id.
 */
class EntityManager : public boost::noncopyable
{
public:

	/**
	 * @brief Default constructor.
	 */
	EntityManager();
	
	/**
	 * @brief Default destructor.
	 */
	~EntityManager();

	/**
	 * @returns Nullptr if the entity is not found, or a pointer to the entity.
	 */
	std::shared_ptr<Entity>	GetEntity(const EntityId& id);


	void AddTag(const EntityId& id, const Tag& tag);
	void RemoveTag(const EntityId& id, const Tag& tag);
	bool HasTag(const EntityId& id, const Tag& tag);


	bool HasInterface(const EntityId& id, const ComponentType& type);
	template<class T> std::shared_ptr<T> QueryInterface(const EntityId& id, const InterfaceType& type);
	void AttachComponent(const EntityId& entity_id, std::shared_ptr<ComponentInterface> component);
	void DetachComponent(const EntityId& entity_id, const ComponentType& type);

	/**
	 * @brief Adds an entity to the manager.
	 *
	 * @param entity The entity you wish to be managed.
	 */
	bool AddEntity(std::shared_ptr<Entity> entity);

	/**
	 * @brief Removed an entity from the manager.
	 *
	 * @param id The id of the entity you want to be removed.
	 */
	void RemoveEntity(const EntityId& id);

	/**
	 * @brief Removes all entities from the manager.
	 */
	void RemoveAllEntities(void);

	/**
	 * @brief Broadcasts a message to an object's components.
	 *
	 * @param id Id of the object which the message should be broadcasted to.
	 * @param message The message to be sent to each of the objects components.
	 */
	void BroadcastMessage(const EntityId& entity_id, const Message message);

	/**
	 * @brief Updates each entity
	 *
	 * @param deltasMilliseconds The number of milliseconds between the last update.
	 */
	void Update(const float deltaMilliseconds);

protected:
private:
	typedef	std::map<EntityId, std::shared_ptr<Entity>>				EntityMap;
	typedef std::map<EntityId, std::shared_ptr<Entity>>::iterator	EntityMapIterator;

	EntityMap entities_;

};

#include <anh/component/entity_manager-inl.h>

} // namespace component
} // namespace anh

#endif // LIBANH_COMPONENT_ENTITY_MANAGER_H_