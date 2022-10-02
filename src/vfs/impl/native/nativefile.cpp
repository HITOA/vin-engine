#include "nativefile.hpp"

Vin::NativeFile::NativeFile(const std::filesystem::path& path, FileMode mode) {
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

bool Vin::NativeFile::IsValid()
{
	return m_FileStream.is_open();
}

size_t Vin::NativeFile::ReadBytes(char* buff, size_t buffsize)
{
	size_t currpos = GetPos();
	m_FileStream.read(buff, buffsize);
	return GetPos() - currpos;
}

size_t Vin::NativeFile::WriteBytes(char* buff, size_t buffsize)
{
	size_t currpos = m_FileStream.tellp();
	m_FileStream.write(buff, buffsize);
	return (size_t)m_FileStream.tellp() - currpos;
}

bool Vin::NativeFile::IsEof()
{
	return m_FileStream.rdstate() & std::ios_base::eofbit;
}

size_t Vin::NativeFile::GetSize()
{
	size_t currpos = GetPos();
	m_FileStream.seekg(std::ios_base::end);
	size_t size = m_FileStream.tellg();
	m_FileStream.seekg(currpos);
	return size;
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
