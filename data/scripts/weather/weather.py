# swgpy
from swgpy import app, utility, weather
from swgpy.weather import weather_event, weather_sequence

# modules
import random
service_mgr = kernel.service_manager()
weather_svc = service_mgr.weather_service()

#This script is called every 30 minutes.
#Weather duration is now set in minutes: weather_event(duration(mins),weatherType,cloudVector(X,Y,Z)).
#Each of the following is a list of weather sequences. Each sequence runs until its duration has expired.
#At the end of the sequence, NOSTORM is used to set the weather back to clear. Whilst each weather sequence list
#is running, no other weather sequence can be used on the same scene until the sequences have expired.
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
