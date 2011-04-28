/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBANH_COMPONENT_ENTITY_BUILDER
#define LIBANH_COMPONENT_ENTITY_BUILDER

#include <functional>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <anh/component/component_interface.h>
#include <anh/component/component_loader_interface.h>
#include <anh/component/entity_manager.h>

namespace anh {
namespace component {

typedef anh::HashString EntityType;
typedef std::function<std::shared_ptr<ComponentInterface>(const EntityId&)> ComponentCreator;

enum EntityBuildErrors {
    BUILD_INCOMPLETE = -1,  /// component doesn't have a registered creator, entity still builds
    BUILD_SUCCESSFUL = 0,   /// no problems object built succesfully without issues
    BUILD_FAILED = 1        /// object is not built
};

/**
 * @brief Uses XML templates to build entities and add them to the EntityManager.
 */
class EntityBuilder : public boost::noncopyable
{
public:
	/** 
	 * @brief Default constructor
	 */
	EntityBuilder(std::shared_ptr<EntityManager> entity_manager);

	/**
	 * @brief Default destructor
	 */
	~EntityBuilder();

	/**
	 * @brief loads all template xml files in the given directory
	 */
	void Init(std::string object_template_dir);

	/**
	 * @brief Deinitializes
	 */
	void Deinit(void);

	/**
	 * @brief Builds an Entity from an XML template, invoking creators and loaders and
	 * adds it to the EntityManager.
     *
     * @param id Id of the entity that is being built.
     * @param type The template to build for this object.
	 * @param name The name of the new object.
     *
     * @returns see @EntityBuildErrors
	 */
    EntityBuildErrors BuildEntity(const EntityId& entity_id, const EntityType& type, const std::string& name);

	/**
	 * @brief Registers a loader that will be called on @BuildEntity
     *
     * @param type the template type to associate this to (type is usually set in the concrete implementation)
     * @param loader the ComponentLoader class associated to the component
     *
     * @returns bool true if the Loader was properly registered.
	 */
	bool RegisterLoader(const ComponentType& type, std::shared_ptr<ComponentLoaderInterface> loader);

	/**
	 * @brief Unregisters a loader for a given ComponentType
     *
     * @param type the template type to remove the associate from
     *
	 */
	void UnregisterLoader(const ComponentType& type);

	/**
	 * @brief Registers a creator that will be called on @BuildEntity
     *
     * @param type the template type to associate this to (type is usually set in the concrete implementation)
     * @param creator the ComponentCreator to create this component
     *
     * @returns bool true if the Loader was properly registered.
	 */
	bool RegisterCreator(const ComponentType& type, ComponentCreator creator);

	/**
	 * @brief UnRegisters the creator
     *
     * @param type the template type to remove association
     *
	 */
	void UnregisterCreator(const ComponentType& type);
	
	/**
	 * @brief Checks to see if the given Template exists
     *
     * @param type the EntityType to see if a template exists for it
     *
     * @returns bool true if the template was found
	 */
	bool TemplateExists(const EntityType& type);

	/**
	 * @brief Checks to see if a creator exists for the given type
     *
     * @param type the ComponentType
     *
     * @returns bool true if the Creator exists for the given type
	 */
	bool CreatorExists(const ComponentType& type);

	/**
	 * @brief Checks to see if a loader exists for the given type
     *
     * @param type the ComponentType
     *
     * @returns bool true if the Loader exists for the given type
	 */
	bool LoaderExists(const ComponentType& type);

protected:
private:

	void LoadTemplates_(const boost::filesystem::path p);

	typedef std::map<ComponentType, std::shared_ptr<ComponentLoaderInterface>>		ComponentLoaders;
	typedef std::pair<ComponentType, std::shared_ptr<ComponentLoaderInterface>>		ComponentLoaderPair;
	typedef std::map<EntityType, boost::property_tree::ptree>						EntityTemplates;
	typedef std::pair<EntityType, boost::property_tree::ptree>						EntityTemplatePair;
	typedef std::map<ComponentType, ComponentCreator>								ComponentCreators;
	typedef std::pair<ComponentType, ComponentCreator>								ComponentCreatorPair;
	typedef std::map<EntityType, TagSet>											EntityTagSets;
	typedef std::pair<EntityType, TagSet>											EntityTagSetPair;
    // map holding the component loaders and their implementation
	ComponentLoaders	component_loaders_;
    // map holding the entity templates and their xml stored in property tree
	EntityTemplates		entity_templates_;
    // map holding the component creators and their implmentation
	ComponentCreators	component_creators_;
    // map holding the TagSet for a given EntityType
	EntityTagSets		entity_tag_sets_;
	
	std::shared_ptr<EntityManager>	entity_manager_;
	
};

} // component
} // anh

#endif // ANH_COMPONENT_ENTITY_BUILDER_H_