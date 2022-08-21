#pragma once

#include <vinbase.hpp>

namespace Vin {
	typedef usize SystemId;

	struct SystemTrait {
	public:
		template<typename T>
		static const SystemId GetId() {
			static const SystemId id = ++lastId;
			return id;
		}
		template<typename T>
		static size_t GetSize() {
			return sizeof(T);
		}
	private:
		static SystemId lastId;
	};

	SystemId SystemTrait::lastId{ 0 };
}
