#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix2x2 : Matrix<T, 2, 2> {
		static const Matrix2x2<T> identity;

		Matrix2x2() : Matrix{} {}
		Matrix2x2(T v) : Matrix{} {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] = v;
		}
		Matrix2x2(
			T v1, T v2, 
			T v3, T v4) :
			Matrix{ v1, v2, v3, v4 } {}
		Matrix2x2(const Matrix<T, rows, cols>& rhs) : Matrix{} {
			memcpy(data, rhs.data, rows * cols * sizeof(T));
		}

		T& operator()(size_t m, size_t n) {
			return data[m + n * cols];
		}
		T operator()(size_t m, size_t n) const {
			return data[m + n * cols];
		}

		//Assignment

		Matrix2x2<T>& operator=(const Matrix2x2<T>& rhs) {
			memcpy(data, rhs.data, sizeof(T) * rows * cols);
			return *this;
		}

		Matrix2x2<T>& operator+=(const Matrix2x2<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] += rhs.data[i];
			return *this;
		}

		Matrix2x2<T>& operator-=(const Matrix2x2<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] -= rhs.data[i];
			return *this;
		}

		Matrix2x2<T>& operator*=(T rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix2x2<T> operator+(
			Matrix2x2<T> lhs, const Matrix2x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix2x2<T> operator-(
			Matrix2x2<T> lhs, const Matrix2x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix2x2<T> operator*(
			Matrix2x2<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};

	template<typename T>
	const  Matrix2x2<T> Matrix2x2<T>::identity = Matrix2x2<T>{ 1, 0, 0, 1 };
}
