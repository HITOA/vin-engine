#include "asset.hpp"
#include <limits.h>

Vin::AssetTypeId Vin::AssetTypeTrait::lastId{ 0 };

Vin::AssetHandle Vin::GetNextAssetHandle()
{
	static AssetHandle lasthandle{ 0 };
	return ++lasthandle;
}
