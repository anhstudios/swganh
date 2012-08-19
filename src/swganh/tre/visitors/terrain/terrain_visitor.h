#ifndef SWGANH_TRE_TERRAIN_VISITOR_H_
#define SWGANH_TRE_TERRAIN_VISITOR_H_

#include "swganh/tre/visitors/visitor_interface.h"

#include <vector>

namespace swganh
{
namespace tre
{
	class Fractal;
	class Layer;
	struct TrnHeader;

	class TerrainVisitor : public VisitorInterface
	{
	public:

		TerrainVisitor();
		~TerrainVisitor();

		/**
			@brief returns the VisitorType associated with this visitor
		*/
		virtual VisitorType getType() { return TRN_VISITOR; }

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

		std::vector<Fractal*>& GetFractals() { return fractals_; }
		std::vector<Layer*>& GetLayers() { return layers_; }

	private:
		TrnHeader* header;
		std::vector<Fractal*> fractals_;
		std::vector<Layer*> layers_;
	};
}
}

#endif