// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_BASE_OBJECT_H_
#define SWGANH_OBJECT_BASE_OBJECT_H_

#include <cstdint>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <boost/thread/mutex.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "swganh/event_dispatcher.h"
#include "swganh/observer/observable_interface.h"
#include "swganh_core/object/object_controller.h"

#include "swganh_core/messages/base_baselines_message.h"
#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/obj_controller_message.h"

#include "swganh/observer/observer_interface.h"
#include "swganh/object/container_interface.h"

#include "swganh/object/slot_interface.h"

namespace swganh {

namespace messages {
namespace controllers {
	class ObjectMenuResponse;
	struct RadialOptions;
}}
namespace object {

typedef std::vector<
    swganh::messages::BaselinesMessage
> BaselinesCacheContainer;

typedef std::vector<
    swganh::messages::DeltasMessage
> DeltasCacheContainer;

typedef std::map<
	swganh::HashString,
	boost::variant<float, int32_t, std::wstring>
> AttributesMap;

typedef std::map<
	int32_t,
	std::shared_ptr<SlotInterface>
> ObjectSlots;

typedef std::vector<std::vector<int32_t>> ObjectArrangements;

typedef swganh::ValueEvent<std::shared_ptr<Object>> ObjectEvent;

class ObjectFactory;
class ObjectMessageBuilder;
class ContainerPermissionsInterface;

class Object : 
	public swganh::observer::ObservableInterface, 
	public swganh::object::ContainerInterface, 
	public std::enable_shared_from_this<Object>
{
public:
    const static uint32_t type = 0;

    typedef ObjectFactory FactoryType;
    typedef ObjectMessageBuilder MessageBuilderType;

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
    // These match the database
	enum Type : uint32_t
	{
		INVALID = 0,
		PLAYER = 1347174745,
		CREATURE = 1129465167,
		TANGIBLE = 1413566031,
		INTANGIBLE = 1230261839,
		CELL = 1396919380,
		MANUFACTURE_SCHEMATIC = 1297302351,
		MISSION = 1296651087,
		WAYPOINT = 1463900496,
		BUILDING = 1112885583,
		FACTORY_CRATE = 1178818900,
		INSTALLATION = 1229869903,
		SHIP = 1874303456,
		WEAPON = 1789660414,
		GUILD = 1145850183,
		GROUP = 1196578128,
		RESOURCE_CONTAINER = 1380142671
	};
	
	typedef std::map<
		uint64_t,
		std::shared_ptr<Object>
	> ObjectMap;
    
	typedef std::shared_ptr<ContainerPermissionsInterface> PermissionsObject;

    Object();
    virtual ~Object() {}

    /**
     * @return True if object has a controller, false if not.
     */
    bool HasController();

    /**
     * @return The current controller for the object, or nullptr if none exists.
     *
     * @TODO Consider returning a null object instead of nullptr.
     */
    std::shared_ptr<swganh::observer::ObserverInterface> GetController();

    /**
     * Sets the controller for this Object instance.
     *
     * @param controller
     */
    void SetController(const std::shared_ptr<swganh::observer::ObserverInterface>& controller);

    /**
     * Clears the active current controller, if one exists, for this instance.
     */
    void ClearController();

	virtual void AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> newObject, int32_t arrangement_id=-2);
	virtual void RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject);
	virtual void TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2);
	virtual void SwapSlots(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, int32_t new_arrangement_id);
	
	virtual void __InternalTransfer(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2);
	virtual bool __HasAwareObject(std::shared_ptr<Object> object);
	virtual void __InternalAddAwareObject(std::shared_ptr<Object> object);
	virtual void __InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr);
	virtual void __InternalRemoveAwareObject(std::shared_ptr<Object> object);

	virtual int32_t __InternalInsert(std::shared_ptr<Object> object, int32_t arrangement_id=-2);
    virtual void __InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func);

	virtual glm::vec3 __InternalGetAbsolutePosition();

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
    void Subscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

    /**
     * Stop receiving state notification changes for the observable object.
     *
     * @param observer The object that no longer wants state change notifications.
     */
    void Unsubscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

    /**
     * Notifies observers that the observable object has changed state.
     *
     * @param message Message containing the updated state of the observable object.
     */
    template<typename T>
    void NotifyObservers(swganh::messages::BaseBaselinesMessage* message)
    {
        if (! (message.view_type == 3 || message.view_type == 6))
        {
            if (HasController())
            {
                GetController()->Notify(message);
            }

            return;
        }

        boost::lock_guard<boost::mutex> lock(object_mutex_);

        NotifyObservers<T>(message);
    }

	void NotifyObservers(swganh::messages::BaseSwgMessage* message);

    /**
     * Returns whether or not the object has been modified since the last reliable
     * update was sent out.
     *
     * @return Modified since last reliable update.
     */
    bool IsDirty();

    /**
     * Returns the most recently generated baselines.
     *
     * @return The most recently generated baselines.
     */
    BaselinesCacheContainer GetBaselines() ;

    /**
     * Returns the deltas messages generated since the last time the
     * object was made clean.
     *
     * @param viewer_id The id of the object viewing this Object instance.
     * @return The most recently generated deltas.
     */
    DeltasCacheContainer GetDeltas(uint64_t viewer_id) ;

    /**
     * Return the client iff template file that describes this Object.
     *
     * @return The object iff template file name.
     */
    std::string GetTemplate();

    /**
     * Sets the client iff template file that describes this Object.
     *
     * @param template_string The object iff template file name.
     */
    void SetTemplate(const std::string& template_string);

    /**
     * @return The object position as a vector.
     */
    glm::vec3 GetPosition();

    /**
     * Updates the object's position.
     *
     * @param position The updated position.
     */
    void SetPosition(glm::vec3 position);

	/**
	* @return bool if the object is in range
	*/
	bool InRange(glm::vec3 target, float range);

    /**
     * @return The object orientation as a quaternion.
     */
    glm::quat GetOrientation();

    /**
     * Faces an object by adjusting the orientation
     *
     */
    void FaceObject(const std::shared_ptr<Object>& object);

    /**
     * Updates the object's orientation.
     *
     * @param orientation The updated orientation.
     */
    void SetOrientation(glm::quat orientation);

    /**
     * @return Heading of the object in (angle/0.0625) where angle is in radians.
     */
    uint8_t GetHeading() ;

    /**
     * @return The container for the current object.
     */
    virtual std::shared_ptr<ContainerInterface> GetContainer();

    /**
    *  @param Type of object to return
     * @return The container for the current object.
     */
    template<typename T>
    std::shared_ptr<T> GetContainer()
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(container_);
#else
            return std::static_pointer_cast<T>(container_);
