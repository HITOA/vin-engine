#ifndef VIN_ENGINE_TIMER_H
#define VIN_ENGINE_TIMER_H

#include <time.h>

namespace Vin {
    struct TimeStep {
    private:
        long long ns;

    public:
        TimeStep(long long ns);

        long long GetNanosecond();
        double GetMillisecond();
        double GetSecond();

        TimeStep operator-(TimeStep& ts);
        TimeStep operator+(TimeStep& ts);
        TimeStep& operator+=(TimeStep& ts);
        TimeStep& operator-=(TimeStep& ts);
    };

    class Timer {
    public:
        Timer();

        long long GetElapsedNanosecond();
        double GetElapsedMillisecond();
        double GetElapsedSecond();

        TimeStep GetTimeStep();

        void Wait(double ms);
    private:
        timespec m_StartTime{0};
    };
}

#endif //VIN_ENGINE_TIMER_H
