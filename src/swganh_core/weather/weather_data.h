// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
namespace swganh {
namespace weather {


	enum Weather : uint32_t
	{
		NOSTORM,
		CLOUDY,
		LIGHTSTORM,
		MEDIUMSTORM,
		HEAVYSTORM
	};

	enum Scene : uint32_t
    {
        CORELLIA = 1,
        DANTOOINE,
        DATHOMIR,
        ENDOR,
        LOK,
        NABOO,
        RORI,
        TALUS,
        TATOOINE,
        YAVIN4
    };

	class WeatherEvent
    {
    public:
        WeatherEvent(long minutes, Weather weather, glm::vec3 vector);

        long GetDuration();
        void SetDuration(long minutes);

        Weather GetWeatherType();
        void SetWeatherType(Weather weather);

        glm::vec3 GetCloudVector();
        void SetCloudVector(glm::vec3 vector);

        bool operator==(WeatherEvent other)
        {
            return weather_type == other.weather_type;
        }
		bool operator<(WeatherEvent other)
		{
			return weather_type < other.weather_type;
		}

    private:
        long duration;
        Weather weather_type;
        glm::vec3 cloud_vector;
    };

	struct WeatherData 
	{
		int32_t duration;
		Weather weather_;
		glm::vec3 cloud_vector;
	};
}
}
