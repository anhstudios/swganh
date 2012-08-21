# swgpy
from swgpy import app, utility, weather
from swgpy.weather import weather_event, weather_sequence

# modules
import random
service_mgr = kernel.service_manager()
weather_svc = service_mgr.weather_service()

#This script is called every 30 minutes.
#As no error check is in place on server, if this script fails it WILL crash the server.
#Weather duration is now set in minutes: weather_event(duration,weatherType,cloudVector).

lightStormSequence = weather_sequence()
lightStormSequence.extend([
	weather_event(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0))
])

mediumStormSequence = weather_sequence()
mediumStormSequence.extend([
	weather_event(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
        weather_event(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)),
        weather_event(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0))
])

heavyStormSequence = weather_sequence()
heavyStormSequence.extend([
	weather_event(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
        weather_event(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(5, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(20, weather.WEATHER.HEAVYSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(5, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)),
        weather_event(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)),
	weather_event(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0))
])

#Crude random function with weight. Needs improving.
def weather_choice(wList, scene):
    n = random.uniform(0, 1)
    for item, weight in wList:
        if n < weight:
            weather_svc.set_scene_weather(scene, item)
            break


weather_choice([(heavyStormSequence, 0.02), (mediumStormSequence, 0.15), (lightStormSequence,0.2)],weather.SCENE.CORELLIA)
weather_choice([(heavyStormSequence, 0.02), (mediumStormSequence, 0.15), (lightStormSequence,0.2)],weather.SCENE.NABOO)
