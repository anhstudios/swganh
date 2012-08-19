#include "layer_visitor.h"

#include "detail/container_layer.h"

#include "swganh/tre/iff/filenode.h"
#include "swganh/tre/iff/foldernode.h"

#include "detail/layer_loader.h"

#include <stack>

using namespace swganh::tre;

std::stack<std::pair<ContainerLayer*, uint32_t>> layer_stack_;

LayerVisitor::~LayerVisitor()
{
	for(auto& layer : layers_)
	{
		delete layer;
	}
}

void LayerVisitor::visit_data(uint32_t depth, std::shared_ptr<file_node> node)
{
	if(node->name() == "DATAPARM" && layer_stack_.size() > 0)
	{
		layer_stack_.top().first->Deserialize(node->data());
	}
}

void LayerVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
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