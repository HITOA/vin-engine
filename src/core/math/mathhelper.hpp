#pragma once

#include "vinpch.hpp"

#include <cmath>

namespace Vin {
	
	static constexpr double PI = 3.1415926535897931;
	static constexpr double deg2rad = PI / 180.0;
	static constexpr double rad2deg = 180.0 / PI;

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

	template<typename T>
	inline T Cos(T v) { return T{}; }
	template<> 
	inline double Cos<double>(double v) { return cos(v); }
	template<>
	inline float Cos<float>(float v) { return cosf(v); }

	template<typename T>
	inline T Sin(T v) { return T{}; }
	template<>
	inline double Sin<double>(double v) { return sin(v); }
	template<>
	inline float Sin<float>(float v) { return sinf(v); }

	template<typename T>
	inline T Tan(T v) { return T{}; }
	template<>
	inline double Tan<double>(double v) { return tan(v); }
	template<>
	inline float Tan<float>(float v) { return tanf(v); }

	template<typename T>
	inline T Max(T v1, T v2) {
		return v1 >= v2 ? v1 : v2;
	}
}
