// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_LIB_ATTRIBUTES_HELPER_H_
#define PUB14_CORE_LIB_ATTRIBUTES_HELPER_H_

#include <memory>
#include <string>
#include <vector>

#include "pub14_core/messages/attribute_list_message.h"

namespace swganh {
namespace object {
    class Object;
}}

namespace swganh_core {
namespace attributes {
	class AttributesHelper
	{	
	public:
        static void SetOptionalAttribute(
            std::vector<swganh::messages::Attribute>& attributes,
			const std::string& attribute_label,
            const std::string& attribute_name,
            const std::shared_ptr<swganh::object::Object> object,
			bool recursive = false
        );
	};

}}


#endif //  PUB14_CORE_LIB_WEAPON_ATTRIBUTE_TEMPLATE_H_