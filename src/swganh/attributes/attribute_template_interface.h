// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_
#define SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/event_dispatcher.h"

namespace swganh {
namespace attributes {

    class AttributeTemplateInterface 
    {
    public:
        virtual void RegisterEventHandlers() = 0;
        virtual std::wstring BuildAttributeTemplate() = 0;
		virtual void LoadAttributes() = 0;
    protected:
        anh::EventDispatcher* event_dispatcher; 
    };

}}


#endif // SWGANH_ATTRIBUTE_TEMPLATE_INTERFACE_H_