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

#ifndef ANH_COMPONENT_ENTITY_H_
#define ANH_COMPONENT_ENTITY_H_

#include <map>
#include <set>
#include <anh/component/component_interface.h>

namespace anh {
namespace component {

typedef anh::HashString Tag;
typedef std::set<Tag>				TagSet;
typedef std::set<Tag>::iterator		TagSetIterator;

/**
 * A collection of components which represent a "seperate existance" or object. This class also
 * contains basic typing (tags) and identification (entity id) attributes.
 */
class Entity : public boost::noncopyable
{
public:

	/**
	 * @brief Default constructor.
	 */
	Entity(const EntityId& id = 0, const std::string name = "", const TagSet& tags = TagSet());

	/**
	 * @brief Default destructor.
	 */
	~Entity();

	void AttachComponent(std::shared_ptr<ComponentInterface> component);
	void DetachComponent(const InterfaceType& type);
	void DetachAllComponents(void);
	bool HasInterface(const InterfaceType& type);

	/**
	 * @brief Queries the entity for a component, if the component does not exist,
	 * a "NulLComponent" will be returned.
	 *
	 * @param type The type of component interface you are requesting.
	 * @returns T The found component or its null instance.
	 */
	template<class T> std::shared_ptr<T> QueryInterface(const InterfaceType& type);

	void AddTag(const Tag& tag);
	void RemoveTag(const Tag& tag);
	bool HasTag(const Tag& tag);

	/**
	 * Updates each component that is attached to the entity.
	 */
	void Update(const float deltaMilliseconds);

	/**
	 * Sends a message to each of the entities components.
	 */
	void BroadcastMessage(Message message);

	const EntityId& id() const { return id_; }
	const std::string& name() const { return name_; }

private:
	typedef std::map<InterfaceType, std::shared_ptr<ComponentInterface>>			ComponentsMap;
	typedef std::map<InterfaceType, std::shared_ptr<ComponentInterface>>::iterator	ComponentsMapIterator;

	std::string							name_;
	EntityId							id_;
	TagSet								tags_;
	ComponentsMap						components_;

};

#include "entity-inl.h"

}
}

#endif