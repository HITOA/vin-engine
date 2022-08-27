#pragma once

#include <vinbase.hpp>

namespace Vin {
	
	typedef usize EntityId;

	struct EntityIterator {
	public:
		EntityIterator(EntityId* ptr) : ptr{ ptr } {};
	public:
		inline EntityId* Get() {
			return ptr;
		}

		inline EntityId* operator->() {
			return ptr;
		}

		inline EntityId* operator++() {
			++ptr;
			return ptr;
		}
		inline EntityId* operator++(int) {
			++ptr;
			return ptr;
		}
		inline EntityId* operator--() {
			--ptr;
			return ptr;
		}
		inline EntityId* operator--(int) {
			--ptr;
			return ptr;
		}
		inline EntityId* operator+(usize idx) {
			return ptr + idx;
		}

		inline friend bool operator==(const EntityIterator& it1, const EntityIterator& it2) {
			return it1.ptr == it2.ptr;
		}
		inline friend bool operator!=(const EntityIterator& it1, const EntityIterator& it2) {
			return it1.ptr != it2.ptr;
		}
	private:
		EntityId* ptr;
	};

	class EntityManager {
	public:
		inline EntityId GetNextId() {
			return ++lastId;
		}
	private:
		EntityId lastId{ 0 };
	};

}
