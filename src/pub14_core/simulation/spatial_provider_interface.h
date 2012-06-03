// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_SPATIAL_PROVIDER_H_
#define SWGANH_SIMULATION_SPATIAL_PROVIDER_H_

#include <vector>
#include <memory>

#include "anh/service/service_interface.h"

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <glm/glm.hpp>

namespace anh {
namespace app {
	class KernelInterface;
}} // namespace anh::service

namespace swganh {
namespace object {
	class Object;
}} // namespace swganh::object

namespace swganh {
namespace simulation {

/**
 * Provides Spatial Indexing functionality.
 */
class SpatialProviderInterface : public anh::service::ServiceInterface
{
public:
	SpatialProviderInterface(anh::app::KernelInterface* kernel) { };
	virtual ~SpatialProviderInterface() { };

	virtual void AddObject(std::shared_ptr<swganh::object::Object> obj) = 0;
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> obj) = 0;
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj, glm::vec3 old_position, glm::vec3 new_position) = 0;

	virtual std::vector<std::shared_ptr<swganh::object::Object>> GetObjectsInRange(glm::vec3 point, float range) = 0;
};

}} // namespace swganh::simulation

#endif // SWGANH_SIMULATION_SPATIAL_INDEX_PROVIDER_H_