#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix2x4 : Matrix<T, 2, 4> {
		Matrix2x4() : Matrix{} {}
		Matrix2x4(T v) : Matrix{} {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] = v;
		}
		Matrix2x4(
			T v1, T v2, T v3, T v4, 
			T v5, T v6, T v7, T v8) :
			Matrix{ v1, v2, v3, v4, v5, v6, v7, v8 } {}
		Matrix2x4(const Matrix<T, rows, cols>& rhs) : Matrix{} {
			memcpy(data, rhs.data, rows * cols * sizeof(T));
		}

		T& operator()(size_t m, size_t n) {
			return data[m + n * cols];
		}
		T operator()(size_t m, size_t n) const {
			return data[m + n * cols];
		}

		//Assignment

		Matrix2x4<T>& operator=(const Matrix2x4<T>& rhs) {
			memcpy(data, rhs.data, sizeof(T) * rows * cols);
			return *this;
		}

		Matrix2x4<T>& operator+=(const Matrix2x4<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] += rhs.data[i];
			return *this;
		}

		Matrix2x4<T>& operator-=(const Matrix2x4<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] -= rhs.data[i];
			return *this;
		}

		Matrix2x4<T>& operator*=(T rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix2x4<T> operator+(
			Matrix2x4<T> lhs, const Matrix2x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix2x4<T> operator-(
			Matrix2x4<T> lhs, const Matrix2x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix2x4<T> operator*(
			Matrix2x4<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};
}
