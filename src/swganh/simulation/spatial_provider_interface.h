// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SPATIAL_PROVIDER_H_
#define SWGANH_SIMULATION_SPATIAL_PROVIDER_H_

#include <memory>
#include <glm/glm.hpp>


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
		
	virtual void SpatialProviderInterface::UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position) = 0;
};

}} // namespace swganh::simulation

#endif // SWGANH_SIMULATION_SPATIAL_INDEX_PROVIDER_H_