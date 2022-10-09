#pragma once

#define PAK_MAGIC "PACK"
#define PAK_FILE_PATH_SIZE 115

namespace Vin {

	struct PakHeader { //Size should be 8
		char magic[4]{}; //Should be PACK
		uint32_t count{ 0 }; //Number of entry in the file table
	};

	struct PakFileTableEntry { //Size should be 128
		char path[PAK_FILE_PATH_SIZE]; //Path of the file (relative) (null-terminated)
		bool compressed{ false }; //If true, file is compressed with lz4
		uint32_t uncompressedsize{ 0 }; //Uncompressed file size
		uint32_t compressedsize{ 0 }; //Should be equal to uncompressedsize if not compressed
		uint32_t offset{ 0 }; //Offset of the file (from the beginning of the pak file to the beginning of the file content)
	};

}
