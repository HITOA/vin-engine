#include "virtualfilesystem.hpp"

bool Vin::VirtualFileSystem::AddFileSystem(std::shared_ptr<FileSystem> fs, bool priority)
{
	if (!fs->IsValid())
		return false;
	if (priority)
		m_FileSystems.insert(m_FileSystems.begin(), fs);
	else
		m_FileSystems.push_back(fs);
	return true;
}

bool Vin::VirtualFileSystem::RemoveFileSystem(std::shared_ptr<FileSystem> fs)
{
	auto it = std::find(m_FileSystems.begin(), m_FileSystems.end(), fs);
	if (it == m_FileSystems.end())
		return false;
	m_FileSystems.erase(it);
	return true;
}

bool Vin::VirtualFileSystem::SetWriteFileSystem(std::weak_ptr<FileSystem> fs)
{
	m_WriteFileSystem = fs;
	return true;
}

std::unique_ptr<Vin::File> Vin::VirtualFileSystem::Open(std::string_view path, FileMode mode)
{
	std::unique_ptr<File> file{ nullptr };
	for (auto& fs : m_FileSystems)
		if ((file = fs->Open(path, mode))->IsValid())
			break;
	return file;
}

bool Vin::VirtualFileSystem::Exists(std::string_view path)
{
	for (auto& fs : m_FileSystems)
		if (fs->Exists(path))
			return true;
	return false;
}
