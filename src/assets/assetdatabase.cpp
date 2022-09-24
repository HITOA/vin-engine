#include "assetdatabase.hpp"

void Vin::AssetDatabase::AddRegistry(AssetRegistry&& registry)
{
	m_Registries.push_back(registry);
}

void Vin::AssetDatabase::AddRegistry(const char* path)
{
	AssetRegistry registry{};
	if (AssetRegistrySerializer::Load(registry, path))
		m_Registries.push_back(registry);
}
