#include "vin/time/timer.h"
#include "vin/core/error/error.h"

Vin::Timer::Timer()
{
    int s = timespec_get(&m_StartTime, TIME_UTC);
    ASSERT(s == TIME_UTC, "timespec_get failed.")
}

inline timespec GetTimeSpec() {
    timespec spec{};
    int s = timespec_get(&spec, TIME_UTC);
    ASSERT(s == TIME_UTC, "timespec_get failed.")

    return spec;
}

long long Vin::Timer::GetElapsedNanosecond()
{
    timespec currentTime = GetTimeSpec();
    return (currentTime.tv_sec - m_StartTime.tv_sec) * (long)1e9 + (currentTime.tv_nsec - m_StartTime.tv_nsec);
}

double Vin::Timer::GetElapsedMillisecond()
{
    return GetElapsedNanosecond() / (double)1e6;
}

double Vin::Timer::GetElapsedSecond()
{
    return GetElapsedNanosecond() / (double)1e9;
}

Vin::TimeStep Vin::Timer::GetTimeStep()
{
    return TimeStep{ GetElapsedNanosecond() };
}

void Vin::Timer::Wait(double ms)
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