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

#include <anh/component/test_components_unittest.h>

namespace anh { namespace test_components {

ComponentInfo NullTransformComponent::component_info_ = ComponentInfo(ComponentType("NullTransformComponent"), false);
std::shared_ptr<NullTransformComponent> TransformComponentInterface::NullComponent = std::shared_ptr<NullTransformComponent>(new NullTransformComponent());
ComponentInfo TransformComponent::component_info_ = ComponentInfo(ComponentType("TransformComponent"), false);

ComponentInfo NullAppearanceComponent::component_info_ = ComponentInfo(ComponentType("NullAppearance"), false);
std::shared_ptr<NullAppearanceComponent> AppearanceComponentInterface::NullComponent = std::shared_ptr<NullAppearanceComponent>(new NullAppearanceComponent());
ComponentInfo AppearanceComponent::component_info_ = ComponentInfo(ComponentType("Appearance"), false);

std::shared_ptr<NullTickableComponent> TickableComponentInterface::NullComponent = std::shared_ptr<NullTickableComponent>(new NullTickableComponent());
ComponentInfo NullTickableComponent::s_component_info_ = ComponentInfo(ComponentType("NullTickableComponent"));
ComponentInfo TickableComponent::s_component_info_ = ComponentInfo(ComponentType("TickableComponent"), true);

std::shared_ptr<NullHAMComponent> HAMComponentInterface::NullComponent = std::shared_ptr<NullHAMComponent>( new NullHAMComponent() );
ComponentInfo NullHAMComponent::component_info_ = ComponentInfo(ComponentType("NullHAMComponent"));
ComponentInfo HAMComponent::component_info_ = ComponentInfo(ComponentType("HAMComponent"));
}} // namespaces