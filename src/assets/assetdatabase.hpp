#pragma once

#include "vinpch.hpp"
#include <vinbase.hpp>

#include "asset.hpp"
#include "assetregistry.hpp"
#include "assert.hpp"

#include "logger/logger.hpp"

namespace Vin {

	class AssetDatabase {
	public:
		static bool LoadRegistry(const char* path);
		static bool SaveRegistry();
		static AssetRegistry& GetRegistry();

		template<typename T>
		static Asset<T> GetAsset(AssetId id) {
			if (s_Database.count(id) <= 0) {
				Logger::Log("Asset {} is not loaded.", id);
				return Asset<T>{};
			}
			return s_Database[id].GetAsset<T>(id);
		}
		template<typename T>
		static Asset<T> GetAsset(const char* path) {
			if (s_Pathes.count(path) <= 0) {
				Logger::Log("Asset {} is not loaded.", path);
				return Asset<T>{};
			}
			return GetAsset<T>(s_Pathes[id]);
		}

		template<typename T>
		static bool LoadAsset(AssetId id) {
			
		}
		template<typename T>
		static bool LoadAsset(const char* path) {

		}

		template<typename T>
		static Asset<T> AddAsset(T&& asset, AssetId id) {
			if (s_Database.count(id) > 0)
				return GetAsset<T>(id);
			AssetHolder holder{ eastl::make_shared<T>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}
		template<typename T>
		static Asset<T> AddAsset(eastl::shared_ptr<T> asset, AssetId id) {
			if (s_Database.count(id) > 0)
				return GetAsset<T>(id);
			AssetHolder holder{ eastl::static_shared_pointer_cast<void>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}

	private:
		static eastl::hash_map<AssetId, AssetHolder> s_Database;
		static eastl::hash_map<eastl::string, AssetId> s_Pathes;
		static AssetRegistry s_Registry;
	};
}
