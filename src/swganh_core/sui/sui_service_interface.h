// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <swganh/service/service_interface.h>


namespace swganh
{
namespace object
{
	class Object;
}
namespace sui
{
	class SUIWindowInterface;

	enum MessageBoxType
	{
		MESSAGE_BOX_OK = 1,
		MESSAGE_BOX_OK_CANCEL,
		MESSAGE_BOX_YES_NO
	};

	enum ListBoxType
	{
		LIST_BOX_OK = 1,
		LIST_BOX_OKCANCEL
	};

	enum InputBoxType
	{
		INPUT_BOX_OK = 1,
		INPUT_BOX_OKCANCEL
	};

	class SUIServiceInterface : public swganh::service::BaseService
	{
	public:
        virtual ~SUIServiceInterface() {}

		//Creates a new SUI page and returns the id of the corresponding window id
		virtual int32_t OpenSUIWindow(std::shared_ptr<SUIWindowInterface> window) = 0;

		//UpdateWindow
		virtual int32_t UpdateSUIWindow(std::shared_ptr<SUIWindowInterface> window) = 0;

		//Get Window
		virtual std::shared_ptr<SUIWindowInterface> GetSUIWindowById(std::shared_ptr<swganh::object::Object> owner, int32_t windowId) = 0;

		virtual std::shared_ptr<SUIWindowInterface> GetSUIWindowByScriptName(std::shared_ptr<swganh::object::Object> owner, std::string script) = 0;

		//Forcefully closes a previously opened window.
		virtual void CloseSUIWindow(std::shared_ptr<swganh::object::Object> owner, int32_t windowId) = 0;

		//Custom SUI Window creator
		virtual std::shared_ptr<SUIWindowInterface> CreateSUIWindow(std::string script_name, std::shared_ptr<swganh::object::Object> owner, 
							std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0) = 0;


		/// PRE WRITTEN SUI WINDOWS ///
		virtual std::shared_ptr<SUIWindowInterface> CreateMessageBox(MessageBoxType msgBox_type, std::wstring title, std::wstring caption,
			std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> ranged_object = nullptr, 
			float max_distance = 0) = 0;

		virtual std::shared_ptr<SUIWindowInterface> CreateListBox(ListBoxType lstBox_type, std::wstring title, std::wstring prompt, 
			std::vector<std::wstring> dataList, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0) = 0;
			
		virtual std::shared_ptr<SUIWindowInterface> CreateInputBox(InputBoxType iptBox_type, std::wstring title, std::wstring prompt, 
			uint32_t input_max_length, std::shared_ptr<swganh::object::Object> owner, 
			std::shared_ptr<swganh::object::Object> ranged_object = nullptr, float max_distance = 0) = 0;
	};
}
}
