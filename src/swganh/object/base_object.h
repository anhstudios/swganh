
#ifndef SWGANH_OBJECT_BASE_OBJECT_H_
#define SWGANH_OBJECT_BASE_OBJECT_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "swganh/scene/messages/baselines_message.h"
#include "swganh/scene/messages/deltas_message.h"


namespace swganh {
namespace scene {
class Scene;  
}}  // swganh::scene

namespace swganh {
namespace object {
    
typedef std::vector<
    std::pair<uint8_t, swganh::scene::messages::BaselinesMessage>
> BaselinesCacheContainer;

typedef std::vector<
    std::pair<uint8_t, swganh::scene::messages::DeltasMessage>
> DeltasCacheContainer;

class BaseObject
{
public:
    enum ViewType
    {
        VIEW_1 = 1,
        VIEW_2,
        VIEW_3,
        VIEW_4,
        VIEW_5,
        VIEW_6,
        VIEW_7,
        VIEW_8,
        VIEW_9
    };
    
public:
    
    virtual ~BaseObject() {}
    
    uint64_t GetObjectId() const;

    std::wstring GetCustomName() const;
    
    void SetCustomName(std::wstring custom_name);
    
    std::shared_ptr<swganh::scene::Scene> GetScene();

    const std::string& GetTemplate() { return template_string_; }
    glm::vec3 GetPosition() { return position_; }
    glm::quat GetOrientation() { return orientation_; }
    uint64_t GetContainer() { return container_id_; }
    float GetComplexity() { return complexity_; }
    const std::string& GetStfNameFile() { return stf_name_file_; }
    const std::string& GetStfNameString() { return stf_name_string_; }
    const std::wstring& GetCustomName() { return custom_name_; }
    uint32_t GetVolume() { return volume_; }
    
    /**
     * Returns the baselines created in the last reliable update. If
     * no baselines exist yet for the object a reliable update will be
     * triggered and the results of that returned.
     */
    BaselinesCacheContainer GetBaselines();
        
    /**
     * @return The deltas created since the last reliable update.
     */
    DeltasCacheContainer GetDeltas();
    
    void ReliableUpdate();
    
protected:
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline1() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline2() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline4() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline5() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline6();
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline7() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline8() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::scene::messages::BaselinesMessage> GetBaseline9() { return boost::optional<swganh::scene::messages::BaselinesMessage>(); }
    
    virtual uint32_t GetType() = 0;
        
    swganh::scene::messages::BaselinesMessage CreateBaselinesMessage(uint16_t view_type, uint16_t opcount = 0);
    
    swganh::scene::messages::DeltasMessage CreateDeltasMessage(uint16_t view_type);
    
    BaselinesCacheContainer baselines_cache_;

    DeltasCacheContainer deltas_cache_;

private:
    uint64_t object_id_;             // create
    std::string template_string_;    // create
    glm::vec3 position_;             // create
    glm::quat orientation_;          // create
    uint64_t container_id_;          // containment
    float complexity_;               // update 3
    std::string stf_name_file_;      // update 3
    std::string stf_name_string_;    // update 3
    std::wstring custom_name_;       // update 3
    uint32_t volume_;                // update 3
    
    std::shared_ptr<swganh::scene::Scene> scene_;
        
};

}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
