#pragma once

#include "core/base.hpp">
#include "core/allocator.hpp"

#include "vinpch.hpp"

namespace Vin {
	typedef usize EventId;

	struct EventTrait {
	public:
		EventId id;
		usize size;

	public:
		template<typename T>
		static const EventId GetId() {
			static const EventId id = ++lastId;
			return id;
		}
		template<typename T>
		static inline usize GetSize() {
			return sizeof(T);
		}
		template<typename T>
		static inline EventTrait GetTrait() {
			EventTrait trait{};
			trait.id = GetId<T>();
			trait.size = GetSize<T>();
			return trait;
		}
	private:
		static EventId lastId;
	};

	class EventHandler { //Manage the event life time
	public:
		template<typename T>
		inline void Bind(T event) { //Alloc and make copy of the event
			m_Trait = EventTrait::GetTrait<T>();
			m_Datas = Alloc<T>(1);
			memcpy(m_Datas, &event, m_Trait.size);
		}
		inline void Unbind() { //Release the event
			if (!m_Datas)
				return;
			Free<void>(m_Datas);
		}
		template<typename T>
		inline T* GetEvent() { //Return the event, nullptr if wrong type of event.
			if (m_Trait.id != EventTrait::GetId<T>())
				return nullptr;
			return (T*)m_Datas;
		}
	private:
		void* m_Datas{ nullptr }; //Should do something to avoid many Allocation/Free every frame
		EventTrait m_Trait{};
	};
}
