#include "terrain_visitor.h"

#include "detail/header.h"

using namespace swganh::tre;

TerrainVisitor::TerrainVisitor()
{
	header = new TrnHeader();
}

TerrainVisitor::~TerrainVisitor()
{
	delete header;
}

void TerrainVisitor::visit_data(uint32_t depth, std::shared_ptr<file_node> node)
{
}

void TerrainVisitor::visit_folder(uint32_t depth, std::shared_ptr<folder_node> node)
{
}