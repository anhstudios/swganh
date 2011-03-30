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
#ifndef ZONE_TAG_COMPONENT_H
#define ZONE_TAG_COMPONENT_H

#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>

#include <boost/flyweight.hpp>
#include <cstdint>
#include <list>

using namespace anh::component;
namespace zone { namespace components {
class NullTagComponent;

class TagComponentInterface : public BaseComponent {
public:
    TagComponentInterface(const ObjectId& id)
		: BaseComponent(id) { }
    virtual std::list<anh::HashString> tag_list() = 0;
    virtual uint32_t hash_tag(const std::string& tag) = 0;
    virtual std::string tag(const uint32_t hash_tag) = 0;
    virtual bool has( anh::HashString& tag ) = 0;
    virtual bool has( uint32_t hash_tag ) = 0;
    virtual bool has( std::string& ) = 0;
    static std::shared_ptr<NullTagComponent> NullComponent;
};

class NullTagComponent : TagComponentInterface {
public:
    NullTagComponent()
		: TagComponentInterface(0) { }

    std::list<anh::HashString> tag_list() { return tag_list_; }
    uint32_t hash_tag(const std::string& tag) { return 0; }
    std::string tag(const uint32_t hash_tag) { return ""; }
    bool has( anh::HashString& hash_string )  { return false; }
    bool has( uint32_t hash_tag ) { return false; }
    bool has( std::string& string)  { return false; }
    const ComponentInfo& component_info() { return component_info_; }
private:
    std::list<anh::HashString> tag_list_;
    
    static ComponentInfo component_info_;
};
class TagComponent : TagComponentInterface {
public:
    TagComponent(const ObjectId& id)
		: TagComponentInterface(id) { }

    void Init(boost::property_tree::ptree& pt);
    std::list<anh::HashString> tag_list() { return tag_list_; }
    uint32_t hash_tag(const std::string& tag);
    std::string tag(const uint32_t hash_tag);
    bool has( anh::HashString& hash_string );
    bool has( uint32_t hash_tag );
    bool has( std::string& string);
    const ComponentInfo& component_info() { return component_info_; }
private:
    std::list<anh::HashString> tag_list_;
    typedef std::list<anh::HashString>::iterator        TagListIterator;
    static ComponentInfo component_info_;
};
} // components
} // zone

#endif //ZONE_TAG_COMPONENT_H