#ifndef INTERPRETERS_WS_H
#define INTERPRETERS_WS_H

#include "../IFFInterpreter.h"
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			class WSInterpreter : public IFFInterpreter
			{
			public:
				
				WSInterpreter();

				virtual void interpret(std::shared_ptr<IFF::FileNode> node);

				struct CHUNK
				{
					std::uint64_t id;
					std::uint64_t parent_id;
					std::uint32_t name_id;
					float scale;
					glm::quat orientation;
					glm::vec3 location;
					float object_priority; //could also be type, but I don't buy it.
					std::uint32_t pob_crc;
				};

				std::map<std::uint64_t, CHUNK> chunks;
				std::vector<std::string> names;
				
				void debug();
				virtual InterpreterType getType() { return WS_INTERPRETER; }

			private:
				void _handleOTNL(anh::ByteBuffer& buffer);
				void _handle0000DATA(anh::ByteBuffer& buffer);
			};
		};
	};
};

#endif