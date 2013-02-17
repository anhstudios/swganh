// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/weather/weather_data.h"

using namespace swganh::weather;

WeatherEvent::WeatherEvent(long minutes, Weather weather, glm::vec3 vector)
{
    duration = minutes;
    weather_type = weather;
    cloud_vector.x = vector.x;
    cloud_vector.y = vector.y;
    cloud_vector.z = vector.z;
}

long WeatherEvent::GetDuration() { return duration; }
void WeatherEvent::SetDuration(long minutes)
{
    duration = minutes;
}

Weather WeatherEvent::GetWeatherType() { return weather_type; }
void WeatherEvent::SetWeatherType(Weather weather)
{
    weather_type = weather;
}

glm::vec3 WeatherEvent::GetCloudVector() { return cloud_vector; }
void WeatherEvent::SetCloudVector(glm::vec3 vector)
{
    cloud_vector.x = vector.x;
    cloud_vector.y = vector.y;
    cloud_vector.z = vector.z;
}