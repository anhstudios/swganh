// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh {
namespace map {

	class MapServiceInterface : public swganh::service::ServiceInterface
	{
	public:
		virtual void AddLocation(std::string scene, std::wstring name, float x, float z, uint32_t category, uint32_t sub_category)=0;
		virtual void RemoveLocation(std::string scene, std::wstring name)=0;
		virtual bool LocationExists(std::string scene, std::wstring name)=0;

		virtual void AddLocation(uint32_t scene_id, std::wstring name, float x, float z, uint32_t category, uint32_t sub_category)=0;
		virtual void RemoveLocation(uint32_t scene_id, std::wstring name)=0;
		virtual bool LocationExists(uint32_t scene_id, std::wstring name)=0;
	};

}} // swganh::map