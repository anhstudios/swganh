#pragma once

#include "swganh/app/swganh_kernel.h"
#include "swganh/static/static_service_interface.h"

namespace swganh
{
namespace statics
{
	class StaticService : public swganh::statics::StaticServiceInterface
	{
	public:

		StaticService(swganh::app::SwganhKernel* kernel);
		~StaticService();

		virtual swganh::service::ServiceDescription GetServiceDescription();

	private:
		swganh::app::SwganhKernel* kernel_;
		std::vector<std::string> attributes_;
	};
}
}