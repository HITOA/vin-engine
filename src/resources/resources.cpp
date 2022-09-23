#include "resources.hpp"

eastl::hash_map<Vin::ResourceHandle, eastl::shared_ptr<Vin::Resource>> Vin::Resources::s_Resources{};
eastl::hash_map<eastl::string, Vin::ResourceHandle> Vin::Resources::s_ResourcesPathe{};
