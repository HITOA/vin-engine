#pragma once

#include "vinpch.hpp"

#include "query.hpp"
#include "entity.hpp"
#include "archetype.hpp"

#include <optick.h>

/*template<size_t N>
struct std::hash<std::bitset<N>> {
	size_t operator()(std::bitset<N> v) const {
		return 0;
	}
};*/

namespace Vin {

	class Registry {
	public:
		
		template<typename... Args>
		EntityId CreateEntity(Args... args) {
			EntityId entityId = m_EntityManager.GetNextId();
			ArchetypeIdx archetypeIdx = AddEntityComponents(args...);

			m_EntityArchetypeMap[entityId] = archetypeIdx;
			m_Archetypes[archetypeIdx].entityidx[entityId] = m_Archetypes[archetypeIdx].archetype.GetSize() - 1;
			m_Archetypes[archetypeIdx].entityIds.push_back(entityId);

			return entityId;
		}

		void DeleteEntity(EntityId entityId) {
			auto it = m_EntityArchetypeMap.find(entityId);
			if (it == m_EntityArchetypeMap.end())
				return;

			ArchetypeIdx archetypeIdx = it->second;
			m_EntityArchetypeMap.erase(it);
			usize entityIdx = m_Archetypes[archetypeIdx].entityidx[entityId];
			m_Archetypes[archetypeIdx].entityidx.erase(entityId);
			m_Archetypes[archetypeIdx].archetype.DeleteComponents(entityIdx);
			m_Archetypes[archetypeIdx].entityIds.erase(m_Archetypes[archetypeIdx].entityIds.begin() + entityIdx);

			for (auto& it2 : m_Archetypes[archetypeIdx].entityidx)
				if (it2.second > entityIdx)
					--it2.second;
		}

		template<typename RetType, typename... Components, typename... Args>
		void Process(RetType(system)(Query<Components...>, Args...), Args&... args) {
			OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Scene);
			auto itend = m_Archetypes.end();
			for (auto it = m_Archetypes.begin(); it != itend; ++it) {
				if (it->archetype.template MatchLayout<Components...>(true)) {
					system(Query<Components...>{
						it->archetype.template GetComponentIterator<Components>()...,
						it->archetype.GetSize() }, args...);
				}
			}
		}

		template<typename RetType, typename... Components, typename... Args>
		void Process(RetType(system)(Query<EntityId, Components...>, Args...), Args&... args) {
			OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Scene);
			auto itend = m_Archetypes.end();
			for (auto it = m_Archetypes.begin(); it != itend; ++it) {
				if (it->archetype.template MatchLayout<Components...>(true)) {
					system(Query<EntityId, Components...>{
						(EntityId*)it->entityIds.data(),
						it->archetype.template GetComponentIterator<Components>()...,
						it->archetype.GetSize() }, args...);
				}
			}
		}

		template<typename RetType, typename... Components, typename... Args>
		void Process(RetType(system)(Registry& registry, Query<Components...>, Args...), Args&... args) {
			OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Scene);
			auto itend = m_Archetypes.end();
			for (auto it = m_Archetypes.begin(); it != itend; ++it) {
				if (it->archetype.template MatchLayout<Components...>(true)) {
					system(*this, Query<Components...>{
						it->archetype.template GetComponentIterator<Components>()...,
							it->archetype.GetSize() }, args...);
				}
			}
		}

		template<typename RetType, typename... Components, typename... Args>
		void Process(RetType(system)(Registry& registry, Query<EntityId, Components...>, Args...), Args&... args) {
			OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Scene);
			auto itend = m_Archetypes.end();
			for (auto it = m_Archetypes.begin(); it != itend; ++it) {
				if (it->archetype.template MatchLayout<Components...>(true)) {
					system(*this, Query<EntityId, Components...>{
						(EntityId*)it->entityIds.data(),
							it->archetype.template GetComponentIterator<Components>()...,
							it->archetype.GetSize() }, args...);
				}
			}
		}

		template<typename T>
		T* GetComponent(EntityId entityId) {
			if (m_EntityArchetypeMap.count(entityId) == 0)
				return nullptr;
			ArchetypeIdx archetypeIdx = m_EntityArchetypeMap[entityId];
			usize componentIdx = m_Archetypes[archetypeIdx].archetype.template GetComponentIdx<T>();
			if (componentIdx == -1)
				return nullptr;
			return m_Archetypes[archetypeIdx].archetype.template GetComponentByIdx<T>(m_Archetypes[archetypeIdx].entityidx[entityId]);
		}

		template<typename T>
		T* AddComponent(EntityId entityId, T component) {
			if (m_EntityArchetypeMap.count(entityId) == 0)
				return nullptr;
			ArchetypeIdx archetypeIdx = m_EntityArchetypeMap[entityId];
			usize componentIdx = m_Archetypes[archetypeIdx].archetype.template GetComponentIdx<T>();
			if (componentIdx != -1) {
				T* ptr = m_Archetypes[archetypeIdx].archetype.template GetComponentByIdx<T>(m_Archetypes[archetypeIdx].entityidx[entityId]);
				memcpy(ptr, &component, sizeof(T));
				return ptr;
			}

			ArchetypeComponentLayout layout = m_Archetypes[archetypeIdx].archetype.GetLayout();
			
			usize entityIdx = m_Archetypes[archetypeIdx].entityidx[entityId];
			usize count = layout.GetSize() + 1;
			ComponentTrait* traits = Alloc<ComponentTrait>(count);
			byte* datas = Alloc<byte>(layout.GetStride() + sizeof(T));

			usize stride{ 0 };
			for (usize i = 0; i < count - 1; ++i) {
				traits[i] = layout.GetComponentTrait(i);
				memcpy(datas + stride, m_Archetypes[archetypeIdx].archetype.GetComponentRawPtr(i, entityIdx), traits[i].size);
				stride += traits[i].size;
			}
			traits[count - 1] = ComponentTrait::GetTrait<T>();
			memcpy(datas + stride, &component, sizeof(T));

			DeleteEntity(entityId);
			archetypeIdx = AddEntityComponents(traits, datas, count);

			m_EntityArchetypeMap[entityId] = archetypeIdx;
			m_Archetypes[archetypeIdx].entityidx[entityId] = m_Archetypes[archetypeIdx].archetype.GetSize() - 1;
			m_Archetypes[archetypeIdx].entityIds.push_back(entityId);

			Free<ComponentTrait>(traits);
			Free<byte>(datas);

			return m_Archetypes[archetypeIdx].archetype.template GetComponentByIdx<T>(m_Archetypes[archetypeIdx].archetype.GetSize() - 1);
		}

		template<typename T>
		void DeleteComponent(EntityId entityId) {
			if (m_EntityArchetypeMap.count(entityId) == 0)
				return;
			ArchetypeIdx archetypeIdx = m_EntityArchetypeMap[entityId];

			ArchetypeComponentLayout layout = m_Archetypes[archetypeIdx].archetype.GetLayout();

			usize entityIdx = m_Archetypes[archetypeIdx].entityidx[entityId];
			usize count = layout.GetSize() - 1;
			ComponentTrait* traits = Alloc<ComponentTrait>(count);

			usize stride{ 0 };
			for (usize i = 0, j = 0; i < layout.GetSize(); ++i) {
				if (layout.GetComponentTrait(i).id == ComponentTrait::GetId<T>())
					continue;

				traits[j] = layout.GetComponentTrait(i);	
				stride += traits[j].size;
				++j;
			}

			byte* datas = Alloc<byte>(stride);
			stride = 0;
			for (usize i = 0; i < count; ++i) {
				memcpy(datas + stride, m_Archetypes[archetypeIdx].archetype.GetComponentRawPtr(
					layout.GetComponentIdx(traits[i].id), entityIdx), traits[i].size);
				stride += traits[i].size;
			}

			DeleteEntity(entityId);
			archetypeIdx = AddEntityComponents(traits, datas, count);

			m_EntityArchetypeMap[entityId] = archetypeIdx;
			m_Archetypes[archetypeIdx].entityidx[entityId] = m_Archetypes[archetypeIdx].archetype.GetSize() - 1;
			m_Archetypes[archetypeIdx].entityIds.push_back(entityId);

			Free<ComponentTrait>(traits);
			Free<byte>(datas);
		}
	private:
		
		/* Add components to the corresponding Archetype, Create the archetype if it doesn't exists. */
		template<typename... Args>
		inline ArchetypeIdx AddEntityComponents(Args... args) {
			const AssetTypeId ids[sizeof...(Args)]{ ComponentTrait::GetId<Args>()... };
			std::bitset<VINECS_MAX_COMPONENT_COUNT> archetypeId{};

			for (AssetTypeId id : ids)
				archetypeId[id] = true;

			if (m_ArchetypeMap.count(archetypeId) > 0) {
				ArchetypeIdx idx = m_ArchetypeMap[archetypeId];
				m_Archetypes[idx].archetype.AddComponents(args...);
				return idx;
			}

			const ComponentTrait traits[sizeof...(args)]{ ComponentTrait::GetTrait<Args>()... };

			ArchetypeComponentLayout layout{};
			for (usize i = 0; i < sizeof...(args); ++i)
				layout.AddComponentTrait(traits[i]);

			m_Archetypes.emplace_back(layout);
			m_Archetypes[m_Archetypes.size() - 1].archetype.AddComponents(args...);

			m_ArchetypeMap[archetypeId] = (ArchetypeIdx)(m_Archetypes.size() - 1);

			return (ArchetypeIdx)(m_Archetypes.size() - 1);
		}

		inline ArchetypeIdx AddEntityComponents(ComponentTrait* traits, byte* datas, usize count) {
			std::bitset<VINECS_MAX_COMPONENT_COUNT> archetypeId{};

			for (usize i = 0; i < count; i++)
				archetypeId[traits[i].id] = true;

			if (m_ArchetypeMap.count(archetypeId) > 0) {
				ArchetypeIdx idx = m_ArchetypeMap[archetypeId];
				m_Archetypes[idx].archetype.AddComponents(traits, datas, count);
				return idx;
			}

			ArchetypeComponentLayout layout{};
			for (usize i = 0; i < count; ++i)
				layout.AddComponentTrait(traits[i]);

			m_Archetypes.emplace_back(layout);
			m_Archetypes[m_Archetypes.size() - 1].archetype.AddComponents(traits, datas, count);

			m_ArchetypeMap[archetypeId] = (ArchetypeIdx)(m_Archetypes.size() - 1);

			return (ArchetypeIdx)(m_Archetypes.size() - 1);
		}
	private:
		EntityManager m_EntityManager{};

		std::vector<Archetype> m_Archetypes{};
		std::unordered_map<std::bitset<VINECS_MAX_COMPONENT_COUNT>, ArchetypeIdx> m_ArchetypeMap{};
		std::unordered_map<EntityId, ArchetypeIdx> m_EntityArchetypeMap{};
	};

}
