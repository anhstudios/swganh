#include "IFFInterpreter.h"

#include <vector>
#include <glm/glm.hpp>

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			class POBInterpreter : public IFFInterpreter
			{
			public:
				POBInterpreter() : IFFInterpreter()
				{
				}
				virtual void interpret(IFF::FileNode* node);

				struct Portal
				{
					std::vector<glm::vec3>
				};

			private:
			};
		};
	};
};