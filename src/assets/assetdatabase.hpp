#pragma once

#include "vinpch.hpp"
#include "core/base.hpp">

#include "asset.hpp"
#include "assetserdes.hpp"
#include "core/assert.hpp"

#include "logger/logger.hpp"
#include "utils/pathnormalizer.hpp"

#define UNLOADUNUSED_MAX_ONCE 512

namespace Vin {

	class AssetDatabase {
	public:
		template<typename T>
		static Asset<T> GetAsset(AssetHandle handle) {
			if (s_Database.count(handle) <= 0) {
				Logger::Log("No asset with handle \"{}\".", handle);
				return Asset<T>{};
			}
			return s_Database[handle].GetAsset<T>(handle);
		}
		template<typename T>
		static Asset<T> GetAsset(const std::string& path) {
			std::string normalizedpath = NormalizeStringPath(path);
			if (s_AssetsPath.count(normalizedpath) <= 0) {
				Logger::Log("Asset \"{}\" is not loaded.", normalizedpath);
				return Asset<T>{};
			}
			return GetAsset<T>(s_AssetsPath[normalizedpath]);
		}

		template<typename T>
		static Asset<T> LoadAsset(const std::string& path) {
			std::string normalizedpath = NormalizeStringPath(path);
			if (s_AssetsPath.count(normalizedpath) > 0)
				return GetAsset<T>(path);

			std::unique_ptr<File> file = VFS::Open(normalizedpath, FileMode::Read);

			if (!file->IsValid()) {
				Logger::Log("Wasn't able to open asset : \"{}\"", normalizedpath);
				return Asset<T>{};
			}

			std::shared_ptr<T> resource = AssetDeserializer<T>::Deserialize(std::move(file));

			if (!resource) {
				Logger::Log("Wasn't able to load asset : \"{}\"", normalizedpath);
				return Asset<T>{};
			}

			return AddAsset<T>(resource, normalizedpath);
		}

		template<typename T>
		static Asset<T> AddAsset(T&& asset, const std::string& path) {
			std::string normalizedpath = NormalizeStringPath(path);
			if (s_AssetsPath.count(normalizedpath) > 0)
				return GetAsset<T>(normalizedpath);
			AssetHolder holder{ std::make_shared<T>(asset), AssetTypeTrait::GetId<T>(), normalizedpath };
			AssetHandle handle = GetNextAssetHandle();
			s_Database[handle] = holder;
			s_AssetsPath[normalizedpath] = handle;
			return holder.GetAsset<T>(handle);
		}
		template<typename T>
		static Asset<T> AddAsset(std::shared_ptr<T> asset, const std::string& path) {
			std::string normalizedpath = NormalizeStringPath(path);
			if (s_AssetsPath.count(normalizedpath) > 0)
				return GetAsset<T>(normalizedpath);
			AssetHolder holder{ std::static_pointer_cast<void>(asset), AssetTypeTrait::GetId<T>(), normalizedpath };
			AssetHandle handle = GetNextAssetHandle();
			s_Database[handle] = holder;
			s_AssetsPath[normalizedpath] = handle;
			return holder.GetAsset<T>(handle);
		}

		static void Unload(AssetHandle handle) {
			if (s_Database.find(handle) == s_Database.end()) {
				Logger::Err("Couldn't unload asset with handle {} : asset does not exists.", handle);
				return;
			}

			AssetHolder& holder = s_Database[handle];

			s_AssetsPath.erase(holder.GetPath());
			s_Database.erase(handle);
		}

		template<typename T>
		static void Unload(const Asset<T>& asset) {
			Unload(asset.m_AssetHandle);
		}

		static void UnloadUnused() {
			static AssetHandle handlebuff[UNLOADUNUSED_MAX_ONCE]{};
			size_t idx = 0;
			for (auto& it : s_Database) {
				if (it.second.GetRefCount() <= 1) {
					handlebuff[idx] = it.first;
					idx++;
				}
				if (idx >= UNLOADUNUSED_MAX_ONCE)
					break;
			}

			for (size_t i = 0; i < idx; i++) {
				AssetHolder& holder = s_Database[handlebuff[i]];

				s_AssetsPath.erase(holder.GetPath());
				s_Database.erase(handlebuff[i]);;
			}
		}

		static size_t GetAssetCount() {
			return s_Database.size();
		}
	private:
		static std::unordered_map<AssetHandle, AssetHolder> s_Database;
		static std::unordered_map<std::string, AssetHandle> s_AssetsPath;
	};
}
