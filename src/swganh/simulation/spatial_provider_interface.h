// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <list>
#include <memory>
#include <glm/glm.hpp>

#include "swganh/object/container_interface.h"
#include "swganh/object/object.h"


namespace swganh {
namespace simulation {

/**
 * Provides Spatial Indexing functionality.
 */
class SpatialProviderInterface : public swganh::object::ContainerInterface
{
public:
	virtual void SvgToFile()=0;
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, const swganh::object::BoundingVolume& old_bounding_volume, const swganh::object::BoundingVolume& new_bounding_volume) = 0;
};

}} // namespace swganh::simulation
