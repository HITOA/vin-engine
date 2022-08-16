#pragma once
/*
#include "vinpch.hpp"
#include "resource.hpp"
#include "core/filesystem/gamefilesystem.hpp"
#include "core/allocator.hpp"

namespace Vin {

	template<typename Allocator = DefaultAllocator>
	class ResourceManager {
	public:
		template<typename T>
		static Ref<Resource<Allocator>, Allocator> Load(const char* path) {
			if (!GameFilesystem::Exists(path))
				return nullptr;
			ResourceHandle currentHandle = ++m_LastHandle;
			Resource<Allocator>* resource = Allocator::New<Resource<Allocator>>();
			
			resource->m_Handle = currentHandle;
			resource->m_Path = path;
			resource->m_TypeId = ResourceTrait::GetTypeId<T>();

			resource->Load(path);

			return Ref<Resource<Allocator>, Allocator>{ resource->Clone() };
		}
		template<typename T>
		static Ref<Resource<Allocator>, Allocator> GetResource(ResourceHandle handle) {

		}
		template<typename T>
		static void Unload(Ref<Resource<Allocator>, Allocator> resource) {

		}
		static void Unload(ResourceHandle handle) {

		}
		static void UnloadUnused() {

		}
	private:
		static std::unordered_map<ResourceHandle, Resource*> s_Resources;
		static std::unordered_map<const char*, ResourceHandle> s_ResourcesPath;
		static ResourceHandle m_LastHandle;
	};

}
*/