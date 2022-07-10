/*****************************************************************//**
 * \file   ecs.hpp
 * \brief  Entity component system, made for another project some time ago.
 * 
 * \author HITO & Resnox
 * \date   July 2022
 *********************************************************************/

#pragma once

#include <cstdint>
#include <type_traits>
#include <memory>
#include <functional>
#include <cstddef>

namespace Vin {
	namespace Component::Trait {
		using IdType = uint16_t;

		namespace {
			static IdType lastId = 0u;
		}

		/**
		 * \brief Give runtime information about a component.
		 * \tparam T Component Type
		*/
		template<class T>
		struct ComponentTrait {
			static_assert(!std::is_void_v<T>, "component trait type can't be void");
		public:
			static constexpr IdType GetId() noexcept {
				static IdType id = ++lastId;
				return id;
			}
			static constexpr size_t GetSize() noexcept {
				static size_t size = sizeof(T);
				return size;
			}
		};
	}
	namespace Component {
#define MIN_COMPONENTARRAY_CAPACITY 16

		class ComponentArray {
		public:
			ComponentArray() = delete;
			ComponentArray(std::size_t stride);
			ComponentArray(std::size_t stride, std::size_t capacity);
			~ComponentArray();
		public:
			/**
			 * \brief Add a component at the end of the component array
			 * \param componentdata ptr to the component, to be copied in the component array
			*/
			void Add(void* componentdata);
			/**
			 * \brief Remove a component from the component array
			 * \param idx index of the component to be removed
			*/
			void Remove(std::size_t idx);
			/**
			 * \brief Get the number of component in the component array
			 * \return the number of component
			*/
			std::size_t Count();
			/**
			 * \brief Get component at index idx
			 * \param idx : index
			 * \return ptr to the component
			*/
			void* GetComponent(std::size_t idx);
		private:
			void CheckForRealloc();
		private:
			void* data;
			std::size_t stride;
			std::size_t capacity;
			std::size_t count;
		};

		struct ComponentArrayData {
			std::shared_ptr<ComponentArray> componentArray;
			Trait::IdType componentId;
		};

		/**
			* \brief Find ComponentArray from component id
			* \param componentId
			* \return SharedPtr to ComponentArray
		*/
		std::shared_ptr<ComponentArray> FindComponentArray(Trait::IdType componentId);
		/**
			* \brief Create ComponentArray
			* \param componentId
			* \param componentSize
		*/
		void CreateComponentArray(Trait::IdType componentId, std::size_t componentSize);
		/**
			* \brief Try to find the ComponentArray first, return if find, otherwise create a new one.
			* \param componentId
			* \param componentSize
			* \return SharedPtr to ComponentArray
		*/
		std::shared_ptr<ComponentArray> FindOrCreateComponentArray(Trait::IdType componentId, std::size_t componentSize);


		/**
		 * \brief Find ComponentArray of type T
		 * \tparam T Component Type
		 * \return ComponentArray of Component T
		*/
		template<class T>
		std::shared_ptr<ComponentArray> FindComponentArray() {
			return FindComponentArray(Trait::ComponentTrait<T>::GetId());
		}

		/**
		 * \brief Create ComponentArray of type T
		 * \tparam T Component Type
		*/
		template<class T>
		void CreateComponentArray() {
			CreateComponentArray(Trait::ComponentTrait<T>::GetId(), Trait::ComponentTrait<T>::GetSize());
		}

		/**
		 * \brief Try to find the ComponentArray first, return if find, otherwise create a new one.
		 * \tparam T Component Type
		 * \return ComponentArray of Component T
		*/
		template<class T>
		std::shared_ptr<ComponentArray> FindOrCreateComponentArray() {
			return FindOrCreateComponentArray(Trait::ComponentTrait<T>::GetId(), Trait::ComponentTrait<T>::GetSize());
		}
	}

	using EntityId = uint32_t;

