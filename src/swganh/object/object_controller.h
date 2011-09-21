
#ifndef SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
#define SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_

#include <exception>
#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "anh/observer/observer_interface.h"
#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace network {
    class RemoteClient;
}}

namespace swganh {
namespace object {

    class Object;

    typedef std::function<
        void (const std::shared_ptr<Object>&, 
        const swganh::messages::ObjControllerMessage&)
    > ObjControllerHandler;

    typedef std::unordered_map<
        uint32_t, 
        ObjControllerHandler
    > ObjControllerHandlerMap;

    class ObjectController : public anh::observer::ObserverInterface, std::enable_shared_from_this<ObjectController>, boost::noncopyable
    {
    public:
        typedef std::runtime_error InvalidControllerMessage;

    public:
        ObjectController(
            std::shared_ptr<ObjControllerHandlerMap> handlers,
            std::shared_ptr<Object> object,
            std::shared_ptr<swganh::network::RemoteClient> client);

        ~ObjectController();

        /**
         * @return Handle to the object this controller manages.
         */
        const std::shared_ptr<Object>& GetObject() const;

        /**
         * @return Handle to the remote client controlling the object.
         */
        const std::shared_ptr<swganh::network::RemoteClient>& GetRemoteClient() const;

        /**
         * Handles an incoming controller message from the remote client.
         *
         * @param message The message recieved from the remote client.
         */
        void HandleControllerMessage(const swganh::messages::ObjControllerMessage& message);

        using anh::observer::ObserverInterface::Notify;

        /**
         * Notifies the controller when the object has been updated.
         *
         * @param message The message to be delivered to the remote client.
         */
        void Notify(const anh::ByteBuffer& message);

    private:
        ObjectController();

        std::shared_ptr<ObjControllerHandlerMap> handlers_;
        std::shared_ptr<Object> object_;
        std::shared_ptr<swganh::network::RemoteClient> client_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
