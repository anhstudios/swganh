
#ifndef SWGANH_OBJECT_BASE_OBJECT_H_
#define SWGANH_OBJECT_BASE_OBJECT_H_

namespace swganh {
namespace object {
    
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
    
    std::wstring GetCustomName() const
    {
        return custom_name_;
    }
    
    void SetCustomName(std::wstring custom_name)
    {
        custom_name_ = custom_name;
        
        // Only build a message if there are observers.
        if (scene->HasObservers(object_id))
        {
            DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
            message.data.write<uint16_t>(2); // update type
            message.data.write(custom_name);
        
            scene->UpdateObservers(object_id, message);
            delta_cache_.push_back(make_pair(BaseObject::VIEW_3, move(message)));            
        }
    }
    
    /**
     * Returns the baselines created in the last reliable update. If
     * no baselines exist yet for the object a reliable update will be
     * triggered and the results of that returned.
     */
    BaselineCacheContainer GetBaselines()
    {
        if (baselines_cache_.empty())
        {
            ReliableUpdate();
        }
        
        return baselines_cache_;
    }
        
    /**
     * @return The deltas created since the last reliable update.
     */
    DeltasCacheContainer GetDeltas()
    {
        if (!deltas_cache_.empty())
        {
            return deltas_cache_;
        }
        
        return DeltasCacheContainer();
    }
    
    BaselinesMessage CreateBaselinesMessage(uint16_t update_type)
    {
        BaselinesMessage message;
        message.object_id = GetObjectId();
        message.object_type = GetType();
        message.update_type = update_type;
    }
    
    DeltasMessage CreateDeltasMessage(uint16_t update_type)
    {        
        DeltasMessage message;
        message.object_id = GetObjectId();
        message.object_type = GetType();
        message.update_type = update_type;
    }
    
    void ReliableUpdate()
    {
        baselines_cache_.clear();
        deltas_cache_.clear();
        
        OnReliableUpdate();
    }
    
protected:
    virtual void OnReliableUpdate() = 0;
    
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
    uint32_t scene_id_;              // update 6
    
    typedef std::vector<
        std::pair<uint8_t, BaselinesMessage>
    > BaselineCacheContainer;
    
    BaselineCacheContainer baselines_cache_;
    
    typedef std::vector<
        std::pair<uint8_t, DeltasMessage>
    > DeltaCacheContainer;

    DeltaCacheContainer deltas_cache_;
};

}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
