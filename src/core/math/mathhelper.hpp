#pragma once

#include <cmath>

namespace Vin {
	
	template<typename T>
	inline void Ceil(T& v) {}
	template<>
	inline void Ceil<double>(double& v) { v = ceil(v); }
	template<>
	inline void Ceil<float>(float& v) { v = ceilf(v); }

	template<typename T>
	inline void Floor(T& v) {}
	template<>
	inline void Floor<float>(float& v) { v = floorf(v); }
	template<>
	inline void Floor<double>(double& v) { v = floor(v); }

	template<typename T>
	inline void Round(T& v) {}
	template<>
	inline void Round<float>(float& v) { v = roundf(v); }
	template<>
	inline void Round<double>(double& v) { v = round(v); }

	template<typename T>
	inline void Clamp(T& v, T m, T M) {}
	template<>
	inline void Clamp<float>(float& v, float m, float M) {
		const float t = v < m ? m : v;
		v = t > M ? M : t;
	}
	template<>
	inline void Clamp<double>(double& v, double m, double M) {
		const double t = v < m ? m : v;
		v = t > M ? M : t;
	}

}