	class Entity {
	public:
		Entity(EntityId id);
	public:
		bool HasComponent(Component::Trait::IdType id);
		void AssignComponent(Component::Trait::IdType id, std::size_t idx);
		std::size_t GetComponentIdx(Component::Trait::IdType id);
		void OffsetComponentIfHigher(Component::Trait::IdType id, std::size_t offset, std::size_t v);
		void RemoveComponent(Component::Trait::IdType id);
		std::unordered_map<Component::Trait::IdType, std::size_t>& GetComponents();
		EntityId GetId();
	private:
		/**
		 * \brief hold component type id with index in the componentarray
		*/
		std::unordered_map<Component::Trait::IdType, std::size_t> components;
		EntityId id;
	};

	namespace EntityManager {
		/**
			* \brief Create a new entity and return his entity id
			* \return corresponding entity id
		*/
		EntityId CreateEntity();
		/**
			* \brief Try to get the entity linked to id
			* \param id entity id
			* \param entityout if entity is found, it will be put in entityout
			* \return true if entity was found, false otherwise
		*/
		bool TryGetEntity(EntityId id, Entity*& entityout);
		/**
			* \brief Add component to entity
			* \param entityId
			* \param componentId
			* \param componentData
			* \param componentSize
			* \return true if succeed, false otherwise
		*/
		bool AddComponent(EntityId entityId, Component::Trait::IdType componentId, void* componentData, size_t componentSize);
		/**
			* \brief Remove component from entity
			* \param entityId
			* \param componentId
			* \return true if the component is no longer existing, false otherwise
		*/
		bool RemoveComponent(EntityId entityId, Component::Trait::IdType componentId);
		/**
			* \brief Destroy an entity
			* \param entity id
		*/
		bool DestroyEntity(EntityId id);

		/**
			* @brief Get Vector of entities
			* @return vector of entities
		*/
		std::vector<Entity>& GetEntities();

		/**
		 * \brief Add Component of type T
		 * \tparam T : Component Type
		 * \param id : Entity Id
		 * \param component : Component to be added
		 * \return true if succeed, false otherwise
		 * \code {.cpp}
		 *	EntityId entity = EntityManager::CreateEntity();
		 *	Transform transform{}; // A component
		 *
		 *	EntityManager::AddComponent(entity, transform);
		 * \endcode
		*/
		template<class T>
		bool AddComponent(EntityId id, T& component) {
			return AddComponent(id,
				Component::Trait::ComponentTrait<T>::GetId(),
				&component,
				Component::Trait::ComponentTrait<T>::GetSize());
		}

		/**
		 * \brief Add Components of type Ts
		 * \param id : Entity Id
		 * \param component : Component to be added
		 * \return true if succeed, false otherwise
		*/
		template<class T, class... Args>
		bool AddComponent(EntityId id, T& component, Args... args) {
			bool c = AddComponent(id,
				Component::Trait::ComponentTrait<T>::GetId(),
				&component,
				Component::Trait::ComponentTrait<T>::GetSize());
			c &= AddComponent(id, args...);
			return c;
		}

		/**
		 * \brief Remove Component of type T
		 * \tparam T : Component Type
		 * \param id : Entity id
		 * \return true if the component is no longer existing, false otherwise
		 * \code {.cpp}
		 *	RemoveComponent<Transform>(entity);
		 * \endcode
		*/
		template<class T>
		bool RemoveComponent(EntityId id) {
			return RemoveComponent(id, Component::Trait::ComponentTrait<T>::GetId());
		}

		/**
		 * @brief Remove Components of type Ts
		 * @param id : EntityId
		 * @return true if the component is no longer existing, false otherwise
		*/
		template<class T, class U, class... Args>
		bool RemoveComponent(EntityId id) {
			bool c = RemoveComponent<T>(id);
			c &= RemoveComponent<U, Args...>(id);
			return c;
		}

		/**
		 * \brief Main ForEach function, Will find any matching entity and call system for every entity & component together
		 * \tparam RetType ReturnType of the system
		 * \tparam ...Args ComponentType of the system (variadic)
		 * \param system 
		*/
		template<class RetType, class... Args, std::size_t... S>
		void ForEach(RetType(system)(EntityId, Args&...), std::index_sequence<S...>) {
			std::vector<std::shared_ptr<Component::ComponentArray>> componentArrays{ Component::FindComponentArray<Args>()... };
			std::vector<Component::Trait::IdType> componentIds{ Component::Trait::ComponentTrait<Args>::GetId()... };
			for (auto& entity : GetEntities()) {
				bool c = true;
				for (auto id : componentIds)
					c &= entity.HasComponent(id);
				if (c) {
					system(entity.GetId(),
						(*((Args*)componentArrays[S]->GetComponent(entity.GetComponentIdx(Component::Trait::ComponentTrait<Args>::GetId()))))...);
				}
			}
		}

		/**
		 * \brief Helper/Warper function for main ForEach
		*/
		template<class RetType, class DummyA, class DummyB, class... Args>
		void ForEach(RetType(system)(EntityId, DummyA, DummyB, Args&...)) {
			ForEach(system, std::make_index_sequence<sizeof...(Args) + 2>{});
		}

		/**
		 * \brief Main ForEach function for one component requirment system
		*/
		template<class RetType, class Arg>
		void ForEach(RetType(system)(EntityId, Arg&)) {
			std::shared_ptr<Component::ComponentArray> componentArray = Component::FindComponentArray<Arg>();
			for (auto& entity : GetEntities())
				if (entity.HasComponent(Component::Trait::ComponentTrait<Arg>::GetId()))
					system(entity.GetId(),
						*((Arg*)componentArray->GetComponent(entity.GetComponentIdx(Component::Trait::ComponentTrait<Arg>::GetId()))));
		}

		/**
		 * \brief Main ForEach function, Will find any matching entity and call system for every entity & component together
		 * \tparam RetType ReturnType of the system
		 * \tparam ...Args ComponentType of the system (variadic)
		 * \param system
		*/
		template<class RetType, class... Args, std::size_t... S>
		void ForEach(std::function<RetType(EntityId, Args&...)> system, std::index_sequence<S...>) {
			std::vector<std::shared_ptr<Component::ComponentArray>> componentArrays{ Component::FindComponentArray<Args>()... };
			std::vector<Component::Trait::IdType> componentIds{ Component::Trait::ComponentTrait<Args>::GetId()... };
			for (auto& entity : GetEntities()) {
				bool c = true;
				for (auto id : componentIds)
					c &= entity.HasComponent(id);
				if (c) {
					system(entity.GetId(),
						(*((Args*)componentArrays[S]->GetComponent(entity.GetComponentIdx(Component::Trait::ComponentTrait<Args>::GetId()))))...);
				}
			}
		}

		/**
		 * \brief Helper/Warper function for main ForEach
		*/
		template<class RetType, class DummyA, class DummyB, class... Args>
		void ForEach(std::function<RetType(EntityId, DummyA, DummyB, Args&...)> system) {
			ForEach(system, std::make_index_sequence<sizeof...(Args) + 2>{});
		}

		/**
		 * \brief Main ForEach function for one component requirment system
		*/
		template<class RetType, class Arg>
		void ForEach(std::function<RetType(EntityId, Arg&)> system) {
			std::shared_ptr<Component::ComponentArray> componentArray = Component::FindComponentArray<Arg>();
			for (auto& entity : GetEntities())
				if (entity.HasComponent(Component::Trait::ComponentTrait<Arg>::GetId()))
					system(entity.GetId(),
						*((Arg*)componentArray->GetComponent(entity.GetComponentIdx(Component::Trait::ComponentTrait<Arg>::GetId()))));
		}
	}
}