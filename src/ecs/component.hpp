#pragma once

#include <vinbase.hpp>

#ifndef VINECS_COMPONENTARRAY_MINCAPACITY
#define VINECS_COMPONENTARRAY_MINCAPACITY 32
#endif

namespace Vin {
	typedef usize ComponentId;

	struct ComponentTrait {
	public:
		template<typename T>
		static const ComponentId GetId() {
			static const ComponentId id = ++lastId;
			return id;
		}
		template<typename T>
		static size_t GetSize() {
			return sizeof(T);
		}
	private:
		static ComponentId lastId;
	};

	ComponentId ComponentTrait::lastId{ 0 };

	class ComponentArray {
	public:
		ComponentArray() = delete;
		ComponentArray(usize stride);
		ComponentArray(usize stride, usize capacity);
	private:
		void* data;
		usize stride;
		usize count;
		usize capacity;
		ComponentId componentId;
	};
}
