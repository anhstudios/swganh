
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
#include "swganh/messages/obj_controller_message.h"

#include "swganh/object/object_controller.h"

namespace swganh {
namespace object {

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
    
    /**
     * \return True if object has a controller, false if not.
     */
    bool HasController();

    /**
     * \return The current controller for the object, or nullptr if none exists.
     *
     * \TODO Consider returning a null object instead of nullptr.
     */
    std::shared_ptr<ObjectController> GetController();

    /**
     * Sets the controller for this Object instance.
     *
     * \param controller
     */
    void SetController(const std::shared_ptr<ObjectController>& controller);
    
    /**
     * Clears the active current controller, if one exists, for this instance.
     */
    void ClearController();

    /**
     * Adds an object to be contained by the current instance.
     *
     * All Object instances are composite objects that can serve as containers
     * for other Objects. For example, storing items in an inventory. 
     *
     * \param object The object to contain.
     * \param containment_type The type of containment in which to hold the given object.
     */
    void AddContainedObject(const std::shared_ptr<Object>& object, ContainmentType containment_type);

    /**
     * Checks to see if the current Object contains the given instance.
     *
     * \param object The object to for which to check containment.
     */
    bool IsContainerForObject(const std::shared_ptr<Object>& object);

    /**
     * Checks to see if the object_id contains the given instance.
     *
     * \param the object_id to check for
     * \return object The Object to return
     */
    template<typename T>
    std::shared_ptr<T> GetContainedObject(uint64_t object_id)
    {
        boost::lock_guard<boost::recursive_mutex> lock(mutex_);

        auto find_iter = contained_objects_.find(object_id);
        if (find_iter == end(contained_objects_))
            return nullptr;
        auto object = find_iter->second;
#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(object);
#else
            return std::static_pointer_cast<T>(object);
#endif
    }

    /**
     * Removes an object from containment.
     *
     * \param object The Object to remove from containment.
     */
    void RemoveContainedObject(const std::shared_ptr<Object>& object);

    /**
     * \return A map of all contained objects and their containment types.
     */
    ObjectMap GetContainedObjects();

    /**
     * Adds an Object to the awareness list of the existing instance.
     *
     * Aware Objects receive all message updates of the Objects they
     * are aware of.
     *
     * \param object The object to be aware of.
     */
    void AddAwareObject(const std::shared_ptr<Object>& object);

    /**
     * \return True if the current Object is aware of the 
     *  given instance, false if not.
     */
    bool IsAwareOfObject(const std::shared_ptr<Object>& object);

    /**
     * Removes an object from the awareness list of the existing instance.
     */
    void RemoveAwareObject(const std::shared_ptr<Object>& object);
        
    /**
     * Returns whether or not this observable object has any observers.
     *
     * \return True if has observers, false if not.
     */
    bool HasObservers();

    /**
     * Start receiving notifications when the observable object changes state.
     *
     * \param observer The object interested in receiving state change notifications.
     */
    void Subscribe(const std::shared_ptr<anh::observer::ObserverInterface>& observer);

    /**
     * Stop receiving state notification changes for the observable object.
     *
     * \param observer The object that no longer wants state change notifications.
     */
    void Unsubscribe(const std::shared_ptr<anh::observer::ObserverInterface>& observer);
    
