#include "gamefilesystem.hpp"

#include <assert.hpp>

#include "physfs/gamefilesystem_physfs.hpp"

Vin::GameFilesystem::GameFilesystemApi* Vin::GameFilesystem::s_GameFilesystemApi = nullptr;

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
	return s_GameFilesystemApi->Exists(filepath);
}

eastl::unique_ptr<Vin::GameFile> Vin::GameFilesystem::Open(const char* filepath, FileMode mod)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	return s_GameFilesystemApi->Open(filepath, mod);
}

eastl::unique_ptr<Vin::GameFile> Vin::GameFilesystem::Create(const char* filepath)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	return s_GameFilesystemApi->Create(filepath);
}

void Vin::GameFilesystem::Delete(const char* filepath)
{
	VIN_ASSERT(s_GameFilesystemApi != nullptr, "Game file system not initialized.");
	s_GameFilesystemApi->Delete(filepath);
}
