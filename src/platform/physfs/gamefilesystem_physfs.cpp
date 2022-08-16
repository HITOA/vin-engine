#include "gamefilesystem_physfs.hpp"

#include "core/assert.hpp"

#include <physfs.h>

Vin::PhysfsGameFilesystemApi::PhysfsGameFilesystemApi() {
	PHYSFS_init(NULL);
}

Vin::PhysfsGameFilesystemApi::~PhysfsGameFilesystemApi()
{
	PHYSFS_deinit();
}

void Vin::PhysfsGameFilesystemApi::Mount(const char* path)
{
	PHYSFS_mount(path, nullptr, 0);
}

bool Vin::PhysfsGameFilesystemApi::Exists(const char* filepath)
{
	return PHYSFS_exists(filepath);
}

eastl::unique_ptr<Vin::GameFile> Vin::PhysfsGameFilesystemApi::Open(const char* filepath, FileMode mod)
{
	if (!Exists(filepath))
		return nullptr;
	return eastl::make_unique<PhysfsGameFile>(filepath, mod);
}

void Vin::PhysfsGameFilesystemApi::Delete(const char* filepath)
{
	PHYSFS_delete(filepath);
}

Vin::PhysfsGameFile::PhysfsGameFile(const char* path, FileMode mod) {
	switch (mod) {
	case FileMode::Read: {
		m_Handle = PHYSFS_openRead(path);
		return;
	}
	case FileMode::Write: {
		m_Handle = PHYSFS_openWrite(path);
		return;
	}
	case FileMode::WriteAppend: {
		m_Handle = PHYSFS_openAppend(path);
		return;
	}
	}

	VIN_ASSERT(false, "Bad filemode.");
	return;
}

Vin::PhysfsGameFile::~PhysfsGameFile()
{
	Close();
}

void Vin::PhysfsGameFile::Close()
{
	PHYSFS_close(m_Handle);
}

char Vin::PhysfsGameFile::ReadByte()
{
	char c;
	PHYSFS_readBytes(m_Handle, &c, 1);
	return c;
}

size_t Vin::PhysfsGameFile::ReadBytes(char* buff, size_t count)
{
	return PHYSFS_readBytes(m_Handle, buff, count);
}

void Vin::PhysfsGameFile::WriteByte(char c)
{
	PHYSFS_writeBytes(m_Handle, &c, 1);
}

void Vin::PhysfsGameFile::WriteBytes(char* buff, size_t count)
{
	PHYSFS_writeBytes(m_Handle, buff, count);
}

size_t Vin::PhysfsGameFile::GetSize()
{
	return PHYSFS_fileLength(m_Handle);
}

bool Vin::PhysfsGameFile::IsEof()
{
	return PHYSFS_eof(m_Handle);
}

size_t Vin::PhysfsGameFile::GetPos()
{
	return PHYSFS_tell(m_Handle);
}

bool Vin::PhysfsGameFile::SetPos(size_t pos)
{
	return PHYSFS_seek(m_Handle, pos);
}
