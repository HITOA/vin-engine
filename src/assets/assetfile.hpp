#pragma once

#include "core/base.hpp"
#include "asset.hpp"

#define ASSET_MAGIC "\xE9\x23\xE6\x0E"

namespace Vin {
	/*enum class AssetType {
		None,
		Unkown,
		Shader,
		Texture
	};

	struct AssetHeader {
		char magic[4];
		AssetId id;
		AssetType type;
	};

	class AssetFileSerializer {
	public:
		template<typename T>
		static bool Load(const char* path, AssetHeader& header, T& data) {
			return Load(path, header, &data, sizeof(T));
		}
		template<typename T>
		static bool Save(const char* path, AssetHeader& header, T& data) {
			return Save(path, header, &data, sizeof(T));
		}

		static bool Load(const char* path, AssetHeader& header, void* buff, usize buffsize);
		static bool Save(const char* path, AssetHeader& header, void* buff, usize buffsize);
	};*/
}
