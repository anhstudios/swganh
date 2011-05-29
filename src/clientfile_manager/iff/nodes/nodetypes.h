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

				//These are the actual types.
				NODE_TYPE_0003DATA,
				NODE_TYPE_PRTL,
				NODE_TYPE_0000VERT,
				NODE_TYPE_0005DATA,
				NODE_TYPE_INDX,
				NODE_TYPE_PRTL0004,
				NODE_TYPE_LGHT,
				NODE_TYPE_0001META,
				NODE_TYPE_PNOD,
				NODE_TYPE_PEDG,
				NODE_TYPE_ECNT,
				NODE_TYPE_ESTR,
			};
};
#endif