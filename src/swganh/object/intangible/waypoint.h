#ifndef SWGANH_OBJECT_INTAGIBLE_WAYPOINT_H_
#define SWGANH_OBJECT_INTAGIBLE_WAYPOINT_H_

#include "swganh/object/intangible.h"
#include <glm/glm.hpp>

namespace swganh {
namespace object {
namespace intangible {

enum WaypointStatus
{
    DEACTIVATED = 0,
    ACTIVATED = 1
};

/**
*  @brief Object that defines the data in a Waypoint
*/
class Waypoint : public swganh::object::Intangible {

public:
    virtual uint32_t GetType() { return Waypoint::type; }
    const static uint32_t type = 0x57415950;

    Waypoint();
    Waypoint(glm::vec3 coordinates, bool activated, const std::string& planet, const std::wstring& name, const std::string& color);

    uint32_t GetUses() const { return uses_; }
	void SetUses(uint32_t uses);

    glm::vec3 GetCoordinates() const { return coordinates_; }
    void SetCoordinates(const glm::vec3& coords);
    void SetCoordinates(float x, float y, float z);

    bool Active() const { return activated_flag_ == 1; }
    void Activate();
    void DeActivate();

    const std::string& GetPlanet() { return planet_name_; }
    void SetPlanet(const std::string& planet_name);

    const std::wstring& GetName() { return name_; }
    void SetName(const std::wstring& name);

    const std::string& GetColor() { return color_; }
    void SetColor(const std::string& color);

private:
    virtual void OnReliableUpdate() {
        Intangible::OnReliableUpdate();
    }

    uint32_t uses_;					//update 3
	glm::vec3 coordinates_;			//update 3
	uint8_t activated_flag_;		//update 3
	uint64_t location_network_id_;	//update 3
	std::string planet_name_;		//update 3
	std::wstring name_;				//update 3
	uint8_t not_used_;				//update 3
	std::string color_;				//update 3
};

}}} // swganh::object::intangible


#endif //SWGANH_OBJECT_INTAGIBLE_WAYPOINT_H_