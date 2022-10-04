#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "vfs/impl/pak/pakspec.hpp"

namespace Vin {

	class PakMaker {
	public:
		void AddFile(std::string_view filepath, std::string_view relativepath, bool compressed = true);
		bool Save(std::string_view pakpath);
	private:
		PakHeader m_Header{ 0 };
		std::vector<PakFileTableEntry> m_FileTable{};
		std::vector<std::filesystem::path> m_FileTablePath{};
	};

}
