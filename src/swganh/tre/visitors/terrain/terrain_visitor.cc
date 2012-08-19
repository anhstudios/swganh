#include "terrain_visitor.h"

#include "detail/fractal.h"
#include "detail/layer.h"

#include "detail/container_layer.h"

#include "detail/layer_loader.h"

#include "swganh/tre/iff/filenode.h"
#include "swganh/tre/iff/foldernode.h"

#include "anh/logger.h"
#include "detail/header.h"

#include <stack>

using namespace swganh::tre;

Fractal* working_fractal_;
std::stack<std::pair<ContainerLayer*, uint32_t>> layer_stack_;

TerrainVisitor::TerrainVisitor()
{
	header = new TrnHeader();
}

TerrainVisitor::~TerrainVisitor()
{
	delete header;
	
	for(auto& fractal : fractals_)
	{
		delete fractal;
	}

	for(auto& layer : layers_)
	{
		delete layer;
	}
}

void TerrainVisitor::visit_data(uint32_t depth, std::shared_ptr<file_node> node)
{
	//Fractal Reading
	if(node->name() == "MFAMDATA")
	{
		working_fractal_ = new Fractal(node->data());
		this->fractals_.push_back(working_fractal_);
	} 
	else if(node->name() == "0001DATA" && working_fractal_ != nullptr) 
	{
		working_fractal_->Deserialize(node->data());
		working_fractal_ = nullptr;
	} 
	else if(node->name() == "DATAPARM" && layer_stack_.size() > 0)
	{
		layer_stack_.top().first->Deserialize(node->data());
	}
	else if(node->name() == "0014DATA")
	{
		header->Deserialize(node->data());
	}
}

void TerrainVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
{	
	while(layer_stack_.size() > 0 && layer_stack_.top().second > depth)
	{
		layer_stack_.pop();
	}

	Layer* working_layer_ = LayerLoader(node);
	if(working_layer_ != nullptr)
	{
		if(layer_stack_.size() == 0)
		{
			layers_.push_back(working_layer_);
		} 
		else 
		{
			layer_stack_.top().first->InsertLayer(working_layer_);
		}

		if(working_layer_->GetType() == LAYER_TYPE_CONTAINER)
		{
			auto entry = std::make_pair<ContainerLayer*, uint32_t>(std::forward<ContainerLayer*>((ContainerLayer*)working_layer_), 
				std::forward<uint32_t>(depth));

			layer_stack_.push(std::move(entry));
		}
	}
}