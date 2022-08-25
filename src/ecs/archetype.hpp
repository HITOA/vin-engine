#pragma once

#include <vinbase.hpp>
#include <allocator.hpp>
#include <cstring>

#include "component.hpp"

#ifndef VINECS_MAX_COMPONENT_BY_ENTITY
#define VINECS_MAX_COMPONENT_BY_ENTITY 64
#endif

#ifndef VINECS_MIN_COMPONENT_CONTAINER_SIZE
#define VINECS_MIN_COMPONENT_CONTAINER_SIZE 16
#endif

namespace Vin {
	typedef usize ArchetypeIdx;
	typedef usize ArchetypeId;

	struct ArchetypeTrait {
	public:
		template<typename... Args>
		static const ArchetypeId GetId() {
			static const ComponentId id = ++lastId;
			return id;
		}
	private:
		static ArchetypeId lastId;
	};

	ArchetypeId ArchetypeTrait::lastId{ 0 };

	enum class ArchetypeMemoryLayout {
		Contiguous,
		Interleaved
	};

	class ArchetypeComponentLayout {
	public:
		template<typename T> 
		inline void AddComponentTrait() {
			layout[count].id = ComponentTrait::GetId<T>();
			layout[count].size = ComponentTrait::GetSize<T>();
			++count;
		}
		inline void AddComponentTrait(ComponentTrait trait) {
			layout[count] = trait;
			++count;
		}

		inline ComponentTrait GetComponentTrait(usize idx) {
			return layout[idx];
		}

		inline usize GetSize() {
			return count;
		}

	private:
		ComponentTrait layout[VINECS_MAX_COMPONENT_BY_ENTITY]{ 0 };
		usize count{ 0 };
	};

	template<ArchetypeMemoryLayout memlayout>
	class ArchetypeComponentContainer {};

	template<>
	class ArchetypeComponentContainer<ArchetypeMemoryLayout::Contiguous> {
	public:
		template<typename T>
		struct Iterator {
		public:
			Iterator(T* ptr) : ptr{ ptr } {};
		public:
			inline T* operator->() {
				return ptr;
			}

			inline T* operator++() {
				++ptr;
				return ptr;
			}
			inline T* operator++(int) {
				++ptr;
				return ptr;
			}
			inline T* operator--() {
				--ptr;
				return ptr;
			}
			inline T* operator--(int) {
				--ptr;
				return ptr;
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
			for (usize i = 0; i < m_Layout.GetSize(); ++i)
				m_Data[i] = Alloc<byte>(layout.GetComponentTrait(i).size * m_Capacity);
		}

		~ArchetypeComponentContainer() {
			for (usize i = 0; i < m_Layout.GetSize(); ++i)
				Free<byte>(m_Data[i]);
			Free<byte*>(m_Data);
		}

		template<typename... Args>
		bool MatchLayout() {
			if (sizeof...(Args) != m_Layout.GetSize())
				return false;
			const ComponentId ids[sizeof...(Args)]{ ComponentTrait::GetId<Args>()... };

			for (usize i = 0; i < sizeof...(Args); ++i)
				if (ids[i] != m_Layout.GetComponentTrait(i).id)
					return false;
			return true;
		}

		template<typename... Args>
		usize AddComponents(Args... args) {
			if (sizeof...(args) != m_Layout.GetSize())
				return m_Count;

			const ComponentTrait traits[sizeof...(args)]{ComponentTrait::GetTrait<Args>()...};

			for (usize i = 0; i < sizeof...(args); ++i)
				if (traits[i].id != m_Layout.GetComponentTrait(i).id)
					return m_Count;

			if (m_Count == m_Capacity)
				ExpandCapacity();

			const void* datas[sizeof...(args)]{ &args... };

			for (usize i = 0; i < sizeof...(args); ++i)
				memcpy(m_Data[i] + (traits[i].size * m_Count), datas[i], traits[i].size);

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

		inline usize GetSize() {
			return m_Count;
		}

		inline usize GetCapacity() {
			return m_Capacity;
		}

		template<typename T>
		inline Iterator<T> Begin(usize componentIdx) {
			if (ComponentTrait::GetId<T>() != m_Layout.GetComponentTrait(componentIdx).id)
				return End<T>(componentIdx);
			return Iterator<T>{ (T*)m_Data[componentIdx] };
		}
		template<typename T>
		inline Iterator<T> End(usize componentIdx) {
			return Iterator<T>{ (T*)(&m_Data[componentIdx][sizeof(T) * m_Count]) };
		}
	private:
		void ExpandCapacity() {
			m_Capacity *= 2;
			for (usize i = 0; i < m_Layout.GetSize(); ++i)
				m_Data[i] = Realloc<byte>(m_Data[i], m_Layout.GetComponentTrait(i).size * m_Capacity);
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

	/*template<ArchetypeMemoryLayout memlayout>
	struct Archetype {
		ArchetypeComponentContainer<memlayout> components{};

		Archetype() = delete;
		Archetype(ArchetypeComponentLayout layout) : components{ layout } {};
	};*/

	//Archetype<ArchetypeMemoryLayout::Interleaved> archetype{}

}
