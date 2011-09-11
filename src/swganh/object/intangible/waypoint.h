#ifndef SWGANH_OBJECT_INTAGIBLE_WAYPOINT_H_
#define SWGANH_OBJECT_INTAGIBLE_WAYPOINT_H_

#include "swganh/object/intangible.h"
#include <glm\glm.hpp>

namespace swganh {
namespace object {
namespace intangible {

class Waypoint : public swganh::object::Intangible {

public:
	uint32_t GetUses() { return uses_; }
	void SetUses(uint32_t uses) 
	{
		uses_ = uses;

		// Only build a message if there are observers.
        if (scene->HasObservers(object_id_))
        {

		}
	}

private:
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