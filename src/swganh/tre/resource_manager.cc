
#include "swganh/byte_buffer.h"
#include "resource_manager.h"

#include "iff/iff.h"

#include "swganh/logger.h"

using namespace swganh::tre;

ResourceManager::ResourceManager(std::shared_ptr<TreArchive> archive) 
	: archive_(archive)
{
}

void ResourceManager::LoadResourceByName(const std::string& name, std::shared_ptr<VisitorInterface> type, bool is_cached) 
{
	iff_file::loadIFF(archive_->GetResource(name), type);
	if(is_cached)
	{
		loadedResources_.insert(ResourceCache::value_type(name, type));
	}
}