
#ifndef SWGANH_SCENE_SCENE_H_
#define SWGANH_SCENE_SCENE_H_

namespace swganh {
namespace scene {
    
class Scene 
{
public:
    bool HasObservers(uint64_t object_id)
    {
        return false;
    }      
    
    template<typename T>
    void UpdateObservers(uint64_t object_id, T message)
    {
            
    }

    uint32_t GetId() { return id_; }
private:
    uint32_t id_;
};
    
}}  // swganh::scene

#endif  // SWGANH_SCENE_SCENE_H_