    /**
     * Notifies observers that the observable object has changed state.
     *
     * \param message Message containing the updated state of the observable object.
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
     * \param message Message containing the updated state of the observable object.
     */
    template<typename T>
    void NotifyObservers(const T& message)
    {
	    boost::lock_guard<boost::recursive_mutex> lock(mutex_);

        std::for_each(
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
     * \return Modified since last reliable update.
     */
    bool IsDirty();

    /**
     * Regenerates the baselines and updates observers.
     */
    void MakeClean(std::shared_ptr<swganh::object::ObjectController> controller);

    /**
     * Returns the most recently generated baselines.
     *
     * \param viewer_id The id of the object viewing this Object instance.
     * \return The most recently generated baselines.
     */
    const BaselinesCacheContainer& GetBaselines(uint64_t viewer_id) ;

    /**
     * Returns the deltas messages generated since the last time the 
     * object was made clean.
     *
     * \param viewer_id The id of the object viewing this Object instance.
     * \return The most recently generated deltas.
     */
    const DeltasCacheContainer& GetDeltas(uint64_t viewer_id) ;
    
    /**
     * Return the client iff template file that describes this Object.
     *
     * \return The object iff template file name.
     */
    const std::string& GetTemplate();

    /**
     * Sets the client iff template file that describes this Object.
     *
     * \param template_string The object iff template file name.
     */
    void SetTemplate(const std::string& template_string);
    
    /**
     * \return The object position as a vector.
     */
    glm::vec3 GetPosition();

    /**
     * Updates the object's position.
     *
     * \param position The updated position.
     */
    void SetPosition(glm::vec3 position);

	/**
	* \return bool if the object is in range
	*/
	bool InRange(glm::vec3 target, float range);

    /**
     * \return The object orientation as a quaternion.
     */
    glm::quat GetOrientation();

    /**
     * Updates the object's orientation.
     *
     * \param orientation The updated orientation.
     */
    void SetOrientation(glm::quat orientation);

    /**
     * \return Heading of the object in (angle/0.0625) where angle is in radians.
     */
    uint8_t GetHeading() ;

    /**
     * \return The container for the current object.
     */
    std::shared_ptr<Object> GetContainer();

    /**
    *  @param Type of object to return
     * \return The container for the current object.
     */
    template<typename T>
    std::shared_ptr<T> GetContainer()
    {
        boost::lock_guard<boost::recursive_mutex> lock(mutex_);
#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(container_);
#else
            return std::static_pointer_cast<T>(container_);
#endif
    }

    /**
     * Sets the container for the current object.
     *
     * \param container The new object container.
     */
    void SetContainer(const std::shared_ptr<Object>& container);

    /**
     * Base complexity for this object (primarily used for crafting).
     *
     * \return The complexity for the object.
     */
    float GetComplexity();

    /**
     * Sets the complexity for the object.
     *
     * \param complexity The new complexity for the object.
     */
    void SetComplexity(float complexity);

    /**
     * \return The stf file containing the default name for this object.
     */
    const std::string& GetStfNameFile();

    /**
     * \return The stf string containing the default name for this object.
     */
    const std::string& GetStfNameString();

    /** 
     * Sets the stf string that is the default name for this object.
     *
     * \param stf_file_name The file name containing the stf string.
     * \param stf_string The stf string containing the default object name.
     */
    void SetStfName(const std::string& stf_file_name, const std::string& stf_string);
    
    /**
     * \return The custom name of the object or an empty string if not set.
     */
    const std::wstring& GetCustomName();

    /**
     * Sets the custom name for the object.
     *
     * \param custom_name The new custom name for the object.
     */
    void SetCustomName(std::wstring custom_name); 
    
    /**
     * Returns the volume of the object, which is a measure of the "space"
     * an object takes up inside of a container.
     *
     * \return The volume of the object.
     */
    uint32_t GetVolume();

    /**
     * Sets the volume of the object.
     * 
     * \param volume The new volume for the object.
     */
    void SetVolume(uint32_t volume);
	
    /**
     * \return The ID of the scene this object belongs to.
     *
     * \TODO Consider holding a pointer back to the scene.
     */
	uint32_t GetSceneId();

    /**
     * Set's the scene id for this object.
     *
     * \param scene_id The id of the scene this object belongs to.
     */
	void SetSceneId(uint32_t scene_id);

    /**
     * Stores a deltas message update for the object.
     *
     * \param message The deltas message to store.
     */
    void AddDeltasUpdate(swganh::messages::DeltasMessage message);
    
    /**
     * Sets the id of this object instance.
     * 
     * \param id The object id.
     */
    void SetObjectId(uint64_t id);

    /**
     * \return The id of this Object instance.
     */
    uint64_t GetObjectId() ;
    
    /**
     * \return The type of the object.
     */
    virtual uint32_t GetType() const { return 0; }
            
protected:
    virtual void OnMakeClean(std::shared_ptr<swganh::object::ObjectController> controller) {}
    
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline1() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline2() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline4() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline5() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline7() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline8() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline9() { return boost::optional<swganh::messages::BaselinesMessage>(); }
    
    swganh::messages::BaselinesMessage CreateBaselinesMessage(uint8_t view_type, uint16_t opcount = 0) ;
    
    swganh::messages::DeltasMessage CreateDeltasMessage(uint8_t view_type, uint16_t update_type, uint16_t update_count = 1) ;

    mutable boost::recursive_mutex mutex_;
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
