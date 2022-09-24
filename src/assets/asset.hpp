#pragma once

#include "vinpch.hpp"
#include <vinbase.hpp>

#define BIT(x) 1 << x
#define ASSET_MAGIC "\xE9\x23\xE6\x0E"

namespace Vin {
	typedef ui32 AssetId;
	typedef usize AssetTypeId;

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

	struct AssetHeader {
		char magic[4];
		AssetId id;
		uint32_t depCount;
	};

	template<typename T>
	class Asset {
	public:
		Asset() = delete;
		Asset(eastl::shared_ptr<T> ptr, AssetId assetId) : m_Ptr{ ptr }, m_AssetId{ assetId } {};

	public:
		T* operator->() {
			return m_Ptr.get();
		}

	private:
		eastl::shared_ptr<T> m_Ptr{ nullptr };
		AssetId m_AssetId{ 0 };
	};

	class AssetHolder {
	public:
		template<typename T>
		Asset<T> GetAsset(AssetId assetId) {
			if (AssetTypeTrait::GetId<T>() != m_TypeId)
				return Asset<T>{ nullptr, 0 };
			return Asset<T>{ eastl::static_shared_pointer_cast<T>(m_Ptr), assetId };
		};

	private:
		eastl::shared_ptr<void> m_Ptr{ nullptr };
		AssetTypeId m_TypeId{ 0 };
	};
}
