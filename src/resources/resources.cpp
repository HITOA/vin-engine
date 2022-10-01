#include "resources.hpp"

std::unordered_map<Vin::ResourceHandle, std::shared_ptr<Vin::Resource>> Vin::Resources::s_Resources{};
std::unordered_map<std::string, Vin::ResourceHandle> Vin::Resources::s_ResourcesPathe{};