#endif
    }

    /**
     * Sets the container for the current object.
     *
     * @param container The new object container.
     */
    void SetContainer(const std::shared_ptr<ContainerInterface>& container);

    /**
     * Base complexity for this object (primarily used for crafting).
     *
     * @return The complexity for the object.
     */
    float GetComplexity();

    /**
     * Sets the complexity for the object.
     *
     * @param complexity The new complexity for the object.
     */
    void SetComplexity(float complexity);

    /**
     * @return The stf file containing the default name for this object.
     */
    std::string GetStfNameFile();

    /**
     * @return The stf string containing the default name for this object.
     */
    std::string GetStfNameString();

    /**
     * Sets the stf string that is the default name for this object.
     *
     * @param stf_file_name The file name containing the stf string.
     * @param stf_string The stf string containing the default object name.
     */
    void SetStfName(const std::string& stf_file_name, const std::string& stf_string);

    /**
     * @return The custom name of the object or an empty string if not set.
     */
    std::wstring GetCustomName();

    /**
     * Sets the custom name for the object.
     *
     * @param custom_name The new custom name for the object.
     */
    void SetCustomName(std::wstring custom_name);

    /**
     * Returns the volume of the object, which is a measure of the "space"
     * an object takes up inside of a container.
     *
     * @return The volume of the object.
     */
    uint32_t GetVolume();

    /**
     * Sets the volume of the object.
     *
     * @param volume The new volume for the object.
     */
    void SetVolume(uint32_t volume);

    /**
     * @return The ID of the scene this object belongs to.
     *
     * @TODO Consider holding a pointer back to the scene.
     */
	uint32_t GetSceneId();

    /**
     * Set's the scene id for this object.
     *
     * @param scene_id The id of the scene this object belongs to.
     */
	void SetSceneId(uint32_t scene_id);

    /**
     * Stores a deltas message update for the object.
     *
     * @param message The deltas message to store.
     */
    void AddDeltasUpdate(swganh::messages::DeltasMessage* message);

    void AddBaselineToCache(swganh::messages::BaselinesMessage* baseline);

    /**
     * Sets the id of this object instance.
     *
     * @param id The object id.
     */
    void SetObjectId(uint64_t id);

    /**
     * @return The id of this Object instance.
     */
    uint64_t GetObjectId();

	int32_t GetArrangementId();
	void SetArrangementId(int32_t arrangement_id);

    /**
     * @return The type of the object.
     */
    virtual uint32_t GetType() const { return 0; }

	/**
	 * @brief Sets the slots and arragements information for the Object
	 * 
	 * This is used to determine which objects can be equipped into which slot for the Object
	 */
	void SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements);

	swganh::EventDispatcher* GetEventDispatcher();
    void SetEventDispatcher(swganh::EventDispatcher* dispatcher);

    void ClearBaselines();
    void ClearDeltas();
    

	void SetFlag(std::string flag);
    void RemoveFlag(std::string flag);
    bool HasFlag(std::string flag);

	/**
	 * @brief Creates and fires off the Baseline event to send the Baselines for the given object
	 */
	virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	/**
	 * @brief Sends the create by crc message to the observer of 'this' object
	 */
	virtual void SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	/**
	 * @brief sends the update containment message for the given observer of this object
	 */
	virtual void SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	/**
	 * @brief sends the destroy message for the given observer of this object
	 */
	virtual void SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	/**
	 * @brief Sets the menu response aka 'radials' for this object 
	 */
	virtual void SetMenuResponse(std::vector<swganh::messages::controllers::RadialOptions> radials); 
	/**
	 * @brief Gets the Menu response aka 'radials' for this object
	 */
	std::shared_ptr<swganh::messages::controllers::ObjectMenuResponse> GetMenuResponse();

	bool operator< (const std::shared_ptr<Object>& other)
	{ 
		return GetObjectId() < other->GetObjectId(); 
	}
	bool operator== (const std::shared_ptr<Object>& other)
	{
		return GetObjectId() == other->GetObjectId();
	}

	/**
	 * @brief Clears the given slot by slot_id
	 */
	bool ClearSlot(int32_t slot_id);
	/**
	 * @brief Gets the slot object by slot_id
	 */
	std::shared_ptr<Object> GetSlotObject(int32_t slot_id);
	/**
	 * @brief Gets the appropriate arrangement given an object
	 */
	int32_t GetAppropriateArrangementId(std::shared_ptr<Object> other);
	/**
	 * @brief Gets the slot descriptors for this object
	 */
	ObjectSlots GetSlotDescriptor();
	/**
	 * @brief Gets the slot arrangements for this object
	 */
	ObjectArrangements GetSlotArrangements();


	bool IsDatabasePersisted();
	bool IsInSnapshot();
	void SetDatabasePersisted(bool value);
	void SetInSnapshot(bool value);

	/**
	 * @brief Gets the Attribute Map
	 */ 
	AttributesMap GetAttributeMap();

	/**
	 * @brief Sets an attribute of the specified type
	 */
	template<typename T>
	void SetAttribute(const std::string& name, T attribute)
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		attributes_map_[name] = attribute;

		event_dispatcher_->Dispatch(std::make_shared<ObjectEvent>("Object::UpdateAttribute", shared_from_this()));
	}

	/**
	 * @brief Gets an attribute as the specified T type
	 */
	template<typename T>
	T GetAttribute(const std::string& name)
	{
		auto val = GetAttribute(name);
		return boost::get<T>(val);
	}
	/**
	 * @brief Gets an attribute value and then converts it to a wstring for printing
	 */
	std::wstring GetAttributeAsString(const std::string& name);
	boost::variant<float, int32_t, std::wstring> GetAttribute(const std::string& name);

	std::wstring GetAttributeRecursiveAsString(const std::string& name);
	boost::variant<float, int32_t, std::wstring> GetAttributeRecursive(const std::string& name);
	template<typename T>
	T GetAttributeRecursive(const std::string& name)
	{
		auto val = GetAttributeRecursive(name);
		return boost::get<T>(val);
	}
	template<typename T>
	T AddAttributeRecursive(T val, const std::string& name)
	{
		ViewObjects(nullptr, 1, false, [&](std::shared_ptr<Object> recurse)
		{
			T found_val = recurse->GetAttribute<T>(name);
			// Add Values
			val += found_val;
		});

		return val;
	}

	uint8_t GetAttributeTemplateId();
	void SetAttributeTemplateId(uint8_t attribute_template_id);


