#pragma once

#include "vinpch.hpp"

namespace Vin {

	/**
	 * Any allocator should follow this.
	 */
	template<typename T>
	class DummyAllocator {
	public:
		using ValueType = T;

	public:
		static T* Allocate(size_t n) { return nullptr; };
		static T* AllocateAligned(size_t n, size_t align) { return nullptr; };

		static T* Reallocate(T* ptr, size_t n) { return nullptr; };
		static T* ReallocateAligned(T* ptr, size_t n, size_t align) { return nullptr; };

		static void Free(T* ptr) {};
		static void FreeAligned(T* ptr) {};

		template<typename... Args>
		static void Construct(T* ptr, Args&&... args) {};
		static void Destroy(T* ptr) {};
	};

	/**
	 * Default allocator with classic malloc
	 */
	template<typename T>
	class DefaultAllocator {
	public:
		using ValueType = T;

	public:
		static T* Allocate(size_t n) {
			return (T*)malloc(sizeof(T) * n);
		}
		static T* AllocateAligned(size_t n, size_t align) {
			return (T*)_aligned_malloc(sizeof(T) * n, align);
		}

		static T* Reallocate(T* ptr, size_t n) {
			return (T*)realloc(ptr, sizeof(T) * n);
		};
		static T* ReallocateAligned(T* ptr, size_t n, size_t align) {
			return (T*)_aligned_realloc(ptr, sizeof(T) * n, align);
		}

		static void Free(T* ptr) {
			free(ptr);
		}
		static void FreeAligned(T* ptr) {
			_aligned_free(ptr);
		}

		template<typename... Args>
		static void Construct(T* ptr, Args&&... args) {
			new(ptr) T{ args... };
		};
		static void Destroy(T* ptr) {
			ptr->~T();
		};
	};

}
