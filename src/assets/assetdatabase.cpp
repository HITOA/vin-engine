#include "assetdatabase.hpp"

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
