#pragma once

#include <vinbase.hpp>

#include "archetype.hpp"

namespace Vin {
	
	typedef usize EntityId;

	struct Entity {
		ArchetypeId id;
		usize pos;
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
