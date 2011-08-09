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

#ifndef LIBANH_COMPONENT_BASE_COMPONENT_H_
#define LIBANH_COMPONENT_BASE_COMPONENT_H_

#include <functional>
#include <map>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/component/component_interface.h>

namespace anh {
namespace component {

/**
 * \brief A basic entity component.
 *
 * This class hosts a basic message handling implementation for use with the HandleMessage function. 
 * Components deriving from this class need only to call the RegisterMessageHandler and UnregisterMessageHandler 
 * methods to bind specific incoming Message Types to a member function of the component.
 */
class BaseComponent : public ComponentInterface
{
public:
    BaseComponent(const ComponentType& component_interface, const ComponentType& type);
    virtual ~BaseComponent();

    virtual void Init(boost::property_tree::ptree& pt) { };
    virtual void Deinit(void) { };

    virtual void OnAttach(void) { };
    virtual void OnDetach(void) { };
    virtual void Update(const float deltaMilliseconds) { };
    virtual void set_dirty(bool dirty = true) { dirty_ = dirty; }
    virtual bool dirty() { return dirty_; }
    virtual std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>> attribute_mapper() { return attribute_mapper_; }
    virtual void set_attribute_mapper(std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>> mapper) { attribute_mapper_ = mapper; }

    /**
     * @brief Called to handle a message passed to this component by the Object Manager.
     * Messages are derived from the event dispatchers IEvent.
     * @see IEvent
     *
     * @param Message The message being passed to this component.
     */
    virtual void HandleMessage(const Message message);

    virtual const ComponentType& component_type(void) { return type_; }
    virtual const ComponentType& interface_type(void) { return interface_; }
    const EntityId& entity_id(void) const { return entity_id_; }
    void set_entity_id(const EntityId& id) { entity_id_ = id; }

protected:
    typedef	std::function<bool(const Message)>	MessageHandler;

    /**
     * @brief Registers a handler function for a specific Message Type.
     *
     * Any time a message is passed to this component with the corresponding MessageType,
     * the bound Handler function will be called to process the message.
     *
     * @param type The type of message to bind the handler to.
     * @param handler The function which will handle the incoming message.
     */
    bool RegisterMessageHandler(const MessageType& type, MessageHandler handler);

    /**
     * @brief Unregisters a handler function for a specific Message Type.
     *
     * @param type The type of message to clear the bindings for.
     */
    void UnregisterMessageHandler(const MessageType& type);

private:
    
    EntityId									entity_id_;
    anh::event_dispatcher::EventDispatcher		event_dispatcher_;
    ComponentType								type_;
    ComponentType								interface_;
    std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>> attribute_mapper_;
    bool                                        dirty_;
};

} // namespace anh
} // namespace component

#endif // LIBANH_COMPONENT_BASE_COMPONENT_H_
