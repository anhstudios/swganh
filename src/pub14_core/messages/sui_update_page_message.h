// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SUI_UPDATE_PAGE_MESSAGE_H_
#define SWGANH_MESSAGES_SUI_UPDATE_PAGE_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

#include <swganh/sui/sui_window_interface.h>

namespace swganh {
namespace messages {

    struct SUIUpdatePageMessage : public BaseSwgMessage<SUIUpdatePageMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x5F3342F6; }

		int32_t window_id;
		std::string script_name;
		std::vector<swganh::sui::SUIWindowInterface::SUI_WINDOW_COMPONENT> components;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(window_id);
			buffer.write(script_name);
			buffer.write(components.size());

			for(auto& component : components)
			{
				buffer.write(component.type);
				buffer.write(component.narrow_params.size());
				for(auto& narrow : component.narrow_params)
				{
					buffer.write(narrow);
				}

				buffer.write(component.wide_params.size());
				for(auto& wide : component.wide_params)
				{
					buffer.write(wide);
				}
			}
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		window_id = buffer.read<int32_t>();
			script_name = buffer.read<std::string>();
			
			int component_count = buffer.read<int32_t>();
			for(int i=0; i < component_count; ++i)
			{
				swganh::sui::SUIWindowInterface::SUI_WINDOW_COMPONENT component;

				int narrows = buffer.read<int32_t>();
				for(int n=0; n < narrows; ++n)
				{
					component.narrow_params.push_back(buffer.read<std::string>());
				}

				int wides = buffer.read<int32_t>();
				for(int w=0; w < wides; ++w)
				{
					component.wide_params.push_back(buffer.read<std::wstring>());
				}

				components.push_back(component);
			}
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SUI_UPDATE_PAGE_MESSAGE_H_
