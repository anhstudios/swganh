/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/
#ifndef ZONE_APPEARANCE_COMPONENT_H
#define ZONE_APPEARANCE_COMPONENT_H

#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>

#include <boost/flyweight.hpp>

using namespace anh::component;
namespace zone { namespace components {
class NullAppearanceComponent;

class AppearanceComponentInterface : public BaseComponent {
public:
    AppearanceComponentInterface(const ObjectId& id)
		: BaseComponent(id) { }
    virtual const int race() = 0;
    virtual const bool sex() = 0;
    virtual const std::string& model() = 0;
    virtual const std::string& name() = 0;
    virtual const std::string& name_file() = 0;
    virtual const std::string& detail_file() = 0;
    virtual const std::string& color() = 0;
    virtual const std::string& customization_string() = 0;
    virtual std::map<std::string, uint16_t> customization_map() = 0;
    static std::shared_ptr<NullAppearanceComponent> NullComponent;
};

class NullAppearanceComponent : AppearanceComponentInterface {
public:
    NullAppearanceComponent()
		: AppearanceComponentInterface(0) { }
    const int race() { return race_; }
    const bool sex() { return gender_; }
    const std::string& model() { return model_; }
    const std::string& name() { return name_; }
    const std::string& name_file() { return name_file_; }
    const std::string& detail_file() { return detail_file_; }
    const std::string& color() { return color_; }
    const std::string& customization_string() { return customization_string_; }
    std::map<std::string, uint16_t> customization_map() { return customization_map_; }

    const ComponentInfo& component_info() { return component_info_; }
private:
    boost::flyweight<int> race_;
    bool gender_;
    boost::flyweight<std::string> model_;
    boost::flyweight<std::string> name_;
    boost::flyweight<std::string> name_file_;
    boost::flyweight<std::string> detail_file_;
    boost::flyweight<std::string> color_;
    boost::flyweight<std::string> customization_string_;
    boost::flyweight<std::map<std::string, uint16_t>> customization_map_;

    static ComponentInfo component_info_;
};

class AppearanceComponent : public AppearanceComponentInterface {
public:
    AppearanceComponent(const ObjectId& id);

    void Init(boost::property_tree::ptree& pt);
    //void HandleMessage(const Message message);
    const int race() { return race_; }
    const bool sex() { return gender_; }
    const std::string& model() { return model_; }
    const std::string& name() { return name_; }
    const std::string& name_file() { return name_file_; }
    const std::string& detail_file() { return detail_file_; }
    const std::string& color() { return color_; }
    const std::string& customization_string() { return customization_string_; }
    std::map<std::string, uint16_t> customization_map() { return customization_map_; }

    const ComponentInfo& component_info() { return component_info_; }
private:
    boost::flyweight<int> race_;
    bool gender_;
    boost::flyweight<std::string> model_;
    boost::flyweight<std::string> name_;
    boost::flyweight<std::string> name_file_;
    boost::flyweight<std::string> detail_file_;
    boost::flyweight<std::string> color_;
    boost::flyweight<std::string> customization_string_;
    boost::flyweight<std::map<std::string, uint16_t>> customization_map_;

    static ComponentInfo component_info_;
};

} // namespace components
} // namespace zone
#endif // ZONE_APPEARANCE_COMPONENT_H