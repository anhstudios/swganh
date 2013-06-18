// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

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

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

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
#include "swganh_core/object/container_interface.h"

#include "swganh_core/object/slot_interface.h"

#define DISPATCH(BIG, LITTLE) if(auto dispatcher = GetEventDispatcher()) \
{dispatcher->Dispatch(make_shared<BIG ## Event>(#BIG "::" #LITTLE, static_pointer_cast<BIG>(shared_from_this())));}

namespace swganh {
namespace object {

typedef std::map<
	swganh::HashString,
	boost::variant<float, int64_t, std::wstring>
> AttributesMap;

typedef std::map<
	int32_t,
	std::shared_ptr<SlotInterface>
> ObjectSlots;

typedef boost::variant<float, int64_t, std::wstring, boost::blank> AttributeVariant;

typedef std::vector<std::vector<int32_t>> ObjectArrangements;

typedef swganh::ValueEvent<std::shared_ptr<Object>> ObjectEvent;

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point> CollisionBox;
typedef boost::geometry::model::box<Point> AABB;

class ObjectFactory;
class ObjectMessageBuilder;
class ContainerPermissionsInterface;

std::tuple<boost::unique_lock<boost::mutex>, boost::unique_lock<boost::mutex>> LockSimultaneously(std::shared_ptr<Object>& obj1, std::shared_ptr<Object>& obj2);

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
    virtual ~Object();

    /**
     * @return True if object has a controller, false if not.
     */
    bool HasController();
	bool HasController(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The current controller for the object, or nullptr if none exists.
     *
     * @TODO Consider returning a null object instead of nullptr.
     */
    std::shared_ptr<swganh::observer::ObserverInterface> GetController();
	std::shared_ptr<swganh::observer::ObserverInterface> GetController(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the controller for this Object instance.
     *
     * @param controller
     */
    void SetController(const std::shared_ptr<swganh::observer::ObserverInterface>& controller);
	void SetController(const std::shared_ptr<swganh::observer::ObserverInterface>& controller, boost::unique_lock<boost::mutex>& lock);

    /**
     * Clears the active current controller, if one exists, for this instance.
     */
    void ClearController();
	void ClearController(boost::unique_lock<boost::mutex>& lock);

	virtual void AddObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> newObject, int32_t arrangement_id=-2);
	virtual void RemoveObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> oldObject);
	virtual void TransferObject(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, glm::vec3 new_position,  int32_t arrangement_id=-2);
	virtual void SwapSlots(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, int32_t new_arrangement_id);
	
	virtual void __InternalTransfer(std::shared_ptr<Object> requester, std::shared_ptr<Object> object, std::shared_ptr<ContainerInterface> newContainer, int32_t arrangement_id=-2);
	virtual bool __HasAwareObject(std::shared_ptr<Object> object);
	virtual void __InternalAddAwareObject(std::shared_ptr<Object> object, bool reverse_still_valid);
	virtual void __InternalViewAwareObjects(std::function<void(std::shared_ptr<swganh::object::Object>)> func, std::shared_ptr<swganh::object::Object> hint=nullptr);
	virtual void __InternalRemoveAwareObject(std::shared_ptr<Object> object, bool reverse_still_valid);

	virtual int32_t __InternalInsert(std::shared_ptr<Object> object, glm::vec3 new_position, int32_t arrangement_id=-2);
    virtual void __InternalViewObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::function<void(std::shared_ptr<Object>)> func);
	virtual void __InternalGetObjects(std::shared_ptr<Object> requester, uint32_t max_depth, bool topDown, std::list<std::shared_ptr<Object>>& out);

	virtual void __InternalGetAbsolutes(glm::vec3& pos, glm::quat& rot);

	/**
     * Returns whether or not this observable object has any observers.
     *
     * @return True if has observers, false if not.
     */
    bool HasObservers();
	bool HasObservers(boost::unique_lock<boost::mutex>& lock);

    /**
     * Start receiving notifications when the observable object changes state.
     *
     * @param observer The object interested in receiving state change notifications.
     */
    void Subscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer);
	void Subscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer, boost::unique_lock<boost::mutex>& lock);

    /**
     * Stop receiving state notification changes for the observable object.
     *
     * @param observer The object that no longer wants state change notifications.
     */
    void Unsubscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer);
	void Unsubscribe(const std::shared_ptr<swganh::observer::ObserverInterface>& observer, boost::unique_lock<boost::mutex>& lock);

    /**
     * Notifies observers that the observable object has changed state.
     *
     * @param message Message containing the updated state of the observable object.
     */
	template<typename T>
	void NotifyObservers(swganh::messages::BaseBaselinesMessage* message) {
        auto lock = AcquireLock();
        NotifyObservers(message, lock);
    }

    template<typename T>
    void NotifyObservers(swganh::messages::BaseBaselinesMessage* message, boost::unique_lock<boost::mutex>& lock)
    {
        if (! (message->view_type == 3 || message->view_type == 6))
        {
            if (HasController(lock))
            {
                GetController(lock)->Notify(message);
            }

            return;
        }

        NotifyObservers(message, lock);
    }

	void NotifyObservers(swganh::messages::BaseSwgMessage* message);
	void NotifyObservers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    /**
     * Returns whether or not the object has been modified since the last reliable
     * update was sent out.
     *
     * @return Modified since last reliable update.
     */
    bool IsDirty(boost::unique_lock<boost::mutex>& lock);

    /**
     * Return the client iff template file that describes this Object.
     *
     * @return The object iff template file name.
     */
    std::string GetTemplate();
	std::string GetTemplate(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the client iff template file that describes this Object.
     *
     * @param template_string The object iff template file name.
     */
    void SetTemplate(const std::string& template_string);
	void SetTemplate(const std::string& template_string, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The object position as a vector.
     */
    glm::vec3 GetPosition();
	glm::vec3 GetPosition(boost::unique_lock<boost::mutex>& lock);

    /**
     * Updates the object's position.
     *
     * @param position The updated position.
     */
    void SetPosition(glm::vec3 position);
	void SetPosition(glm::vec3 position, boost::unique_lock<boost::mutex>& lock);

	/**
	 * Updates the objects position (server initiated)
	 *
	 * @param contained object
	 * @param position of the updated position
	 */
	void UpdatePosition(const glm::vec3& new_position, const glm::quat& quaternion, std::shared_ptr<Object> parent=nullptr);

	/**
	* @return bool if the object is in range
	*/
	bool InRange(glm::vec3 target, float range);

	/**
	* @return float of the range to the given target
	*/
	float RangeTo(glm::vec3 target);

    /**
     * @return The object orientation as a quaternion.
     */
    glm::quat GetOrientation();
	glm::quat GetOrientation(boost::unique_lock<boost::mutex>& lock);

    /**
     * Faces an object by adjusting the orientation
     *
     */
    void FaceObject(const std::shared_ptr<Object>& object);

	void FacePosition(const glm::vec3& position);
	void FacePosition(const glm::vec3& position, boost::unique_lock<boost::mutex>& lock);

    /**
     * Updates the object's orientation.
     *
     * @param orientation The updated orientation.
     */
    void SetOrientation(glm::quat orientation);
	void SetOrientation(glm::quat orientation, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return Heading of the object in (angle/0.0625) where angle is in radians.
     */
	uint8_t GetHeading() ;
    uint8_t GetHeading(glm::quat orientation) ;

    /**
     * @return The container for the current object.
     */
    virtual std::shared_ptr<ContainerInterface> GetContainer();
	virtual std::shared_ptr<ContainerInterface> GetContainer(boost::unique_lock<boost::mutex>& lock);

    /**
    *  @param Type of object to return
     * @return The container for the current object.
     */
	template<typename T> std::shared_ptr<T> GetContainer() { return GetContainer<T>(AcquireLock()); }
    template<typename T>
    std::shared_ptr<T> GetContainer(boost::unique_lock<boost::mutex>& lock)
    {
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
	void SetContainer(const std::shared_ptr<ContainerInterface>& container, boost::unique_lock<boost::mutex>& lock);

    /**
     * Base complexity for this object (primarily used for crafting).
     *
     * @return The complexity for the object.
     */
    float GetComplexity();
	float GetComplexity(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the complexity for the object.
     *
     * @param complexity The new complexity for the object.
     */
    void SetComplexity(float complexity);
	void SetComplexity(float complexity, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The stf file containing the default name for this object.
     */
    std::string GetStfNameFile();
	std::string GetStfNameFile(boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The stf string containing the default name for this object.
     */
    std::string GetStfNameString();
	std::string GetStfNameString(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the stf string that is the default name for this object.
     *
     * @param stf_file_name The file name containing the stf string.
     * @param stf_string The stf string containing the default object name.
     */
    void SetStfName(const std::string& stf_file_name, const std::string& stf_string);
	void SetStfName(const std::string& stf_file_name, const std::string& stf_string, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The custom name of the object or an empty string if not set.
     */
    std::wstring GetCustomName();
	std::wstring GetCustomName(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the custom name for the object.
     *
     * @param custom_name The new custom name for the object.
     */
    void SetCustomName(std::wstring custom_name);
	void SetCustomName(std::wstring custom_name, boost::unique_lock<boost::mutex>& lock);
    
    /**
     * @return Returns the firstname of a custom name (if set) otherwise empty string is returned.
     */
    std::wstring GetFirstName() const;
	std::wstring GetFirstName(boost::unique_lock<boost::mutex>& lock) const;
    
    /**
     * @return Returns the sir name of a custom name (if set) otherwise empty string is returned.
     */
    std::wstring GetSurName() const;
	std::wstring GetSurName(boost::unique_lock<boost::mutex>& lock) const;

    /**
     * Returns the volume of the object, which is a measure of the "space"
     * an object takes up inside of a container.
     *
     * @return The volume of the object.
     */
    uint32_t GetVolume();
	uint32_t GetVolume(boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the volume of the object.
     *
     * @param volume The new volume for the object.
     */
    void SetVolume(uint32_t volume);
	void SetVolume(uint32_t volume, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The ID of the scene this object belongs to.
     *
     * @TODO Consider holding a pointer back to the scene.
     */
	uint32_t GetSceneId();
	uint32_t GetSceneId(boost::unique_lock<boost::mutex>& lock);

    /**
     * Set's the scene id for this object.
     *
     * @param scene_id The id of the scene this object belongs to.
     */
	void SetSceneId(uint32_t scene_id);
	void SetSceneId(uint32_t scene_id, boost::unique_lock<boost::mutex>& lock);

	uint32_t GetInstanceId();
	uint32_t GetInstanceId(boost::unique_lock<boost::mutex>& lock);

	void SetInstanceId(uint32_t instance_id);
	void SetInstanceId(uint32_t instance_id, boost::unique_lock<boost::mutex>& lock);

    /**
     * Sets the id of this object instance.
     *
     * @param id The object id.
     */
    void SetObjectId(uint64_t id);
	void SetObjectId(uint64_t id, boost::unique_lock<boost::mutex>& lock);

    /**
     * @return The id of this Object instance.
     */
    uint64_t GetObjectId();
	uint64_t GetObjectId(boost::unique_lock<boost::mutex>& lock);

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
	void SetSlotInformation(ObjectSlots slots, ObjectArrangements arrangements, boost::unique_lock<boost::mutex>& lock);

	swganh::EventDispatcher* GetEventDispatcher();
    void SetEventDispatcher(swganh::EventDispatcher* dispatcher);

	void SetFlag(std::string flag);
	void SetFlag(std::string flag, boost::unique_lock<boost::mutex>& lock);

    void RemoveFlag(std::string flag);
	void RemoveFlag(std::string flag, boost::unique_lock<boost::mutex>& lock);

    bool HasFlag(std::string flag);
	bool HasFlag(std::string flag, boost::unique_lock<boost::mutex>& lock);

	/**
	 * @brief Creates and fires off the Baseline event to send the Baselines for the given object
	 */
	virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	
	/**
	 * @brief Sends the create by crc message to the observer of 'this' object
	 */
	virtual void SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	virtual void SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock);
	
	/**
	 * @brief sends the update containment message for the given observer of this object
	 */
	virtual void SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer, bool send_on_no_parent=true);
	void SendUpdateContainmentMessage(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock, bool send_on_no_parent);
	
	/**
	 * @brief sends the destroy message for the given observer of this object
	 */
	virtual void SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	virtual void SendDestroy(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock);

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
	bool ClearSlot(int32_t slot_id, boost::unique_lock<boost::mutex>& lock);
	
	/**
	 * @brief Gets the slot object by slot_id
	 */
	std::shared_ptr<Object> GetSlotObject(int32_t slot_id);
	std::shared_ptr<Object> GetSlotObject(int32_t slot_id, boost::unique_lock<boost::mutex>& lock);

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
	bool IsDatabasePersisted(boost::unique_lock<boost::mutex>& lock);

	bool IsInSnapshot();
	bool IsInSnapshot(boost::unique_lock<boost::mutex>& lock);

	void SetDatabasePersisted(bool value);
	void SetDatabasePersisted(bool value, boost::unique_lock<boost::mutex>& lock);

	void SetInSnapshot(bool value);
	void SetInSnapshot(bool value, boost::unique_lock<boost::mutex>& lock);

	/**
	 * @brief Gets the Attribute Map
	 */ 
	AttributesMap GetAttributeMap();
	AttributesMap GetAttributeMap(boost::unique_lock<boost::mutex>& lock);

	bool HasAttribute(const std::string& name);
	bool HasAttribute(const std::string& name, boost::unique_lock<boost::mutex>& lock);

	/**
	 * @brief Sets an attribute of the specified type
	 */
	template<typename T>
	void SetAttribute(const std::string& name, T attribute) {
        auto lock = AcquireLock();
        SetAttribute(name, attribute, lock);
    }

	template<typename T>
	void SetAttribute(const std::string& name, T attribute, boost::unique_lock<boost::mutex>& lock)
	{
		attributes_map_[name] = attribute;

		if (event_dispatcher_)
			event_dispatcher_->Dispatch(std::make_shared<ObjectEvent>("Object::UpdateAttribute", shared_from_this()));
	}

	/**
	 * @brief Gets an attribute as the specified T type
	 */
	template<typename T>
	T GetAttribute(const std::string& name) {
        auto lock = AcquireLock();
        return GetAttribute<T>(name, lock);
    }
	
	template<typename T>
	T GetAttribute(const std::string& name, boost::unique_lock<boost::mutex>& lock)
	{
		auto val = GetAttribute(name, lock);
		// Return if not blank
		if (val.which() != 3)
			return boost::get<T>(val);
		return 0;
	}
	/**
	 * @brief Gets an attribute value and then converts it to a wstring for printing
	 */
	std::wstring GetAttributeAsString(const std::string& name);
	std::wstring GetAttributeAsString(const std::string& name, boost::unique_lock<boost::mutex>& lock);

	AttributeVariant GetAttribute(const std::string& name);
	AttributeVariant GetAttribute(const std::string& name, boost::unique_lock<boost::mutex>& lock);

	int8_t GetAttributeTemplateId();
	int8_t GetAttributeTemplateId(boost::unique_lock<boost::mutex>& lock);

	void SetAttributeTemplateId(int8_t attribute_template_id);
	void SetAttributeTemplateId(int8_t attribute_template_id, boost::unique_lock<boost::mutex>& lock);

	void AddDeltasUpdate(swganh::messages::DeltasMessage* message);
	void AddDeltasUpdate(swganh::messages::DeltasMessage* message, boost::unique_lock<boost::mutex>& lock);

	//
	// Spatial/Collision
	//
	virtual void OnCollisionEnter(std::shared_ptr<Object> collider) { }
	virtual void OnCollisionStay(std::shared_ptr<Object> collider) { }
	virtual void OnCollisionLeave(std::shared_ptr<Object> collider) { }

	void BuildSpatialProfile();
	void BuildSpatialProfile(boost::unique_lock<boost::mutex>& lock);

	const std::set<std::shared_ptr<Object>>& GetCollidedObjects(void) const;
	const std::set<std::shared_ptr<Object>>& GetCollidedObjects(boost::unique_lock<boost::mutex>& lock) const;

	void AddCollidedObject(std::shared_ptr<Object> obj);
	void AddCollidedObject(std::shared_ptr<Object> obj,boost::unique_lock<boost::mutex>& lock);

	void RemoveCollidedObject(std::shared_ptr<Object> obj);
	void RemoveCollidedObject(std::shared_ptr<Object> obj, boost::unique_lock<boost::mutex>& lock);

	const CollisionBox& GetLocalCollisionBox(void) const;
	const CollisionBox& GetLocalCollisionBox(boost::unique_lock<boost::mutex>& lock) const;

	const CollisionBox& GetWorldCollisionBox(void) const;
	const CollisionBox& GetWorldCollisionBox(boost::unique_lock<boost::mutex>& lock) const;

	const AABB& GetAABB(void) const;
	const AABB& GetAABB(boost::unique_lock<boost::mutex>& lock) const;

	void SetCollisionBoxSize(float length, float height);
	void SetCollisionBoxSize(float length, float height, boost::unique_lock<boost::mutex>& lock);

	void SetCollidable(bool collidable);
	void SetCollidable(bool collidable, boost::unique_lock<boost::mutex>& lock);

	bool IsCollidable(void) const;
	bool IsCollidable(boost::unique_lock<boost::mutex>& lock) const;

	boost::unique_lock<boost::mutex> AcquireLock() const;
	boost::unique_lock<boost::mutex> AcquireLock(boost::defer_lock_t t) const;
	boost::unique_lock<boost::mutex> AcquireLock(boost::try_to_lock_t t) const;
	boost::unique_lock<boost::mutex> AcquireLock(boost::adopt_lock_t t) const;

protected: 
	virtual void __BuildCollisionBox(void);
	CollisionBox local_collision_box_;
private:
    
    typedef std::set<std::shared_ptr<swganh::observer::ObserverInterface>> ObserverContainer;
	typedef std::set<std::shared_ptr<swganh::object::Object>> AwareObjectContainer;
    
	void BuildCollisionBox();
	void BuildCollisionBox(boost::unique_lock<boost::mutex>& lock);
	
	void UpdateAABB();
	void UpdateAABB(boost::unique_lock<boost::mutex>& lock);
	
	void UpdateWorldCollisionBox();
	void __InternalUpdateWorldCollisionBox();

	mutable boost::mutex object_mutex_;

	AttributesMap attributes_map_;

    ObjectSlots slot_descriptor_;
	ObjectArrangements slot_arrangements_;

    ObserverContainer observers_;
	AwareObjectContainer aware_objects_;

    std::shared_ptr<ContainerInterface> container_;

    bool is_dirty_;
	bool database_persisted_;
	bool in_snapshot_;

    std::set<std::string> flags_;

	uint64_t object_id_;
	uint32_t scene_id_;
    uint32_t instance_id_;
	std::string template_string_;
    glm::vec3 position_;
    glm::quat orientation_;
    float complexity_;
    std::string stf_name_file_;
    std::string stf_name_string_;
    std::wstring custom_name_;
    uint32_t volume_;
    int32_t arrangement_id_;
	int8_t attributes_template_id;

	std::set<std::shared_ptr<Object>> collided_objects_;
	
	CollisionBox world_collision_box_;
	AABB aabb_;

	float collision_length_;
	float collision_height_;
	bool collidable_;

	std::shared_ptr<swganh::observer::ObserverInterface> controller_;
	swganh::EventDispatcher* event_dispatcher_;
};

}}  // namespace
