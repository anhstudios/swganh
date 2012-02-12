
#include "anh/resource/resource_manager.h"

#include "anh/resource/resource_archive_interface.h"

using namespace anh::resource;
using namespace std;


ResourceHandle::ResourceHandle(
	ResourceManager* resource_manager, 
	const string& resource_name, 
	const shared_ptr<vector<char>>& buffer)
	: resource_manager_(resource_manager)
	, resource_name_(resource_name)
	, buffer_(buffer)
{}

void ResourceHandle::Load(const shared_ptr<ResourceArchiveInterface>& resource_archive)
{
	resource_archive->GetResource(resource_name_, *buffer_);
}

uint32_t ResourceHandle::GetSize() const
{
	return buffer_->size();
}

const vector<char>& ResourceHandle::GetBuffer() const
{
	return *buffer_;
}

const string& ResourceHandle::GetName() const
{
    return resource_name_;
}

ResourceManager::ResourceManager(
	const shared_ptr<ResourceArchiveInterface>& resource_archive,
	uint32_t cache_size_mb)
	: resource_archive_(resource_archive)
	, cache_size_(cache_size_mb * 1024 * 1024)
{}

void ResourceManager::Initialize()
{
	resource_archive_->Open();
}

shared_ptr<ResourceHandle> ResourceManager::GetHandle(const string& resource_name)
{
    auto handle = Find(resource_name);

    if (!handle)
    {
        handle = Load(resource_name);
    }
    else
    {
        Update(handle);
    }

    return handle;
}

void ResourceManager::FlushCache()
{
    least_recently_used_.clear();
    resources_.clear();
}

shared_ptr<ResourceHandle> ResourceManager::Find(const string& resource_name)
{
    auto find_iter = resources_.find(resource_name);

    if (find_iter == resources_.end())
    {
        return nullptr;
    }

    return find_iter->second;
}

shared_ptr<ResourceHandle> ResourceManager::Load(const string& resource_name)
{
    uint32_t size = resource_archive_->GetResourceSize(resource_name);

    auto buffer = Allocate(size);

    if (!buffer)
    {
        return nullptr;
    }

    shared_ptr<ResourceHandle> handle(
        new ResourceHandle(this, resource_name, move(buffer)),
        [this] (ResourceHandle* handle)
    {
        allocated_ -= handle->GetSize();
        delete handle;   
    });

    handle->Load(resource_archive_);

    least_recently_used_.push_front(handle);
    resources_[resource_name] = handle;

    return handle;
}

void ResourceManager::Update(const shared_ptr<ResourceHandle>& handle)
{
    least_recently_used_.remove(handle);
    least_recently_used_.push_front(handle);
}

shared_ptr<vector<char>> ResourceManager::Allocate(uint32_t size)
{
    auto buffer = make_shared<vector<char>>();
    buffer->reserve(size);
    
    return buffer;
}

void ResourceManager::FreeOneResource()
{
    auto least_used_iter = least_recently_used_.end();
    
    Free(*(--least_used_iter));
}

bool ResourceManager::MakeRoom(uint32_t size)
{
    if (size > cache_size_)
    {
        return false;
    }

    while (size > (cache_size_ - allocated_))
    {
        if (least_recently_used_.empty())
        {
            return false;
        }

        FreeOneResource();
    }

    return true;
}

void ResourceManager::Free(const shared_ptr<ResourceHandle>& handle)
{
    least_recently_used_.remove(handle);
    resources_.erase(handle->GetName());
}
