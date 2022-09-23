#pragma once

#include "vinpch.hpp"
#include <vinbase.hpp>

#define ASSET_MAGIC "\xE9\x23\xE6\x0E"

namespace Vin {
	typedef ui32 AssetId;

	struct AssetHeader {
		char magic[4];
		AssetId id;
		uint32_t depCount;
	};
}
