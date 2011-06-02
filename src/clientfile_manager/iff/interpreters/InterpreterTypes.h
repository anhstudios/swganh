#ifndef IFF_INTERPRETER_TYPES_H
#define IFF_INTERPRETER_TYPES_H

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			enum InterpreterType
			{
				IFF_INTERPRETER, //BASE CLASS

				POB_INTERPRETER,
				TRN_INTERPRETER,
				WS_INTERPRETER
			};
		};
	};
};
#endif