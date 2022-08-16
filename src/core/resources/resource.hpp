#pragma once

/*#include "core/allocator.hpp"
#include "core/vinptr.hpp"

namespace Vin {
	typedef unsigned int ResourceHandle;
	typedef unsigned int ResourceTypeId;

	struct ResourceTrait {
		template<typename T>
		static constexpr ResourceTypeId GetTypeId() {
			static ResourceTypeId id = ++lastId;
			return lastId;
		}
	private:
		static ResourceTypeId lastId;
	};

	ResourceTypeId ResourceTrait::lastId{ 0 };

	template<typename Allocator = DefaultAllocator>
	class Resource {
	public:
		Resource() : m_RefCounter{ Allocator::New<RefCounter>() } {
			m_RefCounter->Increment();
		};

		virtual ~Resource() {
			m_RefCounter->Decrement();
			if (m_RefCounter->Get() <= 0)
				Allocator::Delete(m_RefCounter);
		}

		virtual void Load(const char* path) = 0;
		virtual void Unload() = 0;

		virtual Resource* Clone() = 0;

	private:
		const char* m_Path;
		ResourceHandle m_Handle;
		ResourceTypeId m_TypeId;
		RefCounter* m_RefCounter;

		template<typename U>
		friend class ResourceManager;
	};
}
*/