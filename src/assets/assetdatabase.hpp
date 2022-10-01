#pragma once

#include "vinpch.hpp"
#include "core/base.hpp">

#include "asset.hpp"
#include "assetregistry.hpp"
#include "core/assert.hpp"

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
			AssetHolder holder{ std::make_shared<T>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}
		template<typename T>
		static Asset<T> AddAsset(std::shared_ptr<T> asset, AssetId id) {
			if (s_Database.count(id) > 0)
				return GetAsset<T>(id);
			AssetHolder holder{ std::static_pointer_cast<void>(asset), AssetTypeTrait::GetId<T>() };
			s_Database[id] = holder;
			return holder.GetAsset<T>(id);
		}

	private:
		static std::unordered_map<AssetId, AssetHolder> s_Database;
		static std::unordered_map<std::string, AssetId> s_Pathes;
		static AssetRegistry s_Registry;
	};
}
