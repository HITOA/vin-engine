#pragma once

#include "vinpch.hpp"
#include "resource.hpp"

namespace Vin {

	class ResourceManager {
	public:
		template<typename T>
		static T* Load(const char* path) {

		}
		template<typename T>
		static T* GetResource(ResourceHandle handle) {

		}
		template<typename T>
		static void Unload(T resource) {

		}
		static void Unload(ResourceHandle handle) {

		}
		static void UnloadUnused() {

		}
	private:
		static std::unordered_map<ResourceHandle, Resource*> s_Resources;
		static std::unordered_map<const char*, ResourceHandle> s_ResourcesPath;
	};

}
