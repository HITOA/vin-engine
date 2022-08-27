#pragma once

#include "archetype.hpp"
#include "entity.hpp"

namespace Vin {

	namespace QueryGroup {
		template<typename... Args>
		struct Group {};

		template<typename T1>
		struct Group<T1> {
			T1* a;
		};

		template<typename T1, typename T2>
		struct Group<T1, T2> {
			T1* a;
			T2* b;
		};

		template<typename T1, typename T2, typename T3>
		struct Group<T1, T2, T3> {
			T1* a;
			T2* b;
			T3* c;
		};

		template<typename T1, typename T2, typename T3, typename T4>
		struct Group<T1, T2, T3, T4> {
			T1* a;
			T2* b;
			T3* c;
			T4* d;
		};

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		struct Group<T1, T2, T3, T4, T5> {
			T1* a;
			T2* b;
			T3* c;
			T4* d;
			T5* e;
		};

		template<ArchetypeMemoryLayout memlayout, typename... Args>
		struct IteratorGroup {};

		template<ArchetypeMemoryLayout memlayout, typename T1>
		struct IteratorGroup<memlayout, T1> {
			ArchetypeComponentContainer<memlayout>::Iterator<T1> a;

			inline Group<T1> operator*() {
				return Group<T1>{ a.Get() };
			}

			inline IteratorGroup<memlayout, T1> operator++() {
				++a;
				return *this;
			}
		};

		template<ArchetypeMemoryLayout memlayout, typename T1, typename T2>
		struct IteratorGroup<memlayout, T1, T2> {
			ArchetypeComponentContainer<memlayout>::Iterator<T1> a;
			ArchetypeComponentContainer<memlayout>::Iterator<T2> b;

			inline Group<T1, T2> operator*() {
				return Group<T1, T2>{ a.Get(), b.Get() };
			}

			inline IteratorGroup<memlayout, T1, T2> operator++() {
				++a;
				++b;
				return *this;
			}
		};

		template<ArchetypeMemoryLayout memlayout, typename T1, typename T2, typename T3>
		struct IteratorGroup<memlayout, T1, T2, T3> {
			ArchetypeComponentContainer<memlayout>::Iterator<T1> a;
			ArchetypeComponentContainer<memlayout>::Iterator<T2> b;
			ArchetypeComponentContainer<memlayout>::Iterator<T3> c;

			inline Group<T1, T2, T3> operator*() {
				return Group<T1, T2, T3>{ a.Get(), b.Get(), c.Get() };
			}

			inline IteratorGroup<memlayout, T1, T2, T3> operator++() {
				++a;
				++b;
				++c;
				return *this;
			}
		};

		template<ArchetypeMemoryLayout memlayout, typename T1, typename T2, typename T3, typename T4>
		struct IteratorGroup<memlayout, T1, T2, T3, T4> {
			ArchetypeComponentContainer<memlayout>::Iterator<T1> a;
			ArchetypeComponentContainer<memlayout>::Iterator<T2> b;
			ArchetypeComponentContainer<memlayout>::Iterator<T3> c;
			ArchetypeComponentContainer<memlayout>::Iterator<T4> d;

			inline Group<T1, T2, T3, T4> operator*() {
				return Group<T1, T2, T3, T4>{ a.Get(), b.Get(), c.Get(), d.Get() };
			}

			inline IteratorGroup<memlayout, T1, T2, T3, T4> operator++() {
				++a;
				++b;
				++c;
				++d;
				return *this;
			}
		};

		template<ArchetypeMemoryLayout memlayout, typename T1, typename T2, typename T3, typename T4, typename T5>
		struct IteratorGroup<memlayout, T1, T2, T3, T4, T5> {
			ArchetypeComponentContainer<memlayout>::Iterator<T1> a;
			ArchetypeComponentContainer<memlayout>::Iterator<T2> b;
			ArchetypeComponentContainer<memlayout>::Iterator<T3> c;
			ArchetypeComponentContainer<memlayout>::Iterator<T4> d;
			ArchetypeComponentContainer<memlayout>::Iterator<T5> e;

			inline Group<T1, T2, T3, T4, T5> operator*() {
				return Group<T1, T2, T3, T4, T5>{ a.Get(), b.Get(), c.Get(), d.Get(); e.Get() };
			}

			inline IteratorGroup<memlayout, T1, T2, T3, T4, T5> operator++() {
				++a;
				++b;
				++c;
				++d;
				++e;
				return *this;
			}
		};
	}

	template<ArchetypeMemoryLayout memlayout, typename... Args>
	class Query {
	public:
		struct Iterator {
		public:
			Iterator(QueryGroup::IteratorGroup<memlayout, Args...> grp, usize count) : grp{ grp }, count{ count } {};
		public:
			inline QueryGroup::Group<Args...> operator*() {
				return *grp;
			}

			inline QueryGroup::IteratorGroup<memlayout, Args...> operator++() {
				--count;
				return ++grp;
			}

			inline friend bool operator==(const Iterator& it1, const Iterator& it2) {
				return it1.count == it2.count;
			}
			inline friend bool operator!=(const Iterator& it1, const Iterator& it2) {
				return it1.count != it2.count;
			}
		private:
			QueryGroup::IteratorGroup<memlayout, Args...> grp;
			usize count;
		};
	public:
		Query(ArchetypeComponentContainer<memlayout>::Iterator<Args>... args, usize count) : grp{ args... }, count{ count } {};
		Query(QueryGroup::IteratorGroup<memlayout, Args...> grp, usize count) : grp{ grp }, count{ count } {};

		Iterator begin() {
			return Iterator{ grp, count };
		}

		Iterator end() {
			return Iterator{ grp, 0 };
		}
	private:
		QueryGroup::IteratorGroup<memlayout, Args...> grp;
		usize count;
	};

}
