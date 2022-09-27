#pragma once

#include "vinpch.hpp"
#include <vinbase.hpp>

#include "asset.hpp"
#include "assetregistry.hpp"
#include "assert.hpp"

namespace Vin {

	class AssetDatabase {
	public:
		static void AddRegistry(AssetRegistry&& registry); //Add registry from memory
		static void AddRegistry(const char* path); //Add Registry from file

		template<typename T>
		static Asset<T> GetAsset(AssetId id) {
			if (s_Database.count(id) <= 0) {
				VIN_SOFT_ASSERT(false, "No asset with provided id is loaded.");
				return Asset<T>{ nullptr, 0 };
			}
			return s_Database[id].GetAsset<T>(id);
		}
		template<typename T>
		static Asset<T> GetAsset(const char* path) {
			AssetHeader header{};
			if (!AssetFileSerDes::GetHeader(header, path)) {
				VIN_SOFT_ASSERT(false, "Not able to read asset header.");
				return Asset<T>{ nullptr, 0 };
			}
			return GetAsset<T>(header.id);
		}

		template<typename T>
		static bool LoadAsset(AssetId id) {
			for (AssetRegistry& registry : m_Registries) {
				if (registry.GetOffset() <= id) {
					AssetRegistryPath path = registry.GetPath(id);
					if (LoadAsset(path.path))
						return true;
				}
			}
			VIN_SOFT_ASSERT(false, "No asset on disk with provided id.");
			return false;
		}
		template<typename T>
		static bool LoadAsset(const char* path) {

		}

		template<typename T>
		static Asset<T> AddAsset(T&& asset, AssetId id) {
			AssetHolder holder{ eastl::make_shared<T>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}
		template<typename T>
		static Asset<T> AddAsset(eastl::shared_ptr<T> asset, AssetId id) {
			AssetHolder holder{ eastl::static_shared_pointer_cast<void>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}

	private:
		static eastl::hash_map<AssetId, AssetHolder> s_Database;
		static eastl::hash_map<eastl::string, AssetId> s_Pathes;
		static eastl::fixed_vector<AssetRegistry, 8> s_Registries;
	};
}
