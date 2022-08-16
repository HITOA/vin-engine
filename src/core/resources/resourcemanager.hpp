#pragma once

#include "vinpch.hpp"
#include "resource.hpp"
#include "core/filesystem/gamefilesystem.hpp"
#include "core/allocator.hpp"
#include "core/assert.hpp"
#include "core/logger/logger.hpp"

namespace Vin {
	class Resources {
	public:
		template<typename T>
		static eastl::shared_ptr<T> Load(const char* path) {
			static_assert(eastl::is_base_of<Resource, T>::value, "T must be a derived of Resource class.");
			if (s_ResourcesPathe.find_as(path) != s_ResourcesPathe.end()) {
				eastl::shared_ptr<Resource> resource = s_Resources[s_ResourcesPathe[path]];
				if (resource->m_TypeId != ResourceTrait::GetTypeId<T>()) {
					Logger::Err("Couldn't load resource {} : Resource already exists but are not the same type.", path);
					return nullptr;
				}
				return eastl::static_shared_pointer_cast<T>(resource);
			}

			if (!GameFilesystem::Exists(path)) {
				Logger::Err("Couldn't load resource {} : Resource does not exists.", path);
				return nullptr;
			}

			ResourceHandle handle = ResourceTrait::GetNextHandle();

			eastl::shared_ptr<T> resource = eastl::make_shared<T>();
			resource->m_Handle = handle;
			resource->m_TypeId = ResourceTrait::GetTypeId<T>();
			resource->m_Path = path;

			resource->Load(GameFilesystem::Open(path, FileMode::Read));

			s_Resources[handle] = resource;
			s_ResourcesPathe[path] = handle;

			return resource;
		}

		template<typename T>
		static eastl::shared_ptr<T> GetResource(ResourceHandle handle) {
			if (s_Resources.find(handle) == s_Resources.end()) {
				Logger::Err("Couldn't get resource with handle {} : Resource does not exists.", handle);
				return nullptr;
			}

			eastl::shared_ptr<Resource> resource = s_Resources[handle];

			if (resource->m_TypeId != ResourceTrait::GetTypeId<T>()) {
				Logger::Err("Couldn't get resource with handle {} : Bad resource type", path);
				return nullptr;
			}

			return eastl::static_shared_pointer_cast<T>(resource);
		}
	private:
		static eastl::hash_map<ResourceHandle, eastl::shared_ptr<Resource>> s_Resources;
		static eastl::hash_map<const char*, ResourceHandle> s_ResourcesPathe;
	};

	eastl::hash_map<ResourceHandle, eastl::shared_ptr<Resource>> Resources::s_Resources{};
	eastl::hash_map<const char*, ResourceHandle> Resources::s_ResourcesPathe{};
}