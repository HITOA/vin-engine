#pragma once

#include "vinpch.hpp"

#include "archetype.hpp"
#include "entity.hpp"

namespace Vin {

	class Registry {
	public:
		EntityId CreateEntity() {
			EntityId id = m_EntityManager.GetNextId();
			EntityData data{};
			data.archetype = nullptr;
			data.idx = -1;

			m_Registry[id] = data;
			return id;
		}

		template<typename T>
		T* GetComponent(EntityId id) {
			ComponentId componentId = ComponentTrait::GetId<T>();
			EntityData entity = m_Registry[id];
			Archetype* archetype = entity.archetype;


		}
	private:
		EntityManager m_EntityManager{};

		eastl::vector<Archetype> m_Archetypes{};
		eastl::hash_map<EntityId, EntityData> m_Registry{};
	};

}
