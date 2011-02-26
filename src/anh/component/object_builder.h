/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

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

#ifndef LIBANH_COMPONENT_OBJECT_BUILDER
#define LIBANH_COMPONENT_OBJECT_BUILDER

#include <functional>
#include <map>
#include <Utils/Singleton.h>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <anh/component/component_interface.h>
#include <anh/component/component_loader_interface.h>

namespace anh {
namespace component {

class ObjectBuilder;
#define gObjectBuilder utils::Singleton<ObjectBuilder>::Instance()

typedef anh::HashString ObjectType;
typedef std::function<std::shared_ptr<ComponentInterface>(const ObjectId&)> ComponentCreator;

enum OBJECT_BUILD_STATUS {
    BUILD_INCOMPLETE = -1,  // component doesn't have a registered creator, object still builds
    BUILD_SUCCESSFUL = 0,   // no problems object built succesfully without issues
    BUILD_FAILED = 1        // object is not built
};

/**
 * \brief This class handles the wiring and attachment of components to an entity id
 */
class ObjectBuilder
{
public:
	/** 
	 * \brief Default constructor
	 */
	ObjectBuilder();

	/**
	 * \brief Default destructor
	 */
	~ObjectBuilder();

	/**
	 * \brief loads all template xml files in the given directory
	 */
	void Init(std::string object_template_dir);

	/**
	 * \brief 
	 */
	void Deinit(void);

	/**
	 * \brief Builds the Object given an id and hashstring Type
     *
     * \param ObjectId the id of the object to be built
     * \param ObjectType the hashstring of the type to be built
     *
     * \returns see @OBJECT_BUILD_STATUS
	 */
    OBJECT_BUILD_STATUS BuildObject(const ObjectId& id, const ObjectType& type);

	/**
	 * \brief
	 */
	void DestroyObject(const ObjectId& id);

	/**
	 * \brief
	 */
	bool RegisterLoader(const ComponentType& type, std::shared_ptr<ComponentLoaderInterface> loader);

	/**
	 * \brief
	 */
	void UnregisterLoader(const ComponentType& type);

	/**
	 * \brief
	 */
	bool RegisterCreator(const ComponentType& type, ComponentCreator creator);

	/**
	 * \brief
	 */
	void UnregisterCreator(const ComponentType& type);
	
	/**
	 * \brief
	 */
	bool TemplateExists(const ObjectType& type);

	/**
	 * \brief
	 */
	bool CreatorExists(const ComponentType& type);

	/**
	 * \brief
	 */
	bool LoaderExists(const ComponentType& type);

protected:
private:

	void LoadTemplates_(const boost::filesystem::path p);

	typedef std::map<ComponentType, std::shared_ptr<ComponentLoaderInterface>>		ComponentLoaders;
	typedef std::pair<ComponentType, std::shared_ptr<ComponentLoaderInterface>>		ComponentLoaderPair;
	typedef std::map<ObjectType, boost::property_tree::ptree>						ObjectTemplates;
	typedef std::pair<ObjectType, boost::property_tree::ptree>						ObjectTemplatePair;
	typedef std::map<ComponentType, ComponentCreator>								ComponentCreators;
	typedef std::pair<ComponentType, ComponentCreator>								ComponentCreatorPair;

	ComponentLoaders	component_loaders_;
	ObjectTemplates		object_templates_;
	ComponentCreators	component_creators_;
	
};

}
}

#endif