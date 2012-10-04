# swgpy
from swgpy import app, utility, weather
from swgpy.weather import WeatherEvent, WeatherSequence

# modules
import random
service_mgr = kernel.serviceManager()
weather_svc = service_mgr.weatherService()

#This script is called every 30 minutes.
#Weather duration is now set in minutes: weather_event(duration(mins),weatherType,cloudVector(X,Y,Z)).
#Each of the following is a list of weather sequences. Each sequence runs until its duration has expired.
#At the end of the sequence, NOSTORM is used to set the weather back to clear. Whilst each weather sequence list
#is running, no other weather sequence can be used on the same scene until the sequences have expired.
lightStormSequence = WeatherSequence()
lightStormSequence.append(WeatherEvent(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
lightStormSequence.append(WeatherEvent(10, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)))
lightStormSequence.append(WeatherEvent(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
lightStormSequence.append(WeatherEvent(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0)))

mediumStormSequence = WeatherSequence()
mediumStormSequence.append(WeatherEvent(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
mediumStormSequence.append(WeatherEvent(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)))
mediumStormSequence.append(WeatherEvent(10, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)))
mediumStormSequence.append(WeatherEvent(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)))
mediumStormSequence.append(WeatherEvent(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
mediumStormSequence.append(WeatherEvent(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0)))

heavyStormSequence = WeatherSequence()
heavyStormSequence.append(WeatherEvent(20, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(5, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(20, weather.WEATHER.HEAVYSTORM, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(5, weather.WEATHER.MEDIUMSTORM, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(3, weather.WEATHER.LIGHTSTORM, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(10, weather.WEATHER.CLOUDY, utility.vector3(1.0, 0.0, 0.0)))
heavyStormSequence.append(WeatherEvent(10, weather.WEATHER.NOSTORM, utility.vector3(1.0, 0.0, 0.0)))

#Crude random function with weight. Needs improving.
def weatherChoice(wList, scene):
    n = random.uniform(0, 1)
    for item, weight in wList:
        if n < weight:
            weather_svc.setSceneWeather(scene, item)
            break


weatherChoice([(heavyStormSequence, 0.02), (mediumStormSequence, 0.15), (lightStormSequence,0.2)],weather.SCENE.CORELLIA)
