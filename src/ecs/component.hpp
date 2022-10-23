#pragma once

#include "core/base.hpp"

#define VINECS_MAX_COMPONENT_COUNT 512

namespace Vin {
	typedef usize AssetTypeId;

	struct ComponentTrait {
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
		static inline ComponentTrait GetTrait() {
			ComponentTrait trait{};
			trait.id = GetId<T>();
			trait.size = GetSize<T>();
			return trait;
		}
	private:
		static AssetTypeId lastId;
	};
}
