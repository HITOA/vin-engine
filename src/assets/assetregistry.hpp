#pragma once

#include "asset.hpp"

#define ASSET_REGISTRY_PATH_LENGTH 256
#define ASSET_REGISTRY_MAGIC "\xAF\x5C\xB5\x3B"

namespace Vin {

	struct AssetRegistryHeader {
		char magic[4];
		char name[64];
		char path[256];
		uint32_t count;
	};

	struct AssetRegistryPath {
		char path[256];
	};

	class AssetRegistry {
	public:
		AssetRegistry() = default;

	public:
		AssetRegistryPath GetAssetPath(AssetId id);
		AssetId AddAssetPath(const char* path, size_t size);
		int GetAssetCount() const;

		void SetRegistryName(char name[64]);
		const char* GetRegistryName();

		void SetRegistryPath(char path[256]);
		const char* GetRegistryPath();

	private:
		char m_Name[64];
		char m_Path[256];
		eastl::vector<AssetRegistryPath> m_Pathes{};

		friend class AssetRegistrySerializer;
	};

	class AssetRegistrySerializer {
	public:
		static bool Load(AssetRegistry& registry, const char* path);
		static bool Save(AssetRegistry& registry);
	};
}
