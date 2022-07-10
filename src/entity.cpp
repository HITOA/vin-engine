#include <ecs.hpp>
#include <algorithm>
#include <iterator>

namespace Vin {
#pragma region Entity
    Entity::Entity(EntityId id) : id{ id }, components{} {}

    bool Entity::HasComponent(Component::Trait::IdType id)
    {
        return components.contains(id);
    }

    void Entity::AssignComponent(Component::Trait::IdType id, std::size_t idx)
    {
        components.emplace(id, idx);
    }
    std::size_t Entity::GetComponentIdx(Component::Trait::IdType id)
    {
        return components[id];
    }
    void Entity::OffsetComponentIfHigher(Component::Trait::IdType id, std::size_t offset, std::size_t v)
    {
        if (!components.contains(id))
            return;
        if (components[id] > v)
            components.at(id) += offset;
    }
    void Entity::RemoveComponent(Component::Trait::IdType id)
    {
        components.erase(id);
    }
    std::unordered_map<Component::Trait::IdType, std::size_t>& Entity::GetComponents()
    {
        return components;
    }
    EntityId Entity::GetId()
    {
        return id;
    }
#pragma endregion

    namespace EntityManager {
        namespace {
            EntityId _lastId;
            std::unordered_map<EntityId, std::size_t> _entitiesId2Idx{};
            std::vector<Entity> _entities{};
        }

        EntityId CreateEntity()
        {
            EntityId entityId = ++_lastId;
            std::size_t entityIdx = _entities.size();

            _entitiesId2Idx.emplace(entityId, entityIdx);
            _entities.push_back(Entity{ entityId });

            return entityId;
        }

        bool TryGetEntity(EntityId id, Entity*& entityout)
        {
            if (!_entitiesId2Idx.contains(id))
                return false;
            entityout = &_entities.at(_entitiesId2Idx[id]);
            return true;
        }

        bool AddComponent(EntityId entityId, Component::Trait::IdType componentId, void* componentData, std::size_t componentSize)
        {
            Entity* entity;

            if (!TryGetEntity(entityId, entity)) {
                //Square::Error::Warn(entity_not_found{ std::format("There is no entity link to entityid {}", entityId) });
                return false;
            }

            if (entity->HasComponent(componentId)) {
                //Square::Error::Warn(cant_duplicate_component{ std::format("Entity {} already have component {}", entityId, componentId) });
                return false;
            }

            std::shared_ptr<Component::ComponentArray> componentArray = Component::FindOrCreateComponentArray(componentId, componentSize);
            
            size_t componentIdx = componentArray->Count();
            componentArray->Add(componentData);

            entity->AssignComponent(componentId, componentIdx);
            return true;
        }

        bool RemoveComponent(EntityId entityId, Component::Trait::IdType componentId)
        {
            Entity* entity;

            if (!TryGetEntity(entityId, entity)) {
                //Square::Error::Warn(entity_not_found{ std::format("There is no entity link to entityid {}", entityId) });
                return false;
            }

            if (!entity->HasComponent(componentId))
                return true;

            std::shared_ptr<Component::ComponentArray> componentArray = Component::FindComponentArray(componentId);

            //if (componentArray == nullptr)
                //Square::Error::Throw(component_mismatch{ "ComponentArray missing, do not match with entity." }); //Bad

            std::size_t componentIdx = entity->GetComponentIdx(componentId);

            componentArray->Remove(componentIdx);
            entity->RemoveComponent(componentId);

            for (auto& it : _entities)
                it.OffsetComponentIfHigher(componentId, -1, componentIdx);

            return true;
        }

        bool DestroyEntity(EntityId id)
        {
            Entity* entity;

            if (!TryGetEntity(id, entity)) {
                //Square::Error::Warn(entity_not_found{ std::format("Attempt to destroy a non existent entity, entity id : {}", id) });
                return true;
            }

            std::vector<Component::Trait::IdType> componentIds{};
            std::transform(
                entity->GetComponents().begin(), 
                entity->GetComponents().end(), 
                std::back_inserter(componentIds), 
                []<class T>(T v) { return v.first; });

            for (const auto& componentId : componentIds)
                RemoveComponent(id, componentId);

            size_t entityIdx = _entitiesId2Idx[id];

            _entitiesId2Idx.erase(id);
            
            for (auto& it : _entitiesId2Idx)
                if (it.second > entityIdx)
                    it.second--;
            
            _entities.erase(_entities.begin() + entityIdx);
        }

        std::vector<Entity>& GetEntities() {
            return _entities;
        }
    }
}
