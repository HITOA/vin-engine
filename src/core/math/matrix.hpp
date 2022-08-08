#pragma once

#include "vector.hpp"

namespace Vin {

	template<typename T, size_t M, size_t N>
	struct Matrix {
		T data[M * N];

		Matrix() : data{} {}

		template<typename... Args>
		Matrix(TArgs... elements) : data{ elements... } {}

		T& operator()(size_t m, size_t n) {
			return data[m + n * M];
		}

		T operator()(size_t m, size_t n) const {
			return data[m + n * M];
		}
	};

	template<typename T>
	struct Matrix3x3 : Matrix<T, 3, 3> {
		Matrix3x3() : Matirx{} {}
		Matrix3x3(T v) : Matrix{ v, v, v, v, v, v, v, v, v } {}
	};

	template<typename T>
	struct Matrix4x4 : Matrix<T, 4, 4> {

	};
}
