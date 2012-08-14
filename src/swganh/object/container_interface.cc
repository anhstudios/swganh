#include "container_interface.h"

#include "swganh/object/permissions/container_permissions_interface.h"

using namespace swganh::object;

std::shared_ptr<ContainerPermissionsInterface> ContainerInterface::GetPermissions() 
{ 
	return container_permissions_; 
}

void ContainerInterface::SetPermissions(std::shared_ptr<ContainerPermissionsInterface> obj) { 
	container_permissions_ = obj; 
}