#include "pakfile.hpp"

Vin::PakFile::PakFile(char* buff, size_t buffsize) : m_Buff{ buff }, m_BuffSize{ buffsize }, m_BuffIdx{ 0 }
{
}

void Vin::PakFile::Close()
{
	delete[] m_Buff;
}

bool Vin::PakFile::IsValid()
{
	return m_BuffSize != 0;
}

size_t Vin::PakFile::ReadBytes(char* buff, size_t buffsize)
{
	size_t l = m_BuffSize - m_BuffIdx;
	l = l > buffsize ? buffsize : l;

	memcpy(buff, m_Buff, l);
	m_BuffIdx += l;

	return l;
}

size_t Vin::PakFile::WriteBytes(char* buff, size_t buffsize)
{
	return size_t();
}

bool Vin::PakFile::IsEof()
{
	return m_BuffIdx >= m_BuffSize;
}

size_t Vin::PakFile::GetSize()
{
	return m_BuffSize;
}

size_t Vin::PakFile::GetPos()
{
	return m_BuffIdx;
}

void Vin::PakFile::SetPos(size_t pos)
{
	if (pos > 0 && pos <= m_BuffSize)
		m_BuffIdx = pos;
}
