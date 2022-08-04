#pragma once

#include "vinpch.hpp"

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

	class VinTimer {
	public:
		VinTimer();
		
		long long GetElapsedNanosecond();
		double GetElapsedMillisecond();
		double GetElapsedSecond();

		TimeStep GetTimeStep();

		void Wait(double ms);
	private:
		timespec m_StartTime{0};
	};
}
