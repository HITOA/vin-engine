#pragma once

#include <vinpch.hpp>

namespace Vin {
	class VinTimer {
	public:
		VinTimer();
		
		long long GetElapsedNanoSecond();
		double GetElapsedMiliSecond();
		double GetElapsedSecond();
	private:
		timespec m_StartTime{0};
	};
}
