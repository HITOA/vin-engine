#pragma once

#include "vinpch.hpp"

#include <cmath>

namespace Vin {
	
	template<typename T>
	inline T Ceil(T v) { return T{}; }
	template<>
	inline double Ceil<double>(double v) { return ceil(v); }
	template<>
	inline float Ceil<float>(float v) { return ceilf(v); }

	template<typename T>
	inline T Floor(T v) { return T{}; }
	template<>
	inline double Floor<double>(double v) { return floor(v); }
	template<>
	inline float Floor<float>(float v) { return floorf(v); }

	template<typename T>
	inline T Round(T v) { return T{}; }
	template<>
	inline double Round<double>(double v) { return round(v); }
	template<>
	inline float Round<float>(float v) { return roundf(v); }

	template<typename T>
	inline T Clamp(T v, T m, T M) {
		const T t = v < m ? m : v;
		return t > M ? M : t;
	}

	template<typename T>
	inline T Sqrt(T v) { return T{}; }
	template<>
	inline double Sqrt<double>(double v) { return sqrt(v); }
	template<>
	inline float Sqrt<float>(float v) { return sqrtf(v); }
}
