#pragma once

#include <vinbase.hpp>
#include <allocator.hpp>
#include <cstring>

#include "vinpch.hpp"

#include "component.hpp"
#include "entity.hpp"

#ifndef VINECS_MAX_COMPONENT_BY_ENTITY
#define VINECS_MAX_COMPONENT_BY_ENTITY 64
#endif

#ifndef VINECS_MIN_COMPONENT_CONTAINER_SIZE
#define VINECS_MIN_COMPONENT_CONTAINER_SIZE 16
#endif

namespace Vin {
	typedef usize ArchetypeIdx;

	enum class ArchetypeMemoryLayout {
		Contiguous,
		Interleaved
	};

	class ArchetypeComponentLayout {
	public:
		typedef short ComponentIdx;//<0 = Not present, Any other value is the index.
	public:
		ArchetypeComponentLayout() {
			memset(indices, -1, VINECS_MAX_COMPONENT_COUNT * sizeof(ComponentIdx));
		}

		template<typename T> 
		inline void AddComponentTrait() {
			layout[count].id = ComponentTrait::GetId<T>();
			layout[count].size = ComponentTrait::GetSize<T>();
			indices[layout[count].id] = count;
			stride += layout[count].size;
			++count;
		}
		inline void AddComponentTrait(ComponentTrait trait) {
			layout[count] = trait;
			indices[layout[count].id] = count;
			stride += layout[count].size;
			++count;
		}

		inline ComponentTrait GetComponentTrait(usize idx) {
			return layout[idx];
		}

		inline ComponentIdx GetComponentIdx(AssetTypeId id) {
			return indices[id];
		}

		inline usize GetSize() {
			return count;
		}

		inline usize GetStride() {
			return stride;
		}

	private:
		ComponentIdx indices[VINECS_MAX_COMPONENT_COUNT]{ 0 };
		ComponentTrait layout[VINECS_MAX_COMPONENT_BY_ENTITY]{ 0 };
		usize count{ 0 };
		usize stride{ 0 };
	};

	template<ArchetypeMemoryLayout memlayout>
	class ArchetypeComponentContainer {
	public:
		template<typename T>
		struct Iterator {};
	};

	template<>
	class ArchetypeComponentContainer<ArchetypeMemoryLayout::Contiguous> {
	public:
		template<typename T>
		struct Iterator {
		public:
			Iterator(T* ptr) : ptr{ ptr } {};
		public:
			inline T* Get() {
				return ptr;
			}
			inline T* operator->() {
				return ptr;
			}
			inline T* operator++() {
				return ++ptr;
			}
			inline T* operator--() {
				return --ptr;
			}

			inline friend bool operator==(const Iterator& it1, const Iterator& it2) {
				return it1.ptr == it2.ptr;
			}
			inline friend bool operator!=(const Iterator& it1, const Iterator& it2) {
				return it1.ptr != it2.ptr;
			}
		private:
			T* ptr;
		};
	public:
		ArchetypeComponentContainer() = delete;
		ArchetypeComponentContainer(ArchetypeComponentLayout layout) : m_Layout{ layout } {
			m_Data = Alloc<byte*>(m_Layout.GetSize());
			m_Data[0] = Alloc<byte>(m_Layout.GetStride() * m_Capacity);

			usize currentStride{ 0 };
			for (usize i = 0; i < m_Layout.GetSize(); ++i) {
				m_Data[i] = m_Data[0] + (currentStride * m_Capacity);
				currentStride += m_Layout.GetComponentTrait(i).size;
			}
		}

		ArchetypeComponentContainer(const ArchetypeComponentContainer&) = delete;
		
		ArchetypeComponentContainer(ArchetypeComponentContainer&& rhs) noexcept : 
			m_Data{ eastl::exchange(rhs.m_Data, nullptr) },
			m_Layout{ eastl::move(rhs.m_Layout) },
			m_Count{ eastl::exchange(rhs.m_Count, 0) },
			m_Capacity{ eastl::exchange(rhs.m_Capacity, 0) } {}

		~ArchetypeComponentContainer() {
			if (m_Data == nullptr)
				return;

			Free<byte>(m_Data[0]);
			Free<byte*>(m_Data);
		}

		template<typename... Args>
		bool MatchLayout(bool permissive = false) {
			if (sizeof...(Args) != m_Layout.GetSize() && !permissive)
				return false;
			const AssetTypeId ids[sizeof...(Args)]{ ComponentTrait::GetId<Args>()... };

			for (usize i = 0; i < sizeof...(Args); ++i)
				if (m_Layout.GetComponentIdx(ids[i]) == -1)
					return false;

			return true;
		}

		template<typename T>
		usize GetComponentIdx() {
			return m_Layout.GetComponentIdx(ComponentTrait::GetId<T>());
		}

		template<typename... Args>
		usize AddComponents(Args... args) {
			if (sizeof...(args) != m_Layout.GetSize())
				return m_Count;

			const ComponentTrait traits[sizeof...(args)]{ComponentTrait::GetTrait<Args>()...};

			for (usize i = 0; i < sizeof...(args); ++i)
				if (m_Layout.GetComponentIdx(traits[i].id) == -1)
					return m_Count;

			if (m_Count == m_Capacity)
				ExpandCapacity();

			const void* datas[sizeof...(args)]{ &args... };

			for (usize i = 0; i < sizeof...(args); ++i)
				memcpy(m_Data[m_Layout.GetComponentIdx(traits[i].id)] + (traits[i].size * m_Count), datas[i], traits[i].size);

			return m_Count++;
		}

		usize AddComponents(ComponentTrait* traits, byte* datas, usize count) {
			if (count != m_Layout.GetSize())
				return m_Count;

			for (usize i = 0; i < count; ++i)
				if (m_Layout.GetComponentIdx(traits[i].id) == -1)
					return m_Count;

			if (m_Count == m_Capacity)
				ExpandCapacity();

			usize stride{ 0 };
			for (usize i = 0; i < count; ++i) {
				memcpy(m_Data[m_Layout.GetComponentIdx(traits[i].id)] + (traits[i].size * m_Count), datas + stride, traits[i].size);
				stride += traits[i].size;
			}

			return m_Count++;
		}

		void DeleteComponents(usize idx) {
			if (idx >= m_Count)
				return;

			if (idx == m_Count - 1) {
				--m_Count;
				return;
			}

			for (usize i = 0; i < m_Layout.GetSize(); ++i) {
				usize csize = m_Layout.GetComponentTrait(i).size;
				memcpy(m_Data[i] + (csize * idx), m_Data[i] + (csize * (idx + 1)), csize * (m_Count - idx));
			}
			--m_Count;
		}

		inline ArchetypeComponentLayout GetLayout() {
			return m_Layout;
		}

		inline usize GetSize() {
			return m_Count;
		}

		inline usize GetCapacity() {
			return m_Capacity;
		}

		template<typename T>
		inline Iterator<T> GetComponentIterator(usize idx = 0) {
			return Iterator<T>{ (T*)&m_Data[GetComponentIdx<T>()][sizeof(T) * idx] };
		}

		template<typename T>
		inline T* GetComponentByIdx(usize idx) {
			return (T*)&m_Data[GetComponentIdx<T>()][sizeof(T) * idx];
		}

		inline void* GetComponentRawPtr(usize componentIdx, usize pos) {
			return &m_Data[componentIdx][m_Layout.GetComponentTrait(componentIdx).size * pos];
		}
	private:
		void ExpandCapacity() {
			usize oldcapacity = m_Capacity;
			m_Capacity *= 2;

			m_Data[0] = Realloc<byte>(m_Data[0], m_Layout.GetStride() * m_Capacity);

			usize currentStride{ 0 };
			for (usize i = 0; i < m_Layout.GetSize(); ++i) {
				memcpy(m_Data[0] + (currentStride * m_Capacity),
					m_Data[0] + (currentStride * oldcapacity), m_Layout.GetComponentTrait(i).size * m_Count);

				m_Data[i] = m_Data[0] + (currentStride * m_Capacity);
				currentStride += m_Layout.GetComponentTrait(i).size;
			}
		}

	private:
		ArchetypeComponentLayout m_Layout{};
		usize m_Count{ 0 };
		usize m_Capacity{ VINECS_MIN_COMPONENT_CONTAINER_SIZE };
		byte** m_Data{ nullptr };
	};

	template<>
	class ArchetypeComponentContainer<ArchetypeMemoryLayout::Interleaved> {
	public:
		template<typename T>
		struct Iterator {

		};
	public:
		ArchetypeComponentContainer() = delete;
	private:
		usize stride;
		byte* data;
	};

	template<ArchetypeMemoryLayout memlayout>
	struct Archetype {
		ArchetypeComponentContainer<memlayout> archetype{};
		eastl::vector<EntityId> entityIds{};
		eastl::hash_map<EntityId, usize> entityidx{};

		Archetype(ArchetypeComponentLayout layout) : archetype{ layout } {};
	};

	template<Vin::ArchetypeMemoryLayout memlayout, typename T>
	using _Iterator = typename Vin::ArchetypeComponentContainer<memlayout>::Iterator<T>;

	template<typename T, Vin::ArchetypeMemoryLayout memlayout>
	using Iterator = _Iterator<memlayout, T>;

	/*template<ArchetypeMemoryLayout memlayout>
	struct Archetype {
		ArchetypeComponentContainer<memlayout> components{};

		Archetype() = delete;
		Archetype(ArchetypeComponentLayout layout) : components{ layout } {};
	};*/

	//Archetype<ArchetypeMemoryLayout::Interleaved> archetype{}

}
