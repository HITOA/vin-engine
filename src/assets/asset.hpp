#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#define BIT(x) 1 << x
#define MAX_ASSET_ID_VALUE 4294967295
#define ASSET_ID_CORE_COUNT 256

namespace Vin {
	typedef usize AssetHandle;
	typedef usize AssetTypeId;

	AssetHandle GetNextAssetHandle();

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
		void SetFlags(AssetFlag flags) {
			AssetDatabase::SetAssetFlags(m_AssetHandle, flags);
		}

		T* operator->() {
			return m_Ptr.get();
		}

		operator std::shared_ptr<T>() const {
			return m_Ptr;
		}

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
		AssetHolder(AssetHolder& holder) : 
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
}
