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

#include <glog/logging.h>
#include <anh/component/object_builder.h>
#include <anh/component/object_manager.h>

namespace anh {
namespace component {

ObjectBuilder::ObjectBuilder()
{
}

ObjectBuilder::~ObjectBuilder()
{
	object_templates_.empty();
	component_loaders_.empty();
	component_creators_.empty();
}

void ObjectBuilder::Init(std::string object_template_dir)
{
	boost::filesystem::path templates_path(object_template_dir);
	if(!boost::filesystem::exists(templates_path))
		throw std::runtime_error("Object Template Directory Not Found!");

	LoadTemplates_(templates_path);
}

void ObjectBuilder::Deinit(void)
{
}

OBJECT_BUILD_STATUS ObjectBuilder::BuildObject(const ObjectId& id, const ObjectType& type)
{
    OBJECT_BUILD_STATUS status = BUILD_SUCCESSFUL;
    bool any_attached = false;
    
	ObjectTemplates::iterator i = object_templates_.find(type);

	if(i == object_templates_.end())
        return BUILD_FAILED;

	boost::property_tree::ptree pt = (*i).second;
	for(boost::property_tree::ptree::iterator xml_component = pt.get_child("object").begin(); xml_component != pt.get_child("object").end(); xml_component++) {
		if(xml_component->first.compare("component") != 0)
			continue;

		// Grab the component type.
		ComponentType type(xml_component->second.get<std::string>("<xmlattr>.type").c_str());

		// Search for the components creator function by the type we extracted from xml.
		ComponentCreators::iterator creators_iter = component_creators_.find(type);
		if(creators_iter == component_creators_.end()) {
            status = BUILD_INCOMPLETE;
			continue;
        }

		std::shared_ptr<ComponentInterface> component = (*creators_iter).second(id);
		component->Init(xml_component->second);

		// Search for a component loader, if it exists, call it.
		ComponentLoaders::iterator loader_iter = component_loaders_.find(type);
		if(loader_iter != component_loaders_.end())
			(*loader_iter).second->Load(component);
		
		gObjectManager.AttachComponent(id,component);
        any_attached = true;
    }
    // if the status is incomplete and no components were attached, then we fail
    if (status == BUILD_INCOMPLETE && !any_attached)
        status = BUILD_FAILED;
	return status;
}

void ObjectBuilder::DestroyObject(const ObjectId& id)
{
    gObjectManager.DetatchAllComponents(id);
}

bool ObjectBuilder::RegisterLoader(const ComponentType& type, std::shared_ptr<ComponentLoaderInterface> loader)
{
	ComponentLoaders::iterator i = component_loaders_.find(type);
	if(i != component_loaders_.end()) {
		return false;
	}

	component_loaders_.insert(ComponentLoaderPair(type, loader));
	return true;
}

void ObjectBuilder::UnregisterLoader(const ComponentType& type)
{
	ComponentLoaders::iterator i = component_loaders_.find(type);
	if(i != component_loaders_.end())
		component_loaders_.erase(i);
}

bool ObjectBuilder::RegisterCreator(const ComponentType& type, ComponentCreator creator)
{
	ComponentCreators::iterator i = component_creators_.find(type);
	if(i != component_creators_.end()) {
		return false;
	}

	component_creators_.insert(ComponentCreatorPair(type, creator));
	return true;
}

void ObjectBuilder::UnregisterCreator(const ComponentType& type)
{
	ComponentCreators::iterator i = component_creators_.find(type);
	if(i != component_creators_.end())
		component_creators_.erase(i);
}

bool ObjectBuilder::TemplateExists(const ObjectType& type)
{
	ObjectTemplates::iterator i = object_templates_.find(type);
	if(i == object_templates_.end())
		return false;
	else
		return true;
}

bool ObjectBuilder::CreatorExists(const ComponentType& type)
{
	ComponentCreators::iterator i = component_creators_.find(type);
	if(i == component_creators_.end())
		return false;
	else
		return true;
}

bool ObjectBuilder::LoaderExists(const ComponentType& type)
{
	ComponentLoaders::iterator i = component_loaders_.find(type);
	if(i == component_loaders_.end())
		return false;
	else
		return true;
}

void ObjectBuilder::LoadTemplates_(const boost::filesystem::path p)
{
	boost::filesystem::directory_iterator end_iter;
	for(boost::filesystem::directory_iterator iter(p); iter != end_iter; ++iter)
	{
		if(boost::filesystem::is_directory(iter->status()))
			LoadTemplates_(iter->path());
		else if(boost::filesystem::is_regular_file(iter->status()))
		{
			boost::property_tree::ptree pt;
			boost::property_tree::read_xml(iter->string(), pt);
			object_templates_.insert(ObjectTemplatePair(ObjectType(pt.get<std::string>("object.<xmlattr>.name").c_str()),pt));
		}
	}
}

}
}