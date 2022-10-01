#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#define BIT(x) 1 << x
#define MAX_ASSET_ID_VALUE 4294967295
#define ASSET_ID_CORE_COUNT 256

namespace Vin {
	typedef ui32 AssetId;
	typedef usize AssetTypeId;

	//static AssetId GenerateAssetId();

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
		Asset(std::shared_ptr<T> ptr, AssetId assetId) : m_Ptr{ ptr }, m_AssetId{ assetId } {};

	public:
		T* operator->() {
			return m_Ptr.get();
		}

	private:
		std::shared_ptr<T> m_Ptr{ nullptr };
		AssetId m_AssetId{ 0 };
	};

	class AssetHolder {
	public:
		AssetHolder() : m_Ptr{ nullptr }, m_TypeId{ 0 } {};
		AssetHolder(std::shared_ptr<void> ptr, AssetTypeId typeId) : m_Ptr{ ptr }, m_TypeId{ typeId } {};
		AssetHolder(AssetHolder& holder) : m_Ptr{ holder.m_Ptr }, m_TypeId{ holder.m_TypeId } {};

	public:
		template<typename T>
		Asset<T> GetAsset(AssetId assetId) {
			if (AssetTypeTrait::GetId<T>() != m_TypeId)
				return Asset<T>{ nullptr, 0 };
			return Asset<T>{ std::static_pointer_cast<T>(m_Ptr), assetId };
		};

	private:
		std::shared_ptr<void> m_Ptr{ nullptr };
		AssetTypeId m_TypeId{ 0 };
	};
}
