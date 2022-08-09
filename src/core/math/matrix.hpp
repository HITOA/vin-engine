#pragma once

#include "vinpch.hpp"
#include "core/assert.hpp"
#include "vector.hpp"

namespace Vin {

	template<typename T, size_t m, size_t n>
	struct Matrix {
		T data[m * n];

		Matrix() : data{ 0 } {}
		template<typename... Args>
		Matrix(T v1, Args... vs) : data{ v1, vs... } {}
	};

	template<typename T>
	struct Matrix3x3 : Matrix<T, 3, 3> {
		Matrix3x3() : Matrix{} {}
		Matrix3x3(T v) : Matrix{} {
			for (size_t i = 0; i < 9; i++)
				data[i] = v;
		}
		Matrix3x3(
			T v1, T v2, T v3,
			T v4, T v5, T v6,
			T v7, T v8, T v9) : 
			Matrix{v1, v2, v3, v4, v5, v6, v7, v8, v9} {}

		T& operator()(size_t m, size_t n) {
			return data[m + n * 3];
		}

		T operator()(size_t m, size_t n) const {
			return data[m + n * 3];
		}

		Matrix3x3<T>& operator=(const Matrix3x3<T>& rhs) {
			memcpy(data, rhs.data, sizeof(T) * 9);
			return *this;
		}
	};
}
