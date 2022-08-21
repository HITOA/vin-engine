#include "entity.hpp"

Vin::EntityId Vin::EntityManager::GetNextId()
{
	return ++lastId;
}
