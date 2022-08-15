#include "vinptr.hpp"

void Vin::RefCounter::Increment()
{
	m_RefCounter++;
}

void Vin::RefCounter::Decrement()
{
	m_RefCounter--;
}

unsigned short Vin::RefCounter::Get() const
{
	return m_RefCounter;
}

void Vin::RefCounter::operator++()
{
	m_RefCounter++;
}

void Vin::RefCounter::operator++(int)
{
	m_RefCounter++;
}

void Vin::RefCounter::operator--()
{
	m_RefCounter--;
}

void Vin::RefCounter::operator--(int)
{
	m_RefCounter--;
}
