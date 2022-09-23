#pragma once

#include "asset.hpp"

#define ASSET_REGISTRY_PATH_LENGTH 256
#define ASSET_REGISTRY_MAGIC "\xAF\x5C\xB5\x3B"

namespace Vin {

	struct AssetRegistryHeader {
		char magic[4];
		uint32_t count;
		AssetId offset;
	};

	struct AssetRegistryPath {
		char path[256];
	};

	class AssetRegistry {
	public:
		AssetRegistry() = default;

	public:
		AssetRegistryPath GetPath(AssetId id);
		AssetId AddPath(const char* path, size_t size);

	private:
		AssetId m_Offset{};
		eastl::vector<AssetRegistryPath> m_Pathes{};

		friend class AssetRegistrySerializer;
	};

	class AssetRegistrySerializer {
	public:
		static bool Load(AssetRegistry& registry, const char* path);
		static bool Save(AssetRegistry& registry, const char* path);
	};
}
