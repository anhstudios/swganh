
#ifndef SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
#define SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_

#include <exception>
#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include <tbb/concurrent_unordered_map.h>

#include "anh/observer/observer_interface.h"
#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace network {
    class RemoteClient;
}}

namespace swganh {
namespace object {

    class Object;
    class ObjectController;

    typedef std::function<
        void (const std::shared_ptr<ObjectController>&, 
        const swganh::messages::ObjControllerMessage&)
    > ObjControllerHandler;

    typedef tbb::concurrent_unordered_map<
        uint32_t, 
        ObjControllerHandler
    > ObjControllerHandlerMap;

    class ObjectController : public anh::observer::ObserverInterface, public std::enable_shared_from_this<ObjectController>, boost::noncopyable
    {
    public:
        typedef std::runtime_error InvalidControllerMessage;

    public:
        ObjectController(
            std::shared_ptr<Object> object,
            std::shared_ptr<swganh::network::RemoteClient> client);

        ~ObjectController();

        uint64_t GetId() const;

        /**
         * @return Handle to the object this controller manages.
         */
        std::shared_ptr<Object> GetObject() const;

        /**
         * @return Handle to the remote client controlling the object.
         */
        std::shared_ptr<swganh::network::RemoteClient> GetRemoteClient();
        
        /**
         * Sets handle to the remote client controlling the object.
         *
         * @param remote_client the new remote client for this controller.
         */
        void SetRemoteClient(std::shared_ptr<swganh::network::RemoteClient> remote_client);

        using anh::observer::ObserverInterface::Notify;

        /**
         * Notifies the controller when the object has been updated.
         *
         * @param message The message to be delivered to the remote client.
         */
        void Notify(const anh::ByteBuffer& message);
        
        void Notify(const swganh::messages::ObjControllerMessage& message);

    private:
        ObjectController();

        std::shared_ptr<Object> object_;
        std::shared_ptr<swganh::network::RemoteClient> client_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_BASE_OBJECT_OBSERVER_H_
