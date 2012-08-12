#ifndef SWGANH_CONTAINER_PERMISSIONS_INTERFACE_H_
#define SWGANH_CONTAINER_PERMISSIONS_INTERFACE_H_

#include <memory>

namespace swganh
{
namespace object
{

	class Object;

	class ContainerPermissionsInterface
	{
	public:

		virtual bool canInsert(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object) = 0;

		virtual bool canRemove(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object) = 0;

		virtual bool canView(std::shared_ptr<Object> container, std::shared_ptr<Object> requester) = 0;

	};

	class DefaultContainerPermissions : public ContainerPermissionsInterface
	{
	public:
		bool canInsert(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return false;
		}

		bool canRemove(std::shared_ptr<Object> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return false;
		}

		bool canView(std::shared_ptr<Object> container, std::shared_ptr<Object> requester)
		{
			return false;
		}
	};

}
}

#endif