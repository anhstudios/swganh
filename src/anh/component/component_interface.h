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

#ifndef LIBANH_COMPONENT_COMPONENT_INTERFACE_H_
#define LIBANH_COMPONENT_COMPONENT_INTERFACE_H_

#include <boost/property_tree/ptree.hpp>
#include <anh/event_dispatcher/basic_event.h>
#include <anh/component/attribute_mapper_interface.h>

namespace anh {
namespace component {

typedef	unsigned long long										        EntityId;
typedef std::shared_ptr<anh::event_dispatcher::EventInterface>			Message;
typedef anh::event_dispatcher::EventInterface							IMessage;
typedef anh::event_dispatcher::EventType						        MessageType;
typedef anh::event_dispatcher::SimpleEvent						        SimpleMessage;
typedef std::shared_ptr<anh::event_dispatcher::EventInterface>			MessagePtr;
typedef anh::HashString													ComponentType;
typedef anh::HashString													InterfaceType;

class EntityBuilder;
class Entity;

/**
 * @brief
 */
class ComponentInterface
{
public:
    friend class EntityBuilder;
    friend class Entity;
	/**
	 * @brief Default constructor.
	 */
	ComponentInterface() { }

	/**
	 * @brief Default destructor.
	 */
	virtual ~ComponentInterface() { }

	/**
	 * @brief Initializes the component.
	 *
	 * @param pt Template properties passed in by the EntityBuilder.
	 */
	virtual void Init(boost::property_tree::ptree& pt) = 0;

	/**
	 * @brief Called when the component gets attached to an Entity.
	 */
	virtual void OnAttach(void) = 0;

	/**
	 * @brief Called when the component gets detached from an Entity.
	 */
	virtual void OnDetach(void) = 0;

	/**
	 * @brief Called when the Update function is invoked on an Entity.
	 */
	virtual void Update(const float deltaMilliseconds) = 0;

	/**
	 * @breif Handles an incoming message from an out-side source.
	 * @see IEvent
	 *
	 * @param Message The message being passed to this component.
	 */
	virtual void HandleMessage(const Message message) = 0;

	/**
	 * @returns The type of component this is.
	 */
	virtual const ComponentType& component_type(void) = 0;

	/**
	 * @returns The interface type this component impliments.
	 */
	virtual const InterfaceType& interface_type(void) = 0;

	/**
	 * @returns The Object Id of the entity that owns this component.
	 */
	virtual const EntityId& entity_id(void) const = 0;

	/**
	 * @brief Sets the id of the entity that owns this component.
	 * 
	 * Use with Caution! Only "the wise ones" are allowed to use this function.
	 */
	virtual void set_entity_id(const EntityId& id) = 0;
    /**
    * @brief sets the dirty flag of the component to signal the db update
    *
    * @param dirty 
    */
    virtual void set_dirty(bool dirty = true) = 0;
    /**
    * @brief gets the dirty flag value from the component
    */
    virtual bool dirty()= 0;
private:
    /**
    * @brief Gets the attribute mapper associated with the component.
    */
    virtual std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>> attribute_mapper() = 0;
    /**
    *  @brief Assigns an attribute mapper to the component.
    */
    virtual void set_attribute_mapper(std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>> attribute_mapper) = 0;
};

}  // namespace component
}  // namespace anh

#endif  // LIBANH_COMPONENT_COMPONENT_INTERFACE_H_
