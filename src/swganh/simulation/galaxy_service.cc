
#include "swganh/simulation/galaxy_service.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_manager.h"

#include "swganh/simulation/scene_manager.h"

#include "swganh/app/swganh_kernel.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::simulation;

using anh::app::KernelInterface;
using anh::service::ServiceDescription;
using swganh::base::BaseService;


class GalaxyService::GalaxyServiceImpl {
public:
    const shared_ptr<ObjectManager>& GetObjectManager()
    {
        if (!object_manager_)
        {
            object_manager_ = make_shared<ObjectManager>();
        }

        return object_manager_;
    }

    const shared_ptr<SceneManager>& GetSceneManager()
    {
        if (!scene_manager_)
        {
            scene_manager_ = make_shared<SceneManager>();
        }

        return scene_manager_;
    }

    const shared_ptr<Object>& GetObjectById(uint64_t object_id)
    {
        auto find_iter = loaded_objects_.find(object_id);

        if (find_iter == loaded_objects_.end())
        {
            throw swganh::object::InvalidObject("Requested an invalid object");
        }

        return find_iter->second;
    }

private:
    shared_ptr<ObjectManager> object_manager_;
    shared_ptr<SceneManager> scene_manager_;

    map<uint64_t, shared_ptr<Object>> loaded_objects_;
};

GalaxyService::GalaxyService(shared_ptr<KernelInterface> kernel)
    : BaseService(kernel) 
    , impl_(new GalaxyServiceImpl)
{}
    
GalaxyService::~GalaxyService()
{}

ServiceDescription GalaxyService::GetServiceDescription()
{
    ServiceDescription service_description(
        "GalaxyService",
        "galaxy",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

uint32_t GalaxyService::StartScene(const std::string& scene_label)
{
    return 0;
}

void GalaxyService::StopScene(const std::string& scene_label)
{}

void GalaxyService::StopScene(uint32_t scene_id)
{}

const shared_ptr<Object>& GalaxyService::GetObjectById(uint64_t object_id)
{
    return impl_->GetObjectById(object_id);
}

void GalaxyService::DestroyObjectById(uint64_t object_id)
{

}
