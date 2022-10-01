#pragma once

#include "core/base.hpp">

namespace Vin {
	
	typedef usize EntityId;

	class EntityManager {
	public:
		inline EntityId GetNextId() {
			return ++lastId;
		}
	private:
		EntityId lastId{ 0 };
	};

}
