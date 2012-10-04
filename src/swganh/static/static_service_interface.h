#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "swganh/service/service_interface.h"



namespace swganh
{
namespace statics
{
	struct ElevatorData
	{
		uint64_t dst_cell;
		glm::quat dst_orientation;
		glm::vec3 dst_position;
		uint32_t effect_id;
		bool going_down;
	};

	class StaticServiceInterface : public swganh::service::ServiceInterface
	{
	public:
		virtual std::vector<std::shared_ptr<ElevatorData>> GetElevatorDataForObject(uint64_t terminal_id) = 0;
	};
}
}