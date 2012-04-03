#ifndef SWGANH_OBJECT_WAYPOINT_WAYPOINT_H_
#define SWGANH_OBJECT_WAYPOINT_WAYPOINT_H_

#include <atomic>
#include <mutex>

#include <glm/glm.hpp>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace waypoint {

enum WaypointStatus
{
    DEACTIVATED = 0,
    ACTIVATED = 1
};

/**
* @brief Object that defines the data in a Waypoint
*/
class Waypoint : public swganh::object::Object {

public:
    virtual uint32_t GetType() const { return Waypoint::type; }
    const static uint32_t type = 0x57415950;


    Waypoint();
    Waypoint(glm::vec3 coordinates, bool activated, const std::string& planet, const std::wstring& name, const std::string& color);

    /**
    * @brief Waypoints do not have uses
    */
    uint32_t GetUses();
    void SetUses(uint32_t uses);

    /**
    * @brief gets the glm::vec3 coordinates of the waypoint
    *
    *   @returns glm::vec3 coordinates
    */
    glm::vec3 GetCoordinates() ;
    /**
    * @brief sets the glm::vec3 coordinates of the waypoint
    *   @param coords referenced glm::vec3 object
    */
    void SetCoordinates(const glm::vec3& coords);
    /**
    * @brief sets the coordinates of the waypoint
    *   @param float x, x coordinate
    *   @param float y, y coordinate
    *   @param float z, z coordinate
    */
    void SetCoordinates(float x, float y, float z);

    /**
    * @brief checks if the waypoint is activated
    *   @returns true if the waypoint is activated
    */
    bool Active() const { return activated_flag_ == 1; }
    /**
    * @brief checks if the waypoint is activated
    *   @returns 0 if the waypoint is activated, 1 else
    */
    uint8_t GetActiveFlag() { return activated_flag_; }
    /**
    * @brief sets the current waypoint to Activated
    */
    void Activate();
    /**
    * @brief sets the current waypoint to DeActivated
    */
    void DeActivate();

    uint64_t GetLocationNetworkId() const;

    void SetLocationNetworkId(uint64_t location_network_id);

    /**
    * @brief Gets the string of the planet name as seen in the CRC Tables
    *   @returns string of the planet the waypoint is located
    */
    const std::string& GetPlanet() { return planet_name_; }
    /**
    * @brief sets the current waypoint's planet
    *   @param planet_name the planet to set the string to
    */
    void SetPlanet(const std::string& planet_name);

    /**
    * @brief gets the waypoint's Unicode name
    *   @returns name of the waypoint in Unicode
    */
    const std::wstring& GetName() { return name_; }
    std::string GetNameStandard() { return std::string(std::begin(name_), std::end(name_)); }
    /**
    * @brief sets the waypoint's Unicode name
    *   @param name of the waypoint in Unicode
    */
    void SetName(const std::wstring& name);

    /**
    * @brief gets the waypoint's current color
    *
    *   Possible Options: green,purple,white,blue,yellow,orange,space
    *   @returns color of the waypoint
    */
    const std::string& GetColor() { return color_; }
    /**
    * @brief gets the waypoint's current color as a Byte
    *
    *   Possible Options: 1 - blue,2 - green,3 - orange,4 - yellow,5 - purple/red,6 - white,7 - JTL
    *   @returns Byte color of the waypoint
    */
    uint8_t GetColorByte();
    /**
    * @brief sets the waypoint's color
    *
    *   Possible Options: green,purple,white,blue,yellow,orange,space
    *   @param color of the waypoint
    */
    void SetColor(const std::string& color);
    /**
    * @brief sets the waypoint's color byte
    *
    *   Possible Options: 1 - blue,2 - green,3 - orange,4 - yellow,5 - purple/red,6 - white,7 - JTL
    *   @param color_byte to set the waypoint
    */
    void SetColorByte(uint8_t color_byte);

protected:
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
private:
	friend class WaypointFactory;

    mutable std::mutex waypoint_mutex_;

    std::atomic<uint32_t> uses_;					//update 3
    glm::vec3 coordinates_;			//update 3
    std::atomic<uint8_t> activated_flag_;		//update 3
    std::atomic<uint64_t> location_network_id_;	//update 3
    std::string planet_name_;		//update 3
    std::wstring name_;				//update 3
    std::atomic<uint8_t> not_used_;				//update 3
    std::string color_;				//update 3
};

}}} // swganh::object::waypoint

#endif //SWGANH_OBJECT_WAYPOINT_WAYPOINT_H_
