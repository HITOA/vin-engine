#include "pakfs.hpp"
#include "pakfile.hpp"

#include "logger/logger.hpp"

#include <lz4.h>

Vin::PakFS::PakFS(std::string_view path)
{
	m_FileStream = std::fstream{ path.data(), std::ios_base::binary | std::ios_base::in };
	if (m_FileStream.is_open()) {
		m_FileStream.read((char*)&m_Header, sizeof(PakHeader));
		for (uint32_t i = 0; i < m_Header.count; ++i) {
			PakFileTableEntry entry{};
			m_FileStream.read((char*)&entry, sizeof(PakFileTableEntry));
			m_FileTable.push_back(entry);
		}
	}
}

Vin::PakFS::~PakFS()
{
	m_FileStream.close();
}

bool Vin::PakFS::IsValid()
{
	return m_FileStream.is_open() && (memcmp(m_Header.magic, PAK_MAGIC, 4) == 0);
}

std::unique_ptr<Vin::File> Vin::PakFS::Open(std::string_view path, FileMode mode)
{
	if (mode != FileMode::Read)
		Logger::Log("FileMode set to readonly for \"{}\"", path);

	PakFileTableEntry* entry = GetFileTableEntry(path);
	if (entry == nullptr)
		return nullptr;

	char* compressedbuff = new char[entry->compressedsize];
	m_FileStream.seekg(entry->offset);
	m_FileStream.read(compressedbuff, entry->compressedsize);

	if (!entry->compressed)
		return std::unique_ptr<PakFile>{new PakFile{ compressedbuff, entry->compressedsize }};

	char* uncompressedbuff = new char[entry->uncompressedsize];
	int r = LZ4_decompress_safe(compressedbuff, uncompressedbuff, entry->compressedsize, entry->uncompressedsize);

	if (r < 0) {
		Logger::Err("Error while decompressing \"{}\" : LZ4 Error {}", path, r);
		delete[] compressedbuff;
		delete[] uncompressedbuff;
		return nullptr;
	}

	delete[] compressedbuff;
	return std::unique_ptr<PakFile>{new PakFile{ uncompressedbuff, (size_t)r }};
}

bool Vin::PakFS::Exists(std::string_view path)
{
	return GetFileTableEntry(path) != nullptr;
}

Vin::PakFileTableEntry* Vin::PakFS::GetFileTableEntry(std::string_view path)
{
	if (path.size() > PAK_FILE_PATH_SIZE)
		return nullptr;

	std::filesystem::path filepath{ path };
	filepath.make_preferred();

	for (PakFileTableEntry& entry : m_FileTable) {
		std::filesystem::path entrypath{ entry.path };
		entrypath.make_preferred();

		if (filepath == entrypath)
			return &entry;
	}

	return nullptr;
}
