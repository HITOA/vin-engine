#include "asset.hpp"
#include <limits.h>

Vin::AssetTypeId Vin::AssetTypeTrait::lastId{ 0 };

Vin::AssetHandle Vin::GetNextAssetHandle()
{
	static AssetHandle lasthandle{ 0 };
	return ++lasthandle;
}

std::unordered_map<Vin::AssetHandle, Vin::AssetHolder> Vin::AssetDatabase::s_Database{};
std::unordered_map<std::string, Vin::AssetHandle> Vin::AssetDatabase::s_AssetsPath{};

/*
std::unordered_map<Vin::AssetId, Vin::AssetHolder> Vin::AssetDatabase::s_Database{};
std::unordered_map<std::string, Vin::AssetId> Vin::AssetDatabase::s_Pathes{};
Vin::AssetRegistry Vin::AssetDatabase::s_Registry{};

bool Vin::AssetDatabase::LoadRegistry(const char* path)
{
	return AssetRegistrySerializer::Load(s_Registry, path);
}

bool Vin::AssetDatabase::SaveRegistry()
{
	return AssetRegistrySerializer::Save(s_Registry);
}

Vin::AssetRegistry& Vin::AssetDatabase::GetRegistry()
{
	return s_Registry;
}*/
