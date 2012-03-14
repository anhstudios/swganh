
#ifndef ANH_RESOURCE_RESOURCE_MANAGER_H_
#define ANH_RESOURCE_RESOURCE_MANAGER_H_

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace anh {
namespace resource {

	class ResourceArchiveInterface;
	class ResourceManager;

	class ResourceHandle
	{
	public:
		ResourceHandle(
			ResourceManager* resource_manager, 
			const std::string& resource_name, 
			const std::shared_ptr<std::vector<char>>& buffer);

		void Load(const std::shared_ptr<ResourceArchiveInterface>& resource_archive);

		uint32_t GetSize() const;

		const std::vector<char>& GetBuffer() const;

        const std::string& GetName() const;

	private:
		ResourceManager* resource_manager_;
		std::string resource_name_;
		std::shared_ptr<std::vector<char>> buffer_;
		uint32_t size_;
	};

	class ResourceManager
	{
	public:
		ResourceManager(const std::shared_ptr<ResourceArchiveInterface>& resource_archive, uint32_t cache_size_mb);

		void Initialize();

		std::shared_ptr<ResourceHandle> GetHandle(const std::string& resource_name);

		void FlushCache();

	private:
        typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandleList;
        typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResourceHandleMap;

		std::shared_ptr<ResourceHandle> Find(const std::string& resource_name);
		std::shared_ptr<ResourceHandle> Load(const std::string& resource_name);
		void Update(const std::shared_ptr<ResourceHandle>& handle);
		std::shared_ptr<std::vector<char>> Allocate(uint32_t size);
		void FreeOneResource();
		bool MakeRoom(uint32_t size);
		void Free(const std::shared_ptr<ResourceHandle>& handle);

        ResourceHandleList least_recently_used_;
        ResourceHandleMap resources_;

		std::shared_ptr<ResourceArchiveInterface> resource_archive_;
		uint32_t cache_size_;
        uint32_t allocated_;
	};

}}  // namespace anh::resource

#endif  // ANH_RESOURCE_RESOURCE_MANAGER_H_
