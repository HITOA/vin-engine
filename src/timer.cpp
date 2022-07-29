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

long long Vin::VinTimer::GetElapsedNanoSecond()
{
	timespec currentTime = GetTimeSpec();
	return (currentTime.tv_sec - m_StartTime.tv_sec) * (long)1e9 + (currentTime.tv_nsec - m_StartTime.tv_nsec);
}

double Vin::VinTimer::GetElapsedMiliSecond()
{
	return GetElapsedNanoSecond() / (double)1e6;
}

double Vin::VinTimer::GetElapsedSecond()
{
	return GetElapsedNanoSecond() / (double)1e9;
}
