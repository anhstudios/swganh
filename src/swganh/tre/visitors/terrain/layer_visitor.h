#ifndef SWGANH_TRE_LAYER_VISITOR_H_
#define SWGANH_TRE_LAYER_VISITOR_H_

#include "swganh/tre/visitors/visitor_interface.h"
#include <vector>
#include <list>


namespace swganh
{
namespace tre
{
	class Layer;

	class LayerVisitor : public VisitorInterface
	{
	public:
		/**
			@brief returns the VisitorType associated with this visitor
		*/
		virtual VisitorType getType() { return LAY_VISITOR; }

		/**
			@brief interprets a IFF::FileNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(uint32_t depth, std::shared_ptr<file_node> node);

		/**
			@brief interprets a IFF::FolderNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(uint32_t depth, std::shared_ptr<folder_node> node);
	private:
		std::list<Layer> layers_;
	};
}
}

#endif