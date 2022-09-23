#pragma once

#include "filesystem/gamefilesystem.hpp"

#include <physfs.h>

namespace Vin {

	class PhysfsGameFile : public GameFile {
	public:
		PhysfsGameFile(const char* path, FileMode mod);
		~PhysfsGameFile();

		void Close();
		bool IsValid();

		char ReadByte();
		size_t ReadBytes(char* buff, size_t count);
		void WriteByte(char c);
		void WriteBytes(char* buff, size_t count);

		size_t GetSize();
		bool IsEof();
		size_t GetPos();
		bool SetPos(size_t pos);
	private:
		PHYSFS_File* m_Handle;
	};

	class PhysfsGameFilesystemApi : public GameFilesystem::GameFilesystemApi {
	public:
		PhysfsGameFilesystemApi();
		~PhysfsGameFilesystemApi();

		void Mount(const char* path) final;
		bool Exists(const char* filepath) final;
		eastl::unique_ptr<GameFile> Open(const char* filepath, FileMode mod) final;
		eastl::unique_ptr<GameFile> Create(const char* filepath) final;
		void Delete(const char* filepath) final;
	};

}
