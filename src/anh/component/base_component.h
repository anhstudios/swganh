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

#include <map>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/component/component_interface.h>

namespace anh {
namespace component {

/**
 * \brief A basic object component.
 *
 * This class implements the Object Id handling interface functions.
 *
 * This class also does a basic message handling implementation of the HandleMessage function
 * which uses the EventDispatcher. Components deriving from this class need only to call the
 * RegisterMessageHandler and UnregisterMessageHandler methods to bind specific incoming Message Types
 * to a member function of the component.
 */
class BaseComponent : public ComponentInterface
{
public:
	/**
	 * \brief Default constructor.
	 */
	BaseComponent(ObjectId id);
	
	/**
	 * \brief Default deconstructor.
	 */
	~BaseComponent();

	/**
	 * \brief Initializes the component.
	 *
	 * \param pt Template properties passed by the ObjectBuilder.
	 */
	virtual void Init(boost::property_tree::ptree& pt) { };

	/**
	 * \brief Releases any resources the component holds.
	 */
	virtual void Deinit(void) { };

	/**
	 * \brief Called every tick if the option is enabled in the ComponentType.
	 * \see ComponentType
	 */
	virtual void Update(const float timeout) { };

	/**
	 * \breif Called to handle a message passed to this component by the Object Manager.
	 * Messages are derived from the event dispatchers IEvent.
	 * \see IEvent
	 *
	 * \param Message The message being passed to this component.
	 */
	virtual void HandleMessage(const Message message);

	/**
	 * \returns The type of component this is in the form of a hashed string.
	 * \see HashString
	 */
	virtual const ComponentInfo& component_info(void) = 0;

	/**
	 * \returns The Object Id of the entity that owns this component.
	 */
	const ObjectId& object_id(void);

protected:

	typedef	std::function<bool(const Message)>	MessageHandler;

	/**
	 * \brief Registers a handler function for a specific Message Type.
	 *
	 * Any time a message is passed to this component with the corresponding MessageType,
	 * the bound Handler function will be called to process the message.
	 *
	 * \param type The type of message to bind the handler to.
	 * \param handler The function which will handle the incoming message.
	 */
	void RegisterMessageHandler(const MessageType& type, MessageHandler handler);

	/**
	 * \brief Unregisters a handler function for a specific Message Type.
	 *
	 * \param type The type of message to clear the bindings for.
	 */
	void UnregisterMessageHandler(const MessageType& type);

private:
	
	ObjectId									object_id_;
	anh::event_dispatcher::EventDispatcher		event_dispatcher_;
};

} // namespace anh
} // namespace component

#endif // LIBANH_COMPONENT_BASE_COMPONENT_H_