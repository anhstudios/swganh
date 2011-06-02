#ifndef INTERPRETERS_POB_H
#define INTERPRETERS_POB_H

#include "../IFFInterpreter.h"
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace anh
{
	class ByteBuffer;
};

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			class POBInterpreter : public IFFInterpreter
			{
			public:
				POBInterpreter();

				virtual void interpret(std::shared_ptr<IFF::FileNode> node);

				class Portal
				{
				public:
					std::vector<glm::vec3> vertices;
				};

				class Link
				{
				public:
					std::uint32_t portal_id; //portal id
					
					char unkFlag2;
					std::uint32_t dst_cellid; //destination cell id

					//BELOW HERE IS LIKELY DOOR STUFF WE ARE NOT CONCERNED WITH
					std::string doorname;

					glm::vec3 unk1; //Seems like an inefficient bitmask
					float unk2; //scale?

					glm::vec3 unk3; //Seems like an inefficient bitmask
					float unk4; //Scale?

					glm::vec3 unk5; //Seems like an inefficient bitmask
					float unk6; //Scale?

				};

				class triangle
				{
				public:
					std::uint32_t a;
					std::uint32_t b;
					std::uint32_t c;
				};

				class Cell
				{
				public:
					//0005DATA
					char unkFlag2;
					std::string name;
					char unkFlag3;
					std::string mesh;
					char unkFlag4;
					std::string collision;

					//0000VERT
					std::vector<glm::vec3> vertices;

					//INDX
					std::vector<triangle> triangles;

					//PRTL0004
					std::vector<Link> links;
				};

				unsigned int cell_count() { return cells_.size(); }
				unsigned int portal_count() { return portals_.size(); }

				const Portal& getPortal(unsigned int id) { return portals_[id]; }
				const Cell& getCell(unsigned int id) { return cells_[id]; }

				void debug();

				virtual InterpreterType getType() { return POB_INTERPRETER; }

			private:
				void _handle0003DATA(anh::ByteBuffer& data);
				void _handlePRTL(anh::ByteBuffer& data);
				void _handle0005DATA(anh::ByteBuffer& data);
				void _handle0000VERT(anh::ByteBuffer& data);
				void _handleINDX(anh::ByteBuffer& data);
				void _handlePRTL0004(anh::ByteBuffer& data);

				std::vector<Portal> portals_;
				std::vector<Cell> cells_;
			};
		};
	};
};

#endif