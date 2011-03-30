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
#include "tag_component.h"
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;
namespace zone { namespace components {

// statics
std::shared_ptr<NullTagComponent> TagComponentInterface::NullComponent = std::shared_ptr<NullTagComponent>(new NullTagComponent());
ComponentInfo NullTagComponent::component_info_ = ComponentInfo(ComponentType("NullTagComponent"), false);
ComponentInfo TagComponent::component_info_ = ComponentInfo(ComponentType("TagComponent"), true);

void TagComponent::Init(boost::property_tree::ptree& pt) {
     // initial default values
    string tags = pt.get<string>("common.tag", false);
    list<string> strs;
    boost::split(strs, tags, boost::is_any_of(","));
    for_each(strs.begin(), strs.end(), [&] (string str) {
        tag_list_.push_back(anh::HashString(str));
    });
}
uint32_t TagComponent::hash_tag(const std::string& tag){
    auto it = find_if(tag_list_.begin(), tag_list_.end(), [=] (anh::HashString hash) {
        return hash.ident_string() == tag;
    });
    return (*it).ident();
}
string TagComponent::tag(const uint32_t hash){
    auto it = find_if(tag_list_.begin(), tag_list_.end(), [=] (anh::HashString hash) {
        return hash.ident() == hash;
    });
    return (*it).ident_string();
}
bool TagComponent::has( anh::HashString& hash_string )
{
    auto it = find_if(tag_list_.begin(), tag_list_.end(), [=] (anh::HashString hash) {
        return hash_string == hash;
    });
    if (it != tag_list_.end())
    {
        return true;
    }
    return false;
}
bool TagComponent::has( uint32_t hash_tag )
{
    auto it = find_if(tag_list_.begin(), tag_list_.end(), [=] (anh::HashString hash) {
        return hash_tag == hash.ident();
    });
    if (it != tag_list_.end())
    {
        return true;
    }
    return false;
}
bool TagComponent::has( std::string& string)
{
    auto it = find_if(tag_list_.begin(), tag_list_.end(), [=] (anh::HashString hash) {
        return string == hash.ident_string();
    });
    if (it != tag_list_.end())
    {
        return true;
    }
    return false;
}
} // components
} // zone