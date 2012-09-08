// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/simulation/movement_manager_interface.h"

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <tbb/concurrent_unordered_map.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
}

#endif

namespace swganh {
namespace app {
	class SwganhKernel;
} // app
namespace simulation {
	class SpatialProviderInterface;
	class SimulationServiceInterface;
}} // swganh::simulation

namespace swganh {
namespace simulation {
	class MovementManager : public swganh::simulation::MovementManagerInterface
    {
    public:
		explicit MovementManager(swganh::app::SwganhKernel* kernel);

        void HandleDataTransform(
            const std::shared_ptr<swganh::object::Object>& object, 
            swganh::messages::controllers::DataTransform message);

        void HandleDataTransformWithParent(
            const std::shared_ptr<swganh::object::Object>& object, 
            swganh::messages::controllers::DataTransformWithParent message);
        
        void SendDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B);
        void SendUpdateDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object);
        
        void SendDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B);
        void SendUpdateDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object);

		void SetSpatialProvider(std::shared_ptr<swganh::simulation::SpatialProviderInterface> spatial_provider);

    private:
        void RegisterEvents(swganh::EventDispatcher* event_dispatcher);

        bool ValidateCounter_(uint64_t object_id, uint32_t counter);

        typedef Concurrency::concurrent_unordered_map<
            uint64_t, uint32_t
        > UpdateCounterMap;

        UpdateCounterMap counter_map_;
		std::shared_ptr<swganh::simulation::SpatialProviderInterface> spatial_provider_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::simulation
