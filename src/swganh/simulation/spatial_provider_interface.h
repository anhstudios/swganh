// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <list>
#include <memory>
#include <glm/glm.hpp>

#include "swganh/object/container_interface.h"

namespace swganh{
namespace object {
	class Object;
}}


namespace swganh {
namespace simulation {

/**
 * Provides Spatial Indexing functionality.
 */
class SpatialProviderInterface : public swganh::object::ContainerInterface
{
public:
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position) = 0;
};

}} // namespace swganh::simulation
