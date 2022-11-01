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

		template<typename T1>
		struct Group<EntityId, T1> {
			EntityId id;
			T1* a;
		};

		template<typename T1, typename T2>
		struct Group<T1, T2> {
			T1* a;
			T2* b;
		};

		template<typename T1, typename T2>
		struct Group<EntityId, T1, T2> {
			EntityId id;
			T1* a;
			T2* b;
		};

		template<typename T1, typename T2, typename T3>
		struct Group<T1, T2, T3> {
			T1* a;
			T2* b;
			T3* c;
		};

		template<typename T1, typename T2, typename T3>
		struct Group<EntityId, T1, T2, T3> {
			EntityId id;
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

		template<typename T1, typename T2, typename T3, typename T4>
		struct Group<EntityId, T1, T2, T3, T4> {
			EntityId id;
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

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		struct Group<EntityId, T1, T2, T3, T4, T5> {
			EntityId id;
			T1* a;
			T2* b;
			T3* c;
			T4* d;
			T5* e;
		};

		template<typename... Args>
		struct IteratorGroup {};

		template<typename T1>
		struct IteratorGroup<T1> {
			typename ArchetypeComponentContainer::template Iterator<T1> a;

			inline Group<T1> operator*() {
				return Group<T1>{ a.Get() };
			}

			inline IteratorGroup<T1> operator++() {
				++a;
				return *this;
			}
		};

		template<typename T1>
		struct IteratorGroup<EntityId, T1> {
			EntityId* id;
			typename ArchetypeComponentContainer::template Iterator<T1> a;

			inline Group<EntityId, T1> operator*() {
				return Group<EntityId, T1>{ *id, a.Get() };
			}

			inline IteratorGroup<EntityId, T1> operator++() {
				++id;
				++a;
				return *this;
			}
		};

		template<typename T1, typename T2>
		struct IteratorGroup<T1, T2> {
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;

			inline Group<T1, T2> operator*() {
				return Group<T1, T2>{ a.Get(), b.Get() };
			}

			inline IteratorGroup<T1, T2> operator++() {
				++a;
				++b;
				return *this;
			}
		};

		template<typename T1, typename T2>
		struct IteratorGroup<EntityId, T1, T2> {
			EntityId* id;
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;

			inline Group<EntityId, T1, T2> operator*() {
				return Group<EntityId, T1, T2>{ *id, a.Get(), b.Get() };
			}

			inline IteratorGroup<EntityId, T1, T2> operator++() {
				++id;
				++a;
				++b;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3>
		struct IteratorGroup<T1, T2, T3> {
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;

			inline Group<T1, T2, T3> operator*() {
				return Group<T1, T2, T3>{ a.Get(), b.Get(), c.Get() };
			}

			inline IteratorGroup<T1, T2, T3> operator++() {
				++a;
				++b;
				++c;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3>
		struct IteratorGroup<EntityId, T1, T2, T3> {
			EntityId* id;
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;

			inline Group<EntityId, T1, T2, T3> operator*() {
				return Group<EntityId, T1, T2, T3>{ *id, a.Get(), b.Get(), c.Get() };
			}

			inline IteratorGroup<EntityId, T1, T2, T3> operator++() {
				++id;
				++a;
				++b;
				++c;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3, typename T4>
		struct IteratorGroup<T1, T2, T3, T4> {
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;
			typename ArchetypeComponentContainer::template Iterator<T4> d;

			inline Group<T1, T2, T3, T4> operator*() {
				return Group<T1, T2, T3, T4>{ a.Get(), b.Get(), c.Get(), d.Get() };
			}

			inline IteratorGroup<T1, T2, T3, T4> operator++() {
				++a;
				++b;
				++c;
				++d;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3, typename T4>
		struct IteratorGroup<EntityId, T1, T2, T3, T4> {
			EntityId* id;
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;
			typename ArchetypeComponentContainer::template Iterator<T4> d;

			inline Group<EntityId, T1, T2, T3, T4> operator*() {
				return Group<EntityId, T1, T2, T3, T4>{ *id, a.Get(), b.Get(), c.Get(), d.Get() };
			}

			inline IteratorGroup<EntityId, T1, T2, T3, T4> operator++() {
				++id;
				++a;
				++b;
				++c;
				++d;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		struct IteratorGroup<T1, T2, T3, T4, T5> {
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;
			typename ArchetypeComponentContainer::template Iterator<T4> d;
			typename ArchetypeComponentContainer::template Iterator<T5> e;

			inline Group<T1, T2, T3, T4, T5> operator*() {
				return Group<T1, T2, T3, T4, T5>{ a.Get(), b.Get(), c.Get(), d.Get(), e.Get() };
			}

			inline IteratorGroup<T1, T2, T3, T4, T5> operator++() {
				++a;
				++b;
				++c;
				++d;
				++e;
				return *this;
			}
		};

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		struct IteratorGroup<EntityId, T1, T2, T3, T4, T5> {
			EntityId* id;
			typename ArchetypeComponentContainer::template Iterator<T1> a;
			typename ArchetypeComponentContainer::template Iterator<T2> b;
			typename ArchetypeComponentContainer::template Iterator<T3> c;
			typename ArchetypeComponentContainer::template Iterator<T4> d;
			typename ArchetypeComponentContainer::template Iterator<T5> e;

			inline Group<EntityId, T1, T2, T3, T4, T5> operator*() {
				return Group<EntityId, T1, T2, T3, T4, T5>{ *id, a.Get(), b.Get(), c.Get(), d.Get(), e.Get() };
			}

			inline IteratorGroup<EntityId, T1, T2, T3, T4, T5> operator++() {
				++id;
				++a;
				++b;
				++c;
				++d;
				++e;
				return *this;
			}
		};
	}

	template<typename... Args>
	class Query {
	public:
		struct Iterator {
		public:
			Iterator(QueryGroup::IteratorGroup<Args...> grp, usize count) : grp{ grp }, count{ count } {};
		public:
			inline QueryGroup::Group<Args...> operator*() {
				return *grp;
			}

			inline QueryGroup::IteratorGroup<Args...> operator++() {
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
			QueryGroup::IteratorGroup<Args...> grp;
			usize count;
		};
	public:
		Query(typename ArchetypeComponentContainer::template Iterator<Args>... args, usize count) : grp{ args... }, count{ count } {};
		Query(QueryGroup::IteratorGroup<Args...> grp, usize count) : grp{ grp }, count{ count } {};

		inline usize Count() {
			return count;
		}

		Iterator begin() {
			return Iterator{ grp, count };
		}

		Iterator end() {
			return Iterator{ grp, 0 };
		}
	private:
		QueryGroup::IteratorGroup<Args...> grp;
		usize count;
	};

	template<typename... Args>
	class Query<EntityId, Args...> {
	public:
		struct Iterator {
		public:
			Iterator(QueryGroup::IteratorGroup<EntityId, Args...> grp, usize count) : grp{ grp }, count{ count } {};
		public:
			inline QueryGroup::Group<EntityId, Args...> operator*() {
				return *grp;
			}

			inline QueryGroup::IteratorGroup<EntityId, Args...> operator++() {
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
			QueryGroup::IteratorGroup<EntityId, Args...> grp;
			usize count;
		};
	public:
		Query(EntityId* id, typename ArchetypeComponentContainer::template Iterator<Args>... args, usize count) : grp{ id, args... }, count{ count } {};
		Query(QueryGroup::IteratorGroup<EntityId, Args...> grp, usize count) : grp{ grp }, count{ count } {};

		inline usize Count() {
			return count;
		}

		Iterator begin() {
			return Iterator{ grp, count };
		}

		Iterator end() {
			return Iterator{ grp, 0 };
		}
	private:
		QueryGroup::IteratorGroup<EntityId, Args...> grp;
		usize count;
	};

}
