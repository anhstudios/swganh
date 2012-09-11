// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>

#include <glm/glm.hpp>

#include "swganh_core/object/intangible/intangible.h"

namespace swganh {
namespace object {


enum WaypointStatus
{
    DEACTIVATED = 0,
    ACTIVATED = 1
};

class WaypointFactory;
class WaypointMessageBuilder;

/**
* @brief Object that defines the data in a Waypoint
*/
class Waypoint : public swganh::object::Intangible {

public:
	typedef WaypointFactory FactoryType;
    typedef WaypointMessageBuilder MessageBuilderType;

    virtual uint32_t GetType() const { return Waypoint::type; }
    const static uint32_t type = 0x57415950;


    Waypoint();
    Waypoint(glm::vec3 coordinates, bool activated, const std::string& planet, const std::wstring& name, const std::string& color);

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
    bool Active() const;
    /**
    * @brief checks if the waypoint is activated
    *   @returns 0 if the waypoint is activated, 1 else
    */
    uint8_t GetActiveFlag();
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
    const std::string& GetPlanet();
    /**
    * @brief sets the current waypoint's planet
    *   @param planet_name the planet to set the string to
    */
    void SetPlanet(const std::string& planet_name);

    /**
    * @brief gets the waypoint's Unicode name
    *   @returns name of the waypoint in Unicode
    */
    const std::wstring& GetName();
    std::string GetNameStandard();
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
    const std::string& GetColor();
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

    typedef swganh::ValueEvent<std::shared_ptr<Waypoint>> WaypointEvent;
private:

    glm::vec3 coordinates_;			                    //update 3
    std::atomic<uint8_t> activated_flag_;		        //update 3
    std::atomic<uint64_t> location_network_id_;	        //update 3
    std::string planet_name_;		                    //update 3
    std::wstring name_;				                    //update 3
    std::string color_;				                    //update 3
};

}} // swganh::object
