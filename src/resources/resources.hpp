#pragma once

#include "vinpch.hpp"
#include "resource.hpp"
#include "filesystem/gamefilesystem.hpp"
#include "core/allocator.hpp"
#include <assert.hpp>
#include "logger/logger.hpp"

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

		static void Unload(ResourceHandle handle) {
			if (s_Resources.find(handle) == s_Resources.end()) {
				Logger::Err("Couldn't unload resource with handle {} : Resource does not exists.", handle);
				return;
			}

			eastl::shared_ptr<Resource> resource = s_Resources[handle];

			resource->Unload();

			s_ResourcesPathe.erase(resource->m_Path);
			s_Resources.erase(handle);
		}

		static void Unload(eastl::shared_ptr<Resource> resource) {
			Unload(resource->m_Handle);
		}

		static void UnloadUnused() {
			static constexpr size_t handlebuffsize = 512;
			static ResourceHandle handlebuff[handlebuffsize]{};
			size_t idx = 0;
			for (auto& it : s_Resources) {
				if (it.second.use_count() <= 1) {
					handlebuff[idx] = it.first;
					idx++;
				}
				if (idx >= handlebuffsize)
					break;
			}

			for (size_t i = 0; i < idx; i++) {
				eastl::shared_ptr<Resource> resource = s_Resources[handlebuff[i]];

				resource->Unload();

				s_ResourcesPathe.erase(resource->m_Path);
				s_Resources.erase(resource->m_Handle);;
			}
		}

		static size_t GetResourceCount() {
			return s_Resources.size();
		}
	private:
		static eastl::hash_map<ResourceHandle, eastl::shared_ptr<Resource>> s_Resources;
		static eastl::hash_map<const char*, ResourceHandle> s_ResourcesPathe;
	};

	eastl::hash_map<ResourceHandle, eastl::shared_ptr<Resource>> Resources::s_Resources{};
	eastl::hash_map<const char*, ResourceHandle> Resources::s_ResourcesPathe{};
}