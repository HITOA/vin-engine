#pragma once

#include "core/vinptr.hpp"

namespace Vin {
	typedef unsigned int ResourceHandle;
	typedef unsigned int ResourceTypeId;

	struct ResourceTrait {
		template<typename T>
		static constexpr ResourceTypeId GetId() {
			static ResourceTypeId id = ++lastId;
			return lastId;
		}
	private:
		static ResourceTypeId lastId;
	};

	ResourceTypeId ResourceTrait::lastId{ 0 };

	class Resource {
	public:
		Resource() : m_RefCounter{ new RefCounter{} } {
			m_RefCounter->Increment();
		};

		virtual ~Resource() {
			m_RefCounter->Decrement();
			if (m_RefCounter->Get() <= 0)
				delete m_RefCounter;
		}

		virtual void Load() = 0;
		virtual void Unload() = 0;

	private:
		ResourceHandle m_Handle;
		RefCounter* m_RefCounter;
	};
}
