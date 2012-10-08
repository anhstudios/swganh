// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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

}
}