protected:
	// Radials
	std::shared_ptr<swganh::messages::controllers::ObjectMenuResponse> menu_response_;

	std::atomic<uint64_t> object_id_;                // create
	std::atomic<uint32_t> scene_id_;				 // create
    std::string template_string_;                    // create
    glm::vec3 position_;                             // create
    glm::quat orientation_;                          // create
    float complexity_;                               // update 3
    std::string stf_name_file_;                      // update 3
    std::string stf_name_string_;                    // update 3
    std::wstring custom_name_;                       // update 3
    std::atomic<uint32_t> volume_;                   // update 3
    std::atomic<int32_t> arrangement_id_;
	std::atomic<uint8_t> attributes_template_id;	 // Used to determine which attribute template to use
	mutable boost::mutex object_mutex_;

private:
    
    typedef std::set<std::shared_ptr<swganh::observer::ObserverInterface>> ObserverContainer;
	typedef std::set<std::shared_ptr<swganh::object::Object>> AwareObjectContainer;

	AttributesMap attributes_map_;

    ObjectSlots slot_descriptor_;
	ObjectArrangements slot_arrangements_;

    ObserverContainer observers_;
	AwareObjectContainer aware_objects_;
	std::shared_ptr<swganh::observer::ObserverInterface> controller_;

    BaselinesCacheContainer baselines_;
    DeltasCacheContainer deltas_;

    std::shared_ptr<ContainerInterface> container_;
    swganh::EventDispatcher* event_dispatcher_;

    bool is_dirty_;

	bool database_persisted_;
	bool in_snapshot_;

    boost::mutex flags_mutex_;
    std::set<std::string> flags_;
};

}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
