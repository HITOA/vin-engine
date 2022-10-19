#include "pakmaker.hpp"
#include "logger/logger.hpp"

#include <lz4.h>

void Vin::PakMaker::AddFile(std::string_view filepath, std::string_view relativepath, bool compressed)
{
	if (!std::filesystem::exists(filepath))
		return;

	m_FileTablePath.push_back(filepath);
	PakFileTableEntry entry{};
	entry.compressed = compressed;
	memcpy(entry.path, relativepath.data(), relativepath.size());
	m_FileTable.push_back(entry);
	++m_Header.count;
}

bool Vin::PakMaker::Save(std::string_view pakpath)
{
	std::fstream file{ pakpath.data(), std::ios_base::binary | std::ios_base::trunc | std::ios_base::out};

	if (!file || m_FileTable.size() != m_FileTablePath.size())
		return false;

	memcpy(m_Header.magic, PAK_MAGIC, 4);

	file.write((char*)&m_Header, sizeof(PakHeader));

	for (auto& entry : m_FileTable)
		file.write((char*)&entry, sizeof(PakFileTableEntry));

	for (int i = 0; i < m_FileTablePath.size(); ++i) {
		uint32_t uncompressedsize = std::filesystem::file_size(m_FileTablePath[i]);
		uint32_t compressedsize = uncompressedsize;
		uint32_t offset = file.tellp();

		char* uncompressedbuff = new char[uncompressedsize];
		std::fstream currfile{ m_FileTablePath[i], std::ios_base::binary | std::ios_base::in };
		currfile.read(uncompressedbuff, uncompressedsize);
		currfile.close();

		if (!m_FileTable[i].compressed) {
			file.write(uncompressedbuff, uncompressedsize);
		}
		else {
			char* compressedbuff = new char[uncompressedsize];
			compressedsize = LZ4_compress_default(uncompressedbuff, compressedbuff, uncompressedsize, uncompressedsize);
			
			if (compressedsize == 0) {
				Logger::Err("Compression of \"{}\" in pak failed.", m_FileTablePath[i].string());
				delete[] compressedbuff;
				delete[] uncompressedbuff;
				file.close();
				return false;
			}

			file.write(compressedbuff, compressedsize);
			delete[] compressedbuff;
		}

		delete[] uncompressedbuff;

		file.seekp(sizeof(PakHeader) + sizeof(PakFileTableEntry) * i + PAK_FILE_PATH_SIZE + 1);
		file.write((char*)&uncompressedsize, sizeof(uint32_t));
		file.write((char*)&compressedsize, sizeof(uint32_t));
		file.write((char*)&offset, sizeof(uint32_t));
		file.seekp(0, std::ios::end);
	}

	file.close();
	return true;
}
