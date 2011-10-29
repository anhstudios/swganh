
#ifndef SWGANH_OBJECT_BASE_OBJECT_H_
#define SWGANH_OBJECT_BASE_OBJECT_H_

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "anh/observer/observable_interface.h"
#include "anh/observer/observer_interface.h"

#include "swganh/messages/base_baselines_message.h"
#include "swganh/messages/baselines_message.h"
#include "swganh/messages/deltas_message.h"


namespace swganh {
namespace object {
                    
class ObjectController;

typedef std::vector<
    swganh::messages::BaselinesMessage
> BaselinesCacheContainer;

typedef std::vector<
    swganh::messages::DeltasMessage
> DeltasCacheContainer;

class Object : public anh::observer::ObservableInterface, public std::enable_shared_from_this<Object>
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

	enum Type : uint16_t
	{
		PLAYER = 1,
		CREATURE,
		TANGIBLE,
		INTANGIBLE,
		CELL,
		MANUFACTURE_SCHEMATIC,
		MISSION,
		WAYPOINT,
		BUILDING,
		FACTORY_CRATE,
		INSTALLATION,
		SHIP,
		WEAPON,
		GUILD,
		GROUP
	};

    enum ContainmentType : uint32_t
    {
        UNLINK = 0xFFFFFFFF,
        LINK = 4
    };
    
    typedef std::map<
        uint64_t,
        std::shared_ptr<Object>
    > ObjectMap;

public:
    Object();
    virtual ~Object() {}
    
    bool HasController();
    const std::shared_ptr<ObjectController>& GetController();
    void SetController(const std::shared_ptr<ObjectController>& controller);
    void ClearController();

    void AddContainedObject(const std::shared_ptr<Object>& object, uint32_t containment_type);
    bool IsContainerForObject(const std::shared_ptr<Object>& object);
    void RemoveContainedObject(const std::shared_ptr<Object>& object);
    ObjectMap GetContainedObjects();

    void AddAwareObject(const std::shared_ptr<Object>& object);
    bool IsAwareOfObject(const std::shared_ptr<Object>& object);
    void RemoveAwareObject(const std::shared_ptr<Object>& object);
        
    /**
     * Returns whether or not this observable object has any observers.
     *
     * @return True if has observers, false if not.
     */
    bool HasObservers();

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
    template<typename T>
    void NotifyObservers(const swganh::messages::BaseBaselinesMessage<T>& message)
    {
        if (! (message.view_type == 3 || message.view_type == 6))
        {
            if (HasController())
            {
                controller_->Notify(message);
            }

            return;
        }

        std::for_each(
            observers_.begin(),
            observers_.end(),
            [this, &message] (const std::shared_ptr<anh::observer::ObserverInterface>& observer)
        {
            observer->Notify(message);
        });
    }
    
    /**
     * Notifies observers that the observable object has changed state.
     *
     * @param message Message containing the updated state of the observable object.
     */
    template<typename T>
    void NotifyObservers(const T& message)
    {
        for_each(
            observers_.begin(),
            observers_.end(),
            [&message] (const std::shared_ptr<anh::observer::ObserverInterface>& observer)
        {
            observer->Notify(message);
        });
    }

    void NotifyObservers(const anh::ByteBuffer& message);

    /**
     * Returns whether or not the object has been modified since the last reliable
     * update was sent out.
     *
     * @return Modified since last reliable update.
     */
    bool IsDirty();

    /**
     * Regenerates the baselines and updates observers.
     */
    void MakeClean(std::shared_ptr<swganh::object::ObjectController> controller);

    /**
     * Returns the most recently generated baselines.
     *
     * @param viewer_id The id of the object viewing this Object instance.
     * @return The most recently generated baselines.
     */
    const BaselinesCacheContainer& GetBaselines(uint64_t viewer_id) ;

    /**
     * Returns the deltas messages generated since the last time the 
     * object was made clean.
     *
     * @param viewer_id The id of the object viewing this Object instance.
     * @return The most recently generated deltas.
     */
    const DeltasCacheContainer& GetDeltas(uint64_t viewer_id) ;
    
    const std::string& GetTemplate();
    void SetTemplate(const std::string& template_string);
    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 position);
    glm::quat GetOrientation();

    void SetOrientation(glm::quat orientation);

    /**
     * @return Heading of the object in (angle/0.0625) where angle is in radians.
     */
    uint8_t GetHeading() ;

    const std::shared_ptr<Object>& GetContainer();
    void SetContainer(const std::shared_ptr<Object>& container);

    float GetComplexity();
    void SetComplexity(float complexity);
    const std::string& GetStfNameFile();
    void SetStfNameFile(const std::string& stf_name_file);
    const std::string& GetStfNameString();
    void SetStfNameString(const std::string& stf_name_string);
    const std::wstring& GetCustomName();
	std::string GetCustomNameStandard();
    void SetCustomName(std::wstring custom_name); 
    uint32_t GetVolume();
    void SetVolume(uint32_t volume);
	
	uint32_t GetSceneId();
	void SetSceneId(uint32_t scene_id);

    /**
     * Stores a deltas message update for the object.
     *
     * @param message The deltas message to store.
     */
    void AddDeltasUpdate(swganh::messages::DeltasMessage message);
    
    void SetObjectId(uint64_t id);
    /**
     * @return The id of this Object instance.
     */
    uint64_t GetObjectId() ;
    
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline1() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline2() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline4() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline5() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline7() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline8() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline9() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    
    virtual uint32_t GetType() const { return 0; }
        
    swganh::messages::BaselinesMessage CreateBaselinesMessage(uint16_t view_type, uint16_t opcount = 0) ;
    
    swganh::messages::DeltasMessage CreateDeltasMessage(uint16_t view_type, uint16_t update_type, uint16_t update_count = 1) ;
    
protected:
    boost::recursive_mutex    mutex_;
	uint64_t object_id_;             // create
	uint32_t scene_id_;				 // create
    std::string template_string_;    // create
    glm::vec3 position_;             // create
    glm::quat orientation_;          // create
    float complexity_;               // update 3
    std::string stf_name_file_;      // update 3
    std::string stf_name_string_;    // update 3
    std::wstring custom_name_;       // update 3
    uint32_t volume_;                // update 3

private:
	friend class ObjectMessageBuilder;
    friend class ObjectFactory;

    void AddBaselinesBuilders_();
        
    typedef std::vector<
        std::shared_ptr<anh::observer::ObserverInterface>
    > ObserverContainer;

    typedef std::function<boost::optional<swganh::messages::BaselinesMessage>()> BaselinesBuilder;
    typedef std::vector<BaselinesBuilder> BaselinesBuilderContainer;

    ObjectMap aware_objects_;
    ObjectMap contained_objects_;

    ObserverContainer observers_;
    BaselinesCacheContainer baselines_;
    DeltasCacheContainer deltas_;

    BaselinesBuilderContainer baselines_builders_;

    std::shared_ptr<Object> container_;
    std::shared_ptr<ObjectController> controller_;

    bool is_dirty_;  
};

}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
