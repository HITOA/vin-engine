#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "assetserdes.hpp"
#include "core/assert.hpp"

#include "logger/logger.hpp"
#include "utils/pathnormalizer.hpp"

#define BIT(x) 1 << x
#define MAX_ASSET_ID_VALUE 4294967295
#define ASSET_ID_CORE_COUNT 256
#define UNLOADUNUSED_MAX_ONCE 512

namespace Vin {
	typedef usize AssetHandle;
	typedef usize AssetTypeId;

	AssetHandle GetNextAssetHandle();

    template<typename T>
    class Asset;
    class AssetDatabase;

	struct AssetTypeTrait {
	public:
		AssetTypeId id;
		usize size;

	public:
		template<typename T>
		static const AssetTypeId GetId() {
			static const AssetTypeId id = ++lastId;
			return id;
		}
		template<typename T>
		static inline usize GetSize() {
			return sizeof(T);
		}
		template<typename T>
		static inline AssetTypeTrait GetTrait() {
			AssetTypeTrait trait{};
			trait.id = GetId<T>();
			trait.size = GetSize<T>();
			return trait;
		}
	private:
		static AssetTypeId lastId;
	};

	enum class AssetFlag {
		None = 0,
		Persistent = BIT(0)
	};

	template<typename T>
	class Asset {
	public:
		Asset() = default;
		Asset(std::shared_ptr<T> ptr, AssetHandle assethandle) : m_Ptr{ ptr }, m_AssetHandle{ assethandle } {};

	public:
		void SetFlags(AssetFlag flags);
		T* Get();
		T* operator->();
		operator std::shared_ptr<T>() const;

	private:
		std::shared_ptr<T> m_Ptr{ nullptr };
		AssetHandle m_AssetHandle{};

		friend class AssetDatabase;
	};

	class AssetHolder {
	public:
		AssetHolder() :
			m_Ptr{ nullptr }, m_TypeId{ 0 }, m_Path{}, m_Flags{ AssetFlag::None } {};
		AssetHolder(std::shared_ptr<void> ptr, AssetTypeId typeId, const std::string& path) : 
			m_Ptr{ ptr }, m_TypeId{ typeId }, m_Path{ path }, m_Flags{ AssetFlag::None } {};
		AssetHolder(const AssetHolder& holder) :
			m_Ptr{ holder.m_Ptr }, m_TypeId{ holder.m_TypeId }, m_Path{ holder.m_Path }, m_Flags{ holder.m_Flags } {};

	public:
		template<typename T>
		Asset<T> GetAsset(AssetHandle assethandle) {
			if (AssetTypeTrait::GetId<T>() != m_TypeId)
				return Asset<T>{ nullptr, 0 };
			return Asset<T>{ std::static_pointer_cast<T>(m_Ptr), assethandle };
		};

		std::string GetPath() {
			return m_Path;
		}

		usize GetRefCount() {
			return m_Ptr.use_count();
		}

		AssetFlag GetFlags() {
			return m_Flags;
		}

	private:
		std::shared_ptr<void> m_Ptr{ nullptr };
		AssetTypeId m_TypeId{ 0 };
		std::string m_Path{};
		AssetFlag m_Flags{ AssetFlag::None };

		friend class AssetDatabase;
	};

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
            if (s_AssetsPath.count(normalizedpath) > 0) {
                Logger::Warn("Asset \"{}\" is already loaded.", path);
                return GetAsset<T>(path);
            }

            std::unique_ptr<File> file = VFS::Open(normalizedpath, FileMode::Read);

            if (!file->IsValid()) {
                Logger::Err("Wasn't able to open asset : \"{}\"", normalizedpath);
                return Asset<T>{};
            }

            std::shared_ptr<T> resource = AssetDeserializer<T>::Deserialize(std::move(file));

            if (!resource) {
                Logger::Err("Wasn't able to load asset : \"{}\"", normalizedpath);
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
                if (it.second.GetRefCount() <= 1 && !((int)it.second.GetFlags() & (int)AssetFlag::Persistent)) {
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

        static void SetAssetFlags(AssetHandle handle, AssetFlag flags) {
            s_Database[handle].m_Flags = flags;
        }

        static std::unordered_map<AssetHandle, AssetHolder>::iterator begin() { return s_Database.begin(); };
        static std::unordered_map<AssetHandle, AssetHolder>::iterator end() { return s_Database.end(); };
    private:
        static std::unordered_map<AssetHandle, AssetHolder> s_Database;
        static std::unordered_map<std::string, AssetHandle> s_AssetsPath;
    };
}

template<typename T>
void Vin::Asset<T>::SetFlags(Vin::AssetFlag flags) {
    Vin::AssetDatabase::SetAssetFlags(m_AssetHandle, flags);
}

template<typename T>
T* Vin::Asset<T>::Get() {
    return m_Ptr.get();
}

template<typename T>
T* Vin::Asset<T>::operator->() {
    return m_Ptr.get();
}

template<typename T>
Vin::Asset<T>::operator std::shared_ptr<T>() const {
    return m_Ptr;
}
