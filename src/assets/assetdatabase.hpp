#pragma once

#include "vinpch.hpp"
#include <vinbase.hpp>

#include "asset.hpp"
#include "assetregistry.hpp"

namespace Vin {

	class AssetDatabase {
	public:
		AssetDatabase() = default;

	public:
		void AddRegistry(AssetRegistry&& registry); //Add registry from memory
		void AddRegistry(const char* path); //Add Registry from file

		template<typename T>
		Asset<T> GetAsset(AssetId id) {
			if (m_Database.count(id) <= 0)
				return Asset<T>{ nullptr, 0 };
			return m_Database[id].GetAsset<T>(id);
		}

		template<typename T>
		Asset<T> GetAsset(const char* path); //No Optimal

		template<typename T>
		Asset<T> AddAsset(T&& asset);
		template<typename T>
		Asset<T> AddAsset(eastl::shared_ptr<T> asset);

	private:
		eastl::hash_map<AssetId, AssetHolder> m_Database{};
		eastl::fixed_vector<AssetRegistry, 8> m_Registries{};
	};
}
