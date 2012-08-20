#ifndef SWGANH_TRE_TERRAIN_VISITOR_H_
#define SWGANH_TRE_TERRAIN_VISITOR_H_

#include "swganh/tre/visitors/visitor_interface.h"

#include <vector>
#include <map>
#include <stack>

namespace swganh
{
namespace tre
{
	class Fractal;
	class Layer;
	class ContainerLayer;
	struct TrnHeader;

	typedef std::map<uint32_t,Fractal*> FractalMap;

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

		FractalMap& GetFractals() { return fractals_; }
		std::vector<ContainerLayer*>& GetLayers() { return layers_; }

	private:
		Fractal* working_fractal_;
		std::stack<std::pair<Layer*, uint32_t>> layer_stack_;
		Layer* working_layer_;

		TrnHeader* header;
		FractalMap fractals_;
		std::vector<ContainerLayer*> layers_;
	};
}
}

#endif