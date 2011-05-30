#ifndef NODE_TYPES_H
#define NODE_TYPES_H

namespace ClientFileManager
{
	enum NODE_TYPE
			{
				//You should never see these. They are here for completeness.
				NODE_TYPE_UNKNOWN,
				NODE_TYPE_BASE,
		
				//These are the default types. 
				//They mean that no specialized type could be found.
				NODE_TYPE_FOLDER,
				NODE_TYPE_FILE,
			};
};
#endif