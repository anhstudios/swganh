// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>

#include <glm/glm.hpp>

#include "swganh_core/object/intangible/intangible.h"

namespace swganh {
namespace object {

class WaypointFactory;
class WaypointMessageBuilder;

/**
* @brief Object that defines the data in a Waypoint
*/
class Waypoint : public swganh::object::Intangible {
public:
    enum WaypointStatus
    {
        DEACTIVATED = 0,
        ACTIVATED = 1
    };
        
    enum WaypointColor
    {
        BLUE = 1,
        GREEN,
        ORANGE,
        YELLOW,
        PURPLE,
        WHITE,
        SPACE
    };

	typedef WaypointFactory FactoryType;
    typedef WaypointMessageBuilder MessageBuilderType;

    virtual uint32_t GetType() const { return Waypoint::type; }
    const static uint32_t type = 0x57415950;


    Waypoint();
    Waypoint(glm::vec3 coordinates, bool activated, const std::string& planet, const std::wstring& name, WaypointColor color);

    /**
    * @brief gets the glm::vec3 coordinates of the waypoint
    *
    *   @returns glm::vec3 coordinates
    */
    glm::vec3 GetCoordinates() ;
	glm::vec3 GetCoordinates(boost::unique_lock<boost::mutex>& lock) ;

    /**
    * @brief sets the glm::vec3 coordinates of the waypoint
    *   @param coords referenced glm::vec3 object
    */
    void SetCoordinates(const glm::vec3& coords);
	void SetCoordinates(const glm::vec3& coords, boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief checks if the waypoint is activated
    *   @returns true if the waypoint is activated
    */
    bool Active() const;
	bool Active(boost::unique_lock<boost::mutex>& lock) const;

    /**
    * @brief checks if the waypoint is activated
    *   @returns 0 if the waypoint is activated, 1 else
    */
    uint8_t GetActiveFlag();
	uint8_t GetActiveFlag(boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief sets the current waypoint to Activated
    */
    void Activate();
	void Activate(boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief sets the current waypoint to DeActivated
    */
    void DeActivate();
	void DeActivate(boost::unique_lock<boost::mutex>& lock);

    uint64_t GetLocationNetworkId() const;
	uint64_t GetLocationNetworkId(boost::unique_lock<boost::mutex>& lock) const;

    void SetLocationNetworkId(uint64_t location_network_id);
	void SetLocationNetworkId(uint64_t location_network_id, boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief Gets the string of the planet name as seen in the CRC Tables
    *   @returns string of the planet the waypoint is located
    */
    const std::string& GetPlanet();
	const std::string& GetPlanet(boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief sets the current waypoint's planet
    *   @param planet_name the planet to set the string to
    */
    void SetPlanet(const std::string& planet_name);
	void SetPlanet(const std::string& planet_name, boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief gets the waypoint's Unicode name
    *   @returns name of the waypoint in Unicode
    */
    const std::wstring& GetName();
	const std::wstring& GetName(boost::unique_lock<boost::mutex>& lock);

    std::string GetNameStandard();
	std::string GetNameStandard(boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief sets the waypoint's Unicode name
    *   @param name of the waypoint in Unicode
    */
    void SetName(const std::wstring& name);
	void SetName(const std::wstring& name, boost::unique_lock<boost::mutex>& lock);

    /**
    * @brief gets the waypoint's current color as a Byte
    *
    * Possible Options: 1 - blue,2 - green,3 - orange,4 - yellow,5 - purple/red,6 - white,7 - JTL
    * @returns Byte color of the waypoint
    */
    WaypointColor GetColor();
    WaypointColor GetColor(boost::unique_lock<boost::mutex>& lock);

    std::string GetColorString();
    std::string GetColorString(boost::unique_lock<boost::mutex>& lock);
    
    /**
    * @brief sets the waypoint's color byte
    *
    * Possible Options: 1 - blue,2 - green,3 - orange,4 - yellow,5 - purple/red,6 - white,7 - JTL
    * @param color_byte to set the waypoint
    */
    void SetColor(WaypointColor color_byte);
    void SetColor(WaypointColor color_byte, boost::unique_lock<boost::mutex>& lock);
    
    typedef swganh::ValueEvent<std::shared_ptr<Waypoint>> WaypointEvent;

private:

    glm::vec3 coordinates_; //update 3
    uint8_t activated_flag_; //update 3
	uint64_t location_network_id_; //update 3
    std::string planet_name_; //update 3
    std::wstring name_; //update 3
    std::atomic<uint8_t> color_; //update 3 
};

}} // swganh::object
