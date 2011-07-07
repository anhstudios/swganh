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
#ifndef ANH_COMPONENT_ATTRIBUTE_MAPPER_INTERFACE_H_
#define ANH_COMPONENT_ATTRIBUTE_MAPPER_INTERFACE_H_

#include <cstdint>
#include <memory>

namespace anh { 
namespace component {

/**
 * @brief Maps/Serializes component attributes to an outside source.
 */
template<class ClassToMap>
class AttributeMapperInterface
{
public:
    /**
     * @brief Called when the component is being presisted to the outside source.
     *
      * @param The component to persist.
     */
    virtual void Persist(std::shared_ptr<ClassToMap> component) = 0;

    /**
     * @brief Called initially to populate a components attributes.
     *
     * @param component The component to aggregate data to.
     */
    virtual void Populate(std::shared_ptr<ClassToMap> component) = 0;

    /**
    * @brief used to fill a component object from an outside source.
    *
    * @param entity_id, the id of the entity to grab data from.
    */
    virtual std::shared_ptr<ClassToMap> query_result(uint64_t entity_id) = 0;
protected:
};

} // namespace database
} // namespace anh


#endif // ANH_COMPONENT_ATTRIBUTE_MAPPER_INTERFACE_H_
