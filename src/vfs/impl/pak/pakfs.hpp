#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "vfs/filesystem.hpp"

#include "pakspec.hpp"

namespace Vin {

	class PakFS : public FileSystem {
	public:
		PakFS() = delete;
		PakFS(std::string_view path);
		~PakFS();

	public:
		bool IsValid() final;
		std::unique_ptr<File> Open(std::string_view path, FileMode mode) final;
		bool Exists(std::string_view path) final;

	private:
		PakFileTableEntry* GetFileTableEntry(std::string_view path);

	private:
		std::fstream m_FileStream{};
		PakHeader m_Header{};
		std::vector<PakFileTableEntry> m_FileTable{};
	};

}

