#include <vector>
#include <memory>

#include "ClientFileManager.h"

#include "iff/nodes/foldernode.h"
#include "iff/nodes/Node_PRTL.h"
#include "iff/nodes/Node_PRTL0004.h"
#include "iff/nodes/Node_0000VERT.h"
#include "iff/nodes/Node_0005DATA.h"
#include "iff/nodes/Node_INDX.h"

namespace ClientFileManager
{
	class POB : public SWGFileInterpreter
	{
	public:
		POB(IFF::IFFFile* iff)
			: SWGFileInterpreter(iff)
		{
			for(unsigned int i = 0; i < iff->headnode_count(); ++i)
			{
				_interpretNode(iff->headnode(i));
			}
		}

		struct Cell
		{
			ClientFileManager::IFF::Nodes::Node_0005DATA* names;
			ClientFileManager::IFF::Nodes::Node_0000VERT* verts;
			ClientFileManager::IFF::Nodes::Node_INDX* triangles;
			std::vector<ClientFileManager::IFF::Nodes::Node_PRTL0004*> links;
		};

		virtual std::string getName() { return "Portal OBject."; }

		unsigned int getPortalCount() {return portals_.size();}
		ClientFileManager::IFF::Nodes::Node_PRTL* getPortal(unsigned int id) {return portals_[id];}

		unsigned int getCellCount() {return cells_.size();}
		Cell* getCell(unsigned int id) {return cells_[id];}

	private:
		void _interpretNode(std::shared_ptr<IFF::Node> nod)
		{
			if(nod->type() == ClientFileManager::NODE_TYPE_PRTL)
			{
				portals_.push_back((ClientFileManager::IFF::Nodes::Node_PRTL*)nod.get());
			}
			else if( nod->type() == ClientFileManager::NODE_TYPE_0005DATA)
			{
				Cell* c = new Cell();
				c->names = (ClientFileManager::IFF::Nodes::Node_0005DATA*)nod.get();
				cells_.push_back(c);
			}
			else if( nod->type() == ClientFileManager::NODE_TYPE_0000VERT)
			{
				cells_[cells_.size()-1]->verts = (ClientFileManager::IFF::Nodes::Node_0000VERT*)nod.get();
			}
			else if( nod->type() == ClientFileManager::NODE_TYPE_PRTL0004)
			{
				cells_[cells_.size()-1]->links.push_back((ClientFileManager::IFF::Nodes::Node_PRTL0004*)nod.get());
			}
			else if(nod->type() == ClientFileManager::NODE_TYPE_INDX)
			{
				cells_[cells_.size()-1]->triangles = (ClientFileManager::IFF::Nodes::Node_INDX*)nod.get();
			}
			else if( nod->type() == ClientFileManager::NODE_TYPE_FOLDER)
			{
				ClientFileManager::IFF::FolderNode* folder = (ClientFileManager::IFF::FolderNode*)nod.get();

				for(unsigned int i=0; i < folder->children_count(); ++i)
				{
					_interpretNode(folder->get(i));
				}
			}
		}

		std::vector<ClientFileManager::IFF::Nodes::Node_PRTL*> portals_;
		std::vector<Cell*> cells_;
	};
};