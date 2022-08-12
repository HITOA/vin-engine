#include "gamefilesystem.hpp"

#include "core/assert.hpp"

#include "platform/physfs/gamefilesystem_physfs.hpp"

void Vin::GameFilesystem::Init()
{
	if (s_GameFilesystemApi != nullptr)
		return;

	s_GameFilesystemApi = new PhysfsGameFilesystemApi{};
}

void Vin::GameFilesystem::Terminate()
{
	delete s_GameFilesystemApi;
}

void Vin::GameFilesystem::Mount(const char* path)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	s_GameFilesystemApi->Mount(path);
}

bool Vin::GameFilesystem::Exists(const char* filepath)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	s_GameFilesystemApi->Exists(filepath);
}

std::unique_ptr<Vin::GameFile> Vin::GameFilesystem::Open(const char* filepath, FileMode mod)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	return s_GameFilesystemApi->Open(filepath, mod);
}

void Vin::GameFilesystem::Delete(const char* filepath)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	s_GameFilesystemApi->Delete(filepath);
}
