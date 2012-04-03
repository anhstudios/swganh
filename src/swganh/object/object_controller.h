
#ifndef SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
#define SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_

#include <exception>
#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <tbb/concurrent_unordered_map.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
}

#endif

#include "anh/observer/observer_interface.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/controllers/show_fly_text.h"

namespace swganh {
namespace connection {
    class ConnectionClient;
}}

namespace swganh {
namespace messages {
    class OutOfBand;
}}

namespace swganh {
namespace object {

    class Object;
    class ObjectController;

    typedef std::function<
        void (const std::shared_ptr<ObjectController>&, 
        swganh::messages::ObjControllerMessage)
    > ObjControllerHandler;

    typedef Concurrency::concurrent_unordered_map<
        uint32_t, 
        ObjControllerHandler
    > ObjControllerHandlerMap;

    class ObjectController : public anh::observer::ObserverInterface, public std::enable_shared_from_this<ObjectController>
    {
    public:
        typedef std::runtime_error InvalidControllerMessage;

    public:
        ObjectController(
            std::shared_ptr<Object> object,
            std::shared_ptr<swganh::connection::ConnectionClient> client);

        ~ObjectController();

        uint64_t GetId() const;

        /**
         * @return Handle to the object this controller manages.
         */
        std::shared_ptr<Object> GetObject() const;

        /**
         * @return Handle to the remote client controlling the object.
         */
        std::shared_ptr<swganh::connection::ConnectionClient> GetRemoteClient();
        
        /**
         * Sets handle to the remote client controlling the object.
         *
         * @param remote_client the new remote client for this controller.
         */
        void SetRemoteClient(std::shared_ptr<swganh::connection::ConnectionClient> remote_client);

        using anh::observer::ObserverInterface::Notify;

        /**
         * Notifies the controller when the object has been updated.
         *
         * @param message The message to be delivered to the remote client.
         */
        void Notify(const anh::ByteBuffer& message);

        // Send System Message

        /**
         * Used to send a specific system message to the player
         *
         * @param custom_message, the custom message to send
         * @param chatbox_only used to send to only the chatbox or to the screen as well
         * @param send_to_inrange used to determine to send to any players in range as well
         */
        bool SendSystemMessage(const std::string& custom_message) { return SendSystemMessage(std::wstring(custom_message.begin(), custom_message.end())); }
        bool SendSystemMessage(const std::wstring& custom_message, bool chatbox_only = false, bool send_to_inrange = false);

        bool SendSystemMessage(const swganh::messages::OutOfBand& prose, bool chatbox_only = false, bool send_to_inrange = false);

        /**
         * Used to send Fly Text to the character
         *
         * @param
         */
        void SendFlyText(const std::string& fly_text, swganh::messages::controllers::FlyTextColor color, bool display_flag, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);
        void SendFlyText(const std::string& fly_text, swganh::messages::controllers::FlyTextColor color);
        
    private:
        /**
         * Sends out a system message.
         *
         * This internal method is invoked by the two SendSystemMessage overloads to send out a system message.
         *
         * @param custom_message A custom text string to be sent.
         * @param prose A custom STF string package.
         * @param player The recepient of the system message. If no player is passed the message is sent to everyone.
         * @param chatbox_only Determines whether the message is displayed on screen or in the chatbox
         *                     only. By default this is false meaning messages are by default displayed on screen and the chatbox.
         * @param send_to_inrange If true the message is sent to all in-range players of the target recipient.
         */
        bool SendSystemMessage_(const std::wstring& custom_message, const swganh::messages::OutOfBand& prose, bool chatbox_only, bool send_to_inrange);


        ObjectController();

        std::shared_ptr<Object> object_;
        std::shared_ptr<swganh::connection::ConnectionClient> client_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
