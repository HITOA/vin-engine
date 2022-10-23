#pragma once

#include <new>

#ifndef VIN_ALLOCATOR
#define VIN_ALLOCATOR VinDefaultAllocator
#endif

#ifndef VIN_DEFAULT_MEMORY_ALIGNEMENT
#define VIN_DEFAULT_MEMORY_ALIGNEMENT 16
#endif

namespace Vin {
	class VinDefaultAllocator {
	public:
		static void* Alloc(size_t size) {
			return AllocAligned(size, VIN_DEFAULT_MEMORY_ALIGNEMENT);
		}
		static void* AllocAligned(size_t size, size_t alignement);

		static void Free(void* ptr) {
			return FreeAligned(ptr, VIN_DEFAULT_MEMORY_ALIGNEMENT);
		}
		static void FreeAligned(void* ptr, size_t alignement);

		static void* Realloc(void* ptr, size_t size) {
			return ReallocAligned(ptr, size, VIN_DEFAULT_MEMORY_ALIGNEMENT);
		}
		static void* ReallocAligned(void* ptr, size_t size, size_t alignement);
	};

	/**
	 * @brief Alloc memory for n elements of type T with default alignement.
	*/
	template<typename T>
	T* Alloc(size_t n) {
		return (T*)VIN_ALLOCATOR::Alloc(sizeof(T) * n);
	}
	/**
	 * @brief Alloc memory for n elements of type T with specified alignement.
	*/
	template<typename T>
	T* AllocAligned(size_t n, size_t alignement) {
		return (T*)VIN_ALLOCATOR::AllocAligned(sizeof(T) * n, alignement);
	}

	/**
	 * @brief Free memory with default alignement.
	*/
	template<typename T>
	void Free(T* p) {
		VIN_ALLOCATOR::Free(p);
	}
	/**
	 * @brief Free memory with specified alignement.
	*/
	template<typename T>
	void FreeAligned(T* p, size_t alignement) {
		VIN_ALLOCATOR::FreeAligned(p, alignement);
	}

	/**
	 * @brief Try to realloc memory for n elements of type T with default alignement.
	*/
	template<typename T>
	T* Realloc(T* ptr, size_t n) {
		return (T*)VIN_ALLOCATOR::Realloc(ptr, sizeof(T) * n);
	}
	/**
	 * @brief Try to realloc memory for n elements of type T with specified alignement.
	*/
	template<typename T>
	T* ReallocAligned(T* ptr, size_t n, size_t alignement) {
		return (T*)VIN_ALLOCATOR::ReallocAligned(ptr, sizeof(T) * n, alignement);
	}

	template<typename T, typename... Args>
	void Construct(T* ptr, Args... args) {
		new(ptr) T{ args... };
	}

	template<typename T>
	void Deconstruct(T* ptr) {
		T->~T();
	}
}