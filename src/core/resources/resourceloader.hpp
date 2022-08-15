#pragma once

#include "core/filesystem/gamefilesystem.hpp"

namespace Vin {
	template<typename T>
	class ResourceLoader {
		//static_assert(false, "Cannot load resource of type T. Provide a ResourceLoader<T> specialization.");
	};

	template<>
	class ResourceLoader<char*> {
	public:
		static void* Load(std::unique_ptr<GameFile> file) {
			char* buff = (char*)malloc(file->GetSize());
			file->ReadBytes(buff, file->GetSize());
			return buff;
		}

		static void Unload(void* data) {
			free(data);
		}
	};
}
