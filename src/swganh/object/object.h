
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

#include "anh/observer/observable_interface.h"

#include "swganh/messages/baselines_message.h"
#include "swganh/messages/deltas_message.h"


namespace swganh {
namespace object {
                    
typedef std::vector<
    swganh::messages::BaselinesMessage
> BaselinesCacheContainer;

typedef std::vector<
    swganh::messages::DeltasMessage
> DeltasCacheContainer;

class Object
{
public:
    enum ViewType : uint16_t
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
    Object();

    virtual ~Object() {}
    
    /**
     * Returns whether or not this observable object has any observers.
     *
     * @return True if has observers, false if not.
     */
    bool HasObservers() const;

    /**
     * Start receiving notifications when the observable object changes state.
     *
     * @param observer The object interested in receiving state change notifications.
     */
    void Subscribe(const std::shared_ptr<anh::observer::ObserverInterface>& observer);

    /**
     * Stop receiving state notification changes for the observable object.
     *
     * @param observer The object that no longer wants state change notifications.
     */
    void Unsubscribe(const std::shared_ptr<anh::observer::ObserverInterface>& observer);

    /**
     * Notifies observers that the observable object has changed state.
     *
     * @param message Message containing the updated state of the observable object.
     */
    void NotifyObservers(const anh::ByteBuffer& message);
    
    /**
     * Notifies observers that the observable object has changed state.
     *
     * @param message Message containing the updated state of the observable object.
     */
    template<typename T>
    void NotifyObservers(const T& message)
    {
        anh::ByteBuffer buffer;
        message.serialize(buffer);

        NotifyObservers(buffer);
    }

    /**
     * Returns whether or not the object has been modified since the last reliable
     * update was sent out.
     *
     * @return Modified since last reliable update.
     */
    bool IsDirty() const;

    /**
     * Regenerates the baselines and updates observers.
     */
    void MakeClean();

    /**
     * Returns the most recently generated baselines.
     *
     * @param viewer_id The id of the object viewing this Object instance.
     * @return The most recently generated baselines.
     */
    const BaselinesCacheContainer& GetBaselines(uint64_t viewer_id) const;

    /**
     * Returns the deltas messages generated since the last time the 
     * object was made clean.
     *
     * @param viewer_id The id of the object viewing this Object instance.
     * @return The most recently generated deltas.
     */
    const DeltasCacheContainer& GetDeltas(uint64_t viewer_id) const;
    
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
     * Stores a deltas message update for the object.
     *
     * @param message The deltas message to store.
     */
    void AddDeltasUpdate(swganh::messages::DeltasMessage message);
    
    /**
     * @return The id of this Object instance.
     */
    uint64_t GetObjectId() const;

    /**
     * @return The parent containing this Object instance.
     */
    const std::shared_ptr<Object>& GetParent() const;

    std::wstring GetCustomName() const;
    
    void SetCustomName(std::wstring custom_name); 
    
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline1() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline2() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline4() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline5() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline7() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline8() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline9() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    
    virtual uint32_t GetType() = 0;
        
    swganh::messages::BaselinesMessage CreateBaselinesMessage(uint16_t view_type, uint16_t opcount = 0);
    
    swganh::messages::DeltasMessage CreateDeltasMessage(uint16_t view_type);
    
private:
    void AddBaselinesBuilders_();
    
    /**
     * Determines whether or not an object has a privileged view of
     * this Object instance.
     *
     * @return True if object has privileged view, false if not.
     */
    bool HasPrivilegedView_(uint64_t object_id) const;

    
    typedef std::vector<
        std::shared_ptr<anh::observer::ObserverInterface>
    > ObserverContainer;

    ObserverContainer observers_;
    BaselinesCacheContainer baselines_;
    DeltasCacheContainer deltas_;
    BaselinesCacheContainer public_baselines_;
    DeltasCacheContainer public_deltas_;

    typedef std::function<boost::optional<swganh::messages::BaselinesMessage>()> BaselinesBuilder;
    typedef std::vector<BaselinesBuilder> BaselinesBuilderContainer;

    BaselinesBuilderContainer baselines_builders_;

    std::shared_ptr<Object> parent_;
    
    bool is_dirty_;

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
};

}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
