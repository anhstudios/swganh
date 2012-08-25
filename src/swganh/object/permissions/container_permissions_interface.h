#pragma once

#include "permission_type.h"
#include <memory>

namespace swganh
{
namespace object
{

	class Object;
	class ContainerInterface;

	class ContainerPermissionsInterface
	{
	public:

		virtual PermissionType GetType() = 0;

		virtual bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object) = 0;

		virtual bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object) = 0;

		virtual bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester) = 0;

	};

	class DefaultContainerPermissions : public ContainerPermissionsInterface
	{
	public:

		virtual PermissionType GetType()
		{
			return DEFAULT_CONTAINER_PERMISSION;
		}

		bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return false;
		}

		bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return false;
		}

		bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
		{
			return false;
		}
	};

	class WorldContainerPermissions : public ContainerPermissionsInterface
	{
		virtual PermissionType GetType()
		{
			return WORLD_CONTAINER_PERMISSION;
		}

		bool canInsert(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return true;
		}

		bool canRemove(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester, std::shared_ptr<Object> object)
		{
			return true;
		}

		bool canView(std::shared_ptr<ContainerInterface> container, std::shared_ptr<Object> requester)
		{
			return true;
		}
	};
}
}
