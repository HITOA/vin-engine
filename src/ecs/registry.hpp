#pragma once

#include "vinpch.hpp"

#include "entity.hpp"
#include "archetype.hpp"

namespace Vin {

	template<ArchetypeMemoryLayout memlayout>
	struct ArchetypeEntities {
		ArchetypeComponentContainer<memlayout> archetype{};
		eastl::hash_map<EntityId, usize> entityidx{};
	};

	template<ArchetypeMemoryLayout memlayout>
	class Registry {
	public:
		
		template<typename... Args>
		EntityId CreateEntity(Args... args) {
			EntityId entityId = m_EntityManager.GetNextId();
			ArchetypeIdx archetypeId = AddEntityComponents(args...);

			m_EntityArchetypeMap[entityId] = archetypeId;

			return entityId;
		}
	private:
		
		/* Add components to the corresponding Archetype, Create the archetype if it doesn't exists. */
		template<typename... Args>
		inline ArchetypeIdx AddEntityComponents(Args... args) {
			ArchetypeId id = ArchetypeTrait::GetId<Args...>();

			if (m_ArchetypeMap.count(id) > 0) {
				ArchetypeIdx idx = m_ArchetypeMap[id];
				m_Archetypes[idx].AddComponents(args...);
				return idx;
			}
			/*{
				auto itend = m_Archetypes.end();
				ArchetypeIdx id = 0;
				for (auto it = m_Archetypes.begin(); it != itend; ++it) {
					if (it->MatchLayout<Args...>()) { //Might change (Likely)
						it->AddComponents(args...);
						return id;
					}
					++id;
				}
			}*/

			const ComponentTrait traits[sizeof...(args)]{ ComponentTrait::GetTrait<Args>()... };

			ArchetypeComponentLayout layout{};
			for (usize i = 0; i < sizeof...(args); ++i)
				layout.AddComponentTrait(traits[i]);

			m_Archetypes.emplace_back(layout);
			m_Archetypes[m_Archetypes.size() - 1].AddComponents(args...);

			m_ArchetypeMap[id] = m_Archetypes.size() - 1;

			return m_Archetypes.size() - 1;
		}
	private:
		EntityManager m_EntityManager{};

		eastl::vector<ArchetypeComponentContainer<memlayout>> m_Archetypes{};
		eastl::hash_map<ArchetypeId, ArchetypeIdx> m_ArchetypeMap{};
		eastl::hash_map<EntityId, ArchetypeIdx> m_EntityArchetypeMap{};
	};

}
