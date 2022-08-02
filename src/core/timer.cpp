#include "timer.hpp"

#include "assert.hpp"

Vin::VinTimer::VinTimer()
{
	VIN_ASSERT(timespec_get(&m_StartTime, TIME_UTC) == TIME_UTC, "timespec_get failed.")
}

inline timespec GetTimeSpec() {
	timespec spec{};

	VIN_ASSERT(timespec_get(&spec, TIME_UTC) == TIME_UTC, "timespec_get failed.")

	return spec;
}

long long Vin::VinTimer::GetElapsedNanosecond()
{
	timespec currentTime = GetTimeSpec();
	return (currentTime.tv_sec - m_StartTime.tv_sec) * (long)1e9 + (currentTime.tv_nsec - m_StartTime.tv_nsec);
}

double Vin::VinTimer::GetElapsedMillisecond()
{
	return GetElapsedNanosecond() / (double)1e6;
}

double Vin::VinTimer::GetElapsedSecond()
{
	return GetElapsedNanosecond() / (double)1e9;
}

Vin::TimeStep Vin::VinTimer::GetTimeStep()
{
	return TimeStep{ GetElapsedNanosecond() };
}

void Vin::VinTimer::Wait(double ms)
{
	double start = GetElapsedMillisecond();
	while ((GetElapsedMillisecond() - start) < ms) {
#ifdef _MSC_VER
		_mm_pause();
#else
		__builtin_ia32_pause();
#endif // _MSC_VER
	}
}

Vin::TimeStep::TimeStep(long long ns) : ns{ ns } {}

long long Vin::TimeStep::GetNanosecond()
{
	return ns;
}

double Vin::TimeStep::GetMillisecond()
{
	return ns / (double)1e6;
}

double Vin::TimeStep::GetSecond()
{
	return ns / (double)1e9;
}

Vin::TimeStep Vin::TimeStep::operator-(TimeStep& ts)
{
	return TimeStep{ this->ns - ts.ns };
}

Vin::TimeStep Vin::TimeStep::operator+(TimeStep& ts)
{
	return TimeStep{ this->ns + ts.ns };
}

Vin::TimeStep& Vin::TimeStep::operator+=(TimeStep& ts)
{
	this->ns += ts.ns;
	return *this;
}

Vin::TimeStep& Vin::TimeStep::operator-=(TimeStep& ts)
{
	this->ns -= ts.ns;
	return *this;
}
