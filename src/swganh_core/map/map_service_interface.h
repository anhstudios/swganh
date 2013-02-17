// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh {
namespace map {

	/**
	 * Manages map locations for each scene.
	 */
	class MapServiceInterface : public swganh::service::ServiceInterface
	{
	public:

		/**
		 * Adds a location to a scene by its name.
		 *
		 * @param scene The scene you want to add the location to.
		 * @param name The name of the location you want to add.
		 * @param x The x position of the location.
		 * @param z The z poisition of the location.
		 * @param category The category of the location.
		 * @param sub_category The sub-category of the location.
		 */
		virtual void AddLocation(std::string scene, std::wstring name, float x, float z, uint32_t category, uint32_t sub_category)=0;
		
		/**
		 * Removes a location from a scene by its name.
		 *
		 * @param scene The scene you want to remove the location from.
		 * @param name The name of the location you want to remove.
		 */
		virtual void RemoveLocation(std::string scene, std::wstring name)=0;

		/**
		 * @param scene The scene the location exists in.
		 * @param name The name of the location you want to verify.
		 * @returns Verfies the existance of the location by name.
		 */
		virtual bool LocationExists(std::string scene, std::wstring name)=0;

		/**
		 * Adds a location to a scene by its id.
		 *
		 * @param scene_id The id of the scene you want to add the location to.
		 * @param name The name of the location you want to add.
		 * @param x The x position of the location.
		 * @param z The z poisition of the location.
		 * @param category The category of the location.
		 * @param sub_category The sub-category of the location.
		 */
		virtual void AddLocation(uint32_t scene_id, std::wstring name, float x, float z, uint32_t category, uint32_t sub_category)=0;

		/**
		 * Removes a location from a scene by its id.
		 *
		 * @param scene_id The id of the scene you want to remove the location from.
		 * @param name The name of the location you want to remove.
		 */
		virtual void RemoveLocation(uint32_t scene_id, std::wstring name)=0;

		/**
		 * @param scene_id The id of the scene the location exists in.
		 * @param name The name of the location you want to verify.
		 * @returns Verfies the existance of the location by id.
		 */
		virtual bool LocationExists(uint32_t scene_id, std::wstring name)=0;
	};

}} // swganh::map