
#ifndef SWGANH_SIMULATION_GALAXY_SERVICE_H_
#define SWGANH_SIMULATION_GALAXY_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/base/base_service.h"

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {

    class GalaxyService : public swganh::base::BaseService
    {
    public:
        explicit GalaxyService(std::shared_ptr<anh::app::KernelInterface> kernel);
    
        ~GalaxyService();

        anh::service::ServiceDescription GetServiceDescription();

        uint32_t StartScene(const std::string& scene_label);
        void StopScene(const std::string& scene_label);
        void StopScene(uint32_t scene_id);

        const std::shared_ptr<swganh::object::Object>& GetObjectById(uint64_t object_id);

        template<typename T>
        const std::shared_ptr<T>& GetObjectById(uint64_t object_id)
        {
            std::shared_ptr<Object> object = GetObjectById(object_id);

#if _DEBUG
            return std::dynamic_pointer_cast<T>(object);
#else
            return std::static_pointer_cast<T>(object);
#endif
        }

        void DestroyObjectById(uint64_t object_id);

    private:
        
        class GalaxyServiceImpl;
        std::unique_ptr<GalaxyServiceImpl> impl_;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_GALAXY_SERVICE_H_
