#pragma once

#include <vinbase.hpp>

#include "archetype.hpp"

namespace Vin {
	typedef usize EntityId;

	struct EntityData {
		Archetype* archetype;
		size_t idx;
	};

	class EntityManager {
	public:
		EntityId GetNextId();
	private:
		EntityId lastId{ 0 };
	};
}
