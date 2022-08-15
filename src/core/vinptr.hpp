#pragma once

#include "allocator.hpp"

namespace Vin {

	class RefCounter {
	public:
		RefCounter() = default;
		RefCounter(const RefCounter&) = delete;

		void Increment();
		void Decrement();

		unsigned short Get() const;

		void operator++();
		void operator++(int);
		void operator--();
		void operator--(int);

	private:
		unsigned short m_RefCounter{ 0 };
	};

	template<typename T, typename Allocator = DefaultAllocator<T>>
	class Ref {
	public:
		Ref() = delete;
		Ref(T* ptr) : m_Ptr{ ptr } {
			m_RefCounter = new RefCounter{};
			m_RefCounter->Increment();
		}
		Ref(const Ref& ref) {
			m_Ptr = ref.m_Ptr;
			m_RefCounter = ref.m_RefCounter;
			m_RefCounter->Increment();
		}
		~Ref() {
			m_RefCounter->Decrement();
			if (m_RefCounter->Get() <= 0) {
				Allocator::Destroy(m_Ptr);
				Allocator::Free(m_Ptr);
			}
			delete m_RefCounter;
		}

		T* Get() {
			return m_Ptr;
		}

		T& operator*() {
			return *m_Ptr;
		}
		T* operator->() {
			return m_Ptr;
		}

	private:
		T* m_Ptr;
		RefCounter* m_RefCounter;
	};

	template<typename T, typename... Args, typename Allocator = DefaultAllocator<T>>
	Ref<T> MakeRef(Args&&... args) {
		T* ptr = Allocator::Allocate(1);
		Allocator::Construct(ptr, args...);
		return Ref<T>{ ptr };
	}
}
