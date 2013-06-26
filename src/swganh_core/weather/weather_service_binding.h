// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_WEATHER_SERVICE_BINDING_H_
#define SWGANH_WEATHER_SERVICE_BINDING_H_

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "weather_service_interface.h"
#include "weather_data.h"
#include <boost/python.hpp>

using namespace swganh::weather;
using namespace swganh::weather;
using namespace boost::python;
using namespace std;

void exportSocialService()
{

	enum_<Weather>("WEATHER", "Weather types available in :class:`.WeatherService`")
		.value("NOSTORM", NOSTORM)
        .value("CLOUDY", CLOUDY)
        .value("LIGHTSTORM", LIGHTSTORM)
        .value("MEDIUMSTORM", MEDIUMSTORM)
        .value("HEAVYSTORM", HEAVYSTORM)
		;

    enum_<Scene>("SCENE", "Scenes available in :class:`.WeatherService`")
        .value("CORELLIA", CORELLIA)
        .value("DANTOOINE", DANTOOINE)
        .value("DATHOMIR", DATHOMIR)
        .value("ENDOR", ENDOR)
        .value("LOK", LOK)
        .value("NABOO", NABOO)
        .value("RORI", RORI)
        .value("TALUS", TALUS)
        .value("TATOOINE", TATOOINE)
        .value("YAVIN4", YAVIN4)
        ;

	class_<WeatherServiceInterface, shared_ptr<WeatherServiceInterface>, boost::noncopyable>("WeatherService", "The weather service processes in-game weather features.", no_init)
        .def("getSceneWeather", &WeatherServiceInterface::GetSceneWeather, "Returns the current weather ID from a specified scene.")
        .def("setSceneWeather", &WeatherServiceInterface::SetSceneWeather, "Sets the weather ID on a specified scene and broadcasts a ServerWeatherMessage to all players on that scene.")
        ;

	class_<std::vector<WeatherEvent> >("WeatherSequence", "A vector for WeatherEvent objects which contain duration, weather type, and cloud vector.")
        .def(vector_indexing_suite<std::vector<WeatherEvent>, true>())
        ;

	class_<WeatherEvent>("WeatherEvent", "Contains the duration, weather type, and cloud vector for a weaather event.", init<long, Weather, glm::vec3>())
        .def("getDuration", &WeatherEvent::GetDuration, "Gets the duration.")
        .def("setDuration", &WeatherEvent::SetDuration, "Sets the duration.")
        .def("getWeatherType", &WeatherEvent::GetWeatherType, "Gets the weather type.")
        .def("setWeatherType", &WeatherEvent::SetWeatherType, "Sets the weather type.")
        .def("getCloudVector", &WeatherEvent::GetCloudVector, "Gets the cloud vector.")
        .def("setCloudVector", &WeatherEvent::SetCloudVector, "Sets the cloud vector.")
        .add_property("duration", &WeatherEvent::GetDuration, &WeatherEvent::SetDuration, "Gets and sets the duration.")
        .add_property("weatherType", &WeatherEvent::GetWeatherType, &WeatherEvent::SetWeatherType, "Gets and sets the weather type.")
        .add_property("cloudVector", &WeatherEvent::GetCloudVector, &WeatherEvent::SetCloudVector, "Gets and sets the cloud vector.")
        ;

}

#endif //SWGANH_WEATHER_SERVICE_BINDING_H_