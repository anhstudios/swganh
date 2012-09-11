#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "swganh/sui/sui_window_interface.h"

namespace swganh 
{
namespace sui
{

	typedef std::map<uint32_t, swganh::sui::WindowCallbackFunction> CallbackMap;

	class SUIWindow : public swganh::sui::SUIWindowInterface
	{
	public:

		SUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0);

		/*
			@brief Clears a data source by name.
		*/
		virtual std::shared_ptr<SUIWindowInterface> ClearDataSource(std::string name);

		//02 -- AddChildWidget (Unreversed)

		/**
			@brief Sets a property
			@param location_string FullPath:Name
			@param value a unicode string of the value
		*/
		virtual std::shared_ptr<SUIWindowInterface> SetProperty(std::string location_string, std::wstring value);

		/**
			@brief Adds a data item
			@location location to add the new data item
			@value the default value
		*/
		virtual std::shared_ptr<SUIWindowInterface> AddProperty(std::string location_string, std::wstring value);

		/**
			@brief Subscribes to an event in the client. This is used to return data from the window
			@param callbackName the name of the callback to add
			@param trigger the input trigger to respond to
			@param returned_properties the properties to return
			@param callbackFunction the function to be called when this event takes place
		*/
		virtual std::shared_ptr<SUIWindowInterface> SubscribeToEventCallback( 
			uint32_t event_id,
			std::string event_source,
			swganh::sui::InputTrigger trigger, 
			std::vector<std::string> returned_properties, 
			swganh::sui::WindowCallbackFunction callbackFunction);

		/**
			@brief Adds a data source container
			@param location_string the location
			@param value to set for widget property
		*/
		virtual std::shared_ptr<SUIWindowInterface> AddDataSourceContainer(std::string location_string, std::wstring value);

		//07 -- Docs say not implemented in client

		virtual swganh::sui::WindowCallbackFunction GetFunctionById(uint32_t event_id);

	private:
		CallbackMap callbacks_;
	};

}
}
