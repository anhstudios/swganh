// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace swganh 
{
namespace object
{
	class Object;
}

namespace sui
{

	/**
		@brief An enum of input types that can be subscribed to
	*/
	enum InputTrigger : uint8_t
	{
		TRIGGER_UPDATE = 4,
		TRIGGER_OK = 9,
		TRIGGER_CANCEL = 10
	};

	typedef std::function<bool(std::shared_ptr<swganh::object::Object>,
							uint32_t, std::vector<std::wstring>)> WindowCallbackFunction;

	class SUIWindowInterface : public std::enable_shared_from_this<SUIWindowInterface>
	{
	public:
		
		struct SUI_WINDOW_COMPONENT
		{
			uint8_t type;
			std::vector<std::wstring> wide_params;
			std::vector<std::string> narrow_params;
		};

		SUIWindowInterface(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0)
			: script_name_(script_name)
			, owner_(owner)
			, ranged_object_(ranged_object)
			, max_distance_(max_distance)
		{
		}

		/*
			@brief Clears a data source by name.
		*/
		virtual std::shared_ptr<SUIWindowInterface> ClearDataSource(std::string name) = 0;

		//02 -- AddChildWidget (Unreversed)

		/**
			@brief Sets a property
			@param location_string FullPath:Name
			@param value a unicode string of the value
		*/
		virtual std::shared_ptr<SUIWindowInterface> SetProperty(std::string location_string, std::wstring value) = 0;

		/**
			@brief Adds a data item
			@location location to add the new data item
			@value the default value
		*/
		virtual std::shared_ptr<SUIWindowInterface> AddProperty(std::string location_string, std::wstring value) = 0;

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
			InputTrigger trigger, 
			std::vector<std::string> returned_properties, 
			WindowCallbackFunction callbackFunction) = 0;

		/**
			@brief Adds a data source container
			@param location_string the location
			@param value to set for widget property
		*/
		virtual std::shared_ptr<SUIWindowInterface> AddDataSourceContainer(std::string location_string, std::wstring value) = 0;

		//07 -- Docs say not implemented in client

		/**
			@brief Clears internal data components
		*/
		void ClearComponents()
		{
			components_.clear();
		}
		
		/**
			@brief Returns internal data compoents -- used by sui service
		*/
		std::vector<SUI_WINDOW_COMPONENT>& GetComponents()
		{
			return components_;
		}

		/**
			@brief Returns the script name
		*/
		const std::string GetScriptName() const
		{
			return script_name_;
		}

		/**
			@brief Returns the owner
		*/
		std::shared_ptr<swganh::object::Object> GetOwner()
		{
			return owner_;
		}

		/**
			@brief Returns the ranged object
		*/
		std::shared_ptr<swganh::object::Object> GetRangedObject()
		{
			return ranged_object_;
		}

		/**
			@brief Sets the ranged object
			@param object the ranged object
		*/
		void SetRangedObject(std::shared_ptr<swganh::object::Object> object)
		{
			ranged_object_ = object;
		}

		/**
			@brief Returns the max distance
		*/
		float GetMaxDistance()
		{
			return max_distance_;
		}

		/**
			@brief Sets the max distance
			@param max_distance the new max distance
		*/
		void SetMaxDistance(float max_distance) 
		{
			max_distance_ = max_distance;
		}

		/**
			@brief Returns this window's id
		*/
		int32_t GetWindowId()
		{
			return window_id_;
		}

		/**
			@brief Sets the window's id. For internal use only.
		*/
		void SetWindowId(int32_t id)
		{
			window_id_ = id;
		}
		
		virtual WindowCallbackFunction GetFunctionById(uint32_t event_id) = 0;

	protected:
		std::vector<SUI_WINDOW_COMPONENT> components_;
		int32_t window_id_;
		std::string script_name_;
		std::shared_ptr<swganh::object::Object> owner_;
		std::shared_ptr<swganh::object::Object> ranged_object_; 
		float max_distance_;
	};

}
}
