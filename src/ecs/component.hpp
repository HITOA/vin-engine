#pragma once

#include <vinbase.hpp>

#define VINECS_MAX_COMPONENT_COUNT 512

namespace Vin {
	typedef usize ComponentId;

	struct ComponentTrait {
	public:
		ComponentId id;
		usize size;

	public:
		template<typename T>
		static const ComponentId GetId() {
			static const ComponentId id = ++lastId;
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
		static ComponentId lastId;
	};

	ComponentId ComponentTrait::lastId{ 0 };

	
}
