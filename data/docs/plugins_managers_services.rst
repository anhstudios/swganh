
Plugins, Managers, and Services - WIP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~======

Before we dive into any one of these specific systems, lets do a quick overview of each one, then we will talk about how they interact and how do you know when you should use each.

Services - Intro
================
Services are systems that are available the entire lifetime of the application. They also generally handle interactions directly with the SWG Protocol and they expose
an API for controlling or accessing data that covers a feature set that can be used by all other services.
Services are generally broken up into game system categories, but they can also be more generic than that. For example there is a Login Service that handles
all of the interaction between the Client Login Messages, the database and server itself. This service handles these messages and allow you to do what is necessary with the data.
In most cases, this involves interacting with the database and sending a SWG Protocol Message reply back to the Client. Most services follow this basic pattern, but they are not restricted to it.

Managers - Intro
================
Managers are systems that are all about controlling or managing data and interacting with plugin interfaces to do their job.
Managers make use of events and subscribe to events that are often triggered in the services themselves.
They are in places where the systems are more complex, for example look at SceneManager and MovementManager within the SimulationService.

Plugins - Intro
================
In order to add a large amount of customizability and flexibility within the SWGANH server. The developers have decided to utilize the idea of loading in objects dynamically.
This allows custom plugins to be created that tweaks functionality, utilizes a different data source, and more just with a simple tweak in the configuration file.
The plugin system provides a way to register how to create and destroy objects and then it uses registrations to create new objects on the fly.
This registration is then available to the server at runtime and allows a quick and easy method to get objects as needed.

