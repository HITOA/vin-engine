#pragma once

#include "vinpch.hpp"

namespace Vin {

	typedef usize AssetId;

	struct AssetTrait {
	public:
		AssetId id;
		usize size;

	public:
		template<typename T>
		static const AssetId GetId() {
			static const AssetId id = ++lastId;
			return id;
		}
		template<typename T>
		static inline usize GetSize() {
			return sizeof(T);
		}
		template<typename T>
		static inline AssetTrait GetTrait() {
			AssetTrait trait{};
			trait.id = GetId<T>();
			trait.size = GetSize<T>();
			return trait;
		}

	private:
		static AssetId lastId;
	};

	template<typename T>
	class Asset {
	public:
		Asset() : ptr{ nullptr } {};
		Asset(T* ptr) : ptr{ ptr } {};
		Asset(eastl::shared_ptr<T> ptr) : ptr{ ptr } {};

		operator bool() {
			return ptr.get() != nullptr;
		}

		T* operator->() {
			return ptr.get();
		}
		T operator*() {
			return *ptr.get();
		}
	private:
		eastl::shared_ptr<T> ptr;
	};

	class AssetHolder {
	public:
		AssetHolder(eastl::shared_ptr<void> ptr, AssetTrait trait) : ptr{ ptr }, trait{ trait } {};

		template<typename T>
		Asset<T> GetAsset() {
			if (AssetTrait::GetId<T>() != trait.id)
				return nullptr;
			return eastl::static_shared_pointer_cast<T>(ptr);
		}
	private:
		eastl::shared_ptr<void> ptr;
		AssetTrait trait;
	};

	class AssetDatabase {
	public:
		AssetDatabase() : m_Assets{} {};

		template<typename T>
		Asset<T> GetAsset(const char* path) {
			if (m_Assets.count(path) == 0)
				return nullptr;
			return m_Assets.at(path).GetAsset<T>();
		}
		template<typename T>
		Asset<T> CreateAsset(const char* path, T asset) {
			if (m_Assets.count(path) != 0)
				return GetAsset<T>(path);
			eastl::shared_ptr<T> ptr = eastl::make_shared<T>(asset); //Copy constructor
			m_Assets.insert(eastl::pair<eastl::string, AssetHolder>{ path, AssetHolder{ ptr, AssetTrait::GetTrait<T>() } });
			return ptr;
		}
	private:
		eastl::hash_map<eastl::string, AssetHolder> m_Assets{};
	};
}
