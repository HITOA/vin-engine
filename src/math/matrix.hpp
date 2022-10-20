#pragma once

#include "matrix2x2.hpp"
#include "matrix2x3.hpp"
#include "matrix2x4.hpp"
#include "matrix3x2.hpp"
#include "matrix3x3.hpp"
#include "matrix3x4.hpp"
#include "matrix4x2.hpp"
#include "matrix4x3.hpp"
#include "matrix4x4.hpp"

#include "vector.hpp"

//#include <immintrin.h>

namespace Vin {

	template<typename T, size_t m1, size_t n1, size_t n2>
	auto operator*(const Vin::Matrix<T, m1, n1>& lhs, const Vin::Matrix<T, n1, n2>& rhs) {
		Vin::Matrix<T, m1, n2>::MatrixType::type result{};

		for (size_t y = 0; y < m1; y++)
			for (size_t x = 0; x < n2; x++)
				for (size_t i = 0; i < n1; i++)
					result.data[x + y * n2] += rhs.data[i + y * n1] * lhs.data[x + i * n2];

		return result;
	}

	/*template<>
	inline auto operator*<float, 4, 4, 4>(const Vin::Matrix<float, 4, 4>& lhs, const Vin::Matrix<float, 4, 4>& rhs) {
		Vin::Matrix<float, 4, 4>::MatrixType::type result{};

		__m128 lhsr[4]{ { lhs.data[0], lhs.data[4] , lhs.data[8] ,lhs.data[12] },
						{ lhs.data[1], lhs.data[5] , lhs.data[9] ,lhs.data[13] },
						{ lhs.data[2], lhs.data[6] , lhs.data[10] ,lhs.data[14] },
						{ lhs.data[3], lhs.data[7] , lhs.data[11] ,lhs.data[15] } };

		__m128 rhsr[4]{ _mm_load_ps(rhs.data), _mm_load_ps(rhs.data + 4), 
			_mm_load_ps(rhs.data + 8), _mm_load_ps(rhs.data + 12) };

		for (size_t y = 0; y < 4; y++) {
			for (size_t x = 0; x < 4; x++) {
				__m128 r = _mm_mul_ps(lhsr[x], rhsr[y]);
				r = _mm_hadd_ps(r, r);
				r = _mm_hadd_ps(r, r);
				_mm_store_ss(&result.data[x + y * 4], r);
			}
		}

		return result;
	}*/

	template<typename T, size_t m1, size_t n1>
	auto operator*(const Vin::Matrix<T, m1, n1>& lhs, const Vin::Vector<T, n1>& rhs) {
		Vin::Vector<T, m1>::type result{};

		for (size_t y = 0; y < m1; y++)
			for (size_t i = 0; i < n1; i++)
				result.data[y] += lhs.data[i + y * n1] * rhs.data[i];

		return result;
	}
}