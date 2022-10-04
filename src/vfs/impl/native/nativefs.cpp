#include "nativefs.hpp"
#include "nativefile.hpp"

Vin::NativeFS::NativeFS(std::string_view root) : m_Root{ root }
{
	m_Root = std::filesystem::absolute(m_Root);
}

bool Vin::NativeFS::IsValid()
{
	return std::filesystem::exists(m_Root);
}

std::unique_ptr<Vin::File> Vin::NativeFS::Open(std::string_view path, FileMode mode)
{
	std::filesystem::path fullpath = m_Root / std::filesystem::path(path);
	return std::unique_ptr<Vin::File>{ new NativeFile{ fullpath, mode } };
}

bool Vin::NativeFS::Exists(std::string_view path)
{
	std::filesystem::path fullpath = m_Root / std::filesystem::path(path);
	return std::filesystem::exists(fullpath);
}
