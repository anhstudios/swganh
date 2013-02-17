// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/service/service_interface.h"
#include "weather_data.h"
#include "swganh/app/swganh_kernel.h"
#include <boost/asio.hpp>
namespace swganh{
namespace weather{

}
}

namespace swganh {
namespace weather {
class WeatherProviderInterface;
}} // namespace swganh::character

namespace swganh {
namespace weather {
    
    class WeatherServiceInterface : public swganh::service::ServiceInterface
    {
    public:

        virtual Weather GetSceneWeather(uint32_t scene_id) =0;
		virtual void SetSceneWeather(uint32_t scene_id, std::vector<WeatherEvent> weather_sequence) =0;
    };

}}  // namespace swganh::weather
