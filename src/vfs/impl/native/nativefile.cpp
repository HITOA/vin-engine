#include "nativefile.hpp"

Vin::NativeFile::NativeFile(const std::filesystem::path& path, FileMode mode) : m_Path{ path } {
	static std::ios_base::openmode openmode{};

	switch (mode)
	{
	case Vin::FileMode::Read:
		openmode |= std::ios_base::binary | std::ios_base::in;
		break;
	case Vin::FileMode::Write:
		openmode |= std::ios_base::binary | std::ios_base::in | std::ios_base::out;
		break;
	case Vin::FileMode::WriteAppend:
		openmode |= std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::app;
		break;
	default:
		break;
	}

	m_FileStream = std::fstream{ path, openmode };
}

void Vin::NativeFile::Close()
{
	m_FileStream.close();
}

bool Vin::NativeFile::IsValid()
{
	return m_FileStream.is_open();
}

size_t Vin::NativeFile::ReadBytes(char* buff, size_t buffsize)
{
	m_FileStream.read(buff, buffsize);
	return m_FileStream.gcount();
}

size_t Vin::NativeFile::WriteBytes(char* buff, size_t buffsize)
{
	m_FileStream.write(buff, buffsize);
	return m_FileStream.gcount();
}

bool Vin::NativeFile::IsEof()
{
	return m_FileStream.rdstate() & std::ios_base::eofbit;
}

size_t Vin::NativeFile::GetSize()
{
	return std::filesystem::file_size(m_Path);
}

size_t Vin::NativeFile::GetPos()
{
	return m_FileStream.tellg();
}

void Vin::NativeFile::SetPos(size_t pos)
{
	m_FileStream.seekg(pos);
	m_FileStream.seekp(pos);
}
