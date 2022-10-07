#pragma once

#include "vinpch.hpp"
#include "vfs/vfs.hpp"

namespace Vin {

	template<typename T>
	struct AssetSerializer {
		
	};

	template<typename T>
	struct AssetDeserializer {
		//static std::shared_ptr<T> Deserialize(std::unique_ptr<File> file) {
			//return nullptr;
		//}
	};

	//Basic example

	template<>
	struct AssetDeserializer<std::string> {
		static std::shared_ptr<std::string> Deserialize(std::unique_ptr<File> file) {
			std::shared_ptr<std::string> str{ std::make_shared<std::string>() };
			size_t filesize = file->GetSize();
			str->resize(filesize);
			file->ReadBytes(str->data(), filesize);
			return str;
		}
	};

}
