#include "assetdatabase.hpp"
#include "EASTL/sort.h"

eastl::hash_map<Vin::AssetId, Vin::AssetHolder> Vin::AssetDatabase::s_Database{};
eastl::hash_map<eastl::string, Vin::AssetId> Vin::AssetDatabase::s_Pathes{};
eastl::fixed_vector<Vin::AssetRegistry, 8> Vin::AssetDatabase::s_Registries{};

void Vin::AssetDatabase::AddRegistry(AssetRegistry&& registry)
{
	s_Registries.push_back(registry);
	eastl::quick_sort(s_Registries.begin(), s_Registries.end(), [](const AssetRegistry& a, const AssetRegistry& b) {
		return a.GetOffset() > b.GetOffset();
		});
}

void Vin::AssetDatabase::AddRegistry(const char* path)
{
	AssetRegistry registry{};
	if (AssetRegistrySerDes::Load(registry, path))
		s_Registries.push_back(registry);
	eastl::quick_sort(s_Registries.begin(), s_Registries.end(), [](const AssetRegistry& a, const AssetRegistry& b) {
		return a.GetOffset() > b.GetOffset();
		});
}
