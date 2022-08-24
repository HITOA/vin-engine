#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix4x2 : Matrix<T, 4, 2> {
		Matrix4x2() : Matrix{} {}
		Matrix4x2(T v) : Matrix{} {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] = v;
		}
		Matrix4x2(
			T v1, T v2, 
			T v3, T v4, 
			T v5, T v6, 
			T v7, T v8) :
			Matrix{ v1, v2, v3, v4, v5, v6, v7, v8 } {}
		Matrix4x2(const Matrix<T, 4, 2>& rhs) : Matrix{} {
			memcpy(data, rhs.data, rows * cols * sizeof(T));
		}

		T& operator()(size_t c, size_t r) {
			return data[r + c * cols];
		}
		T operator()(size_t c, size_t r) const {
			return data[r + c * cols];
		}

		//Assignment

		Matrix4x2<T>& operator=(const Matrix4x2<T>& rhs) {
			memcpy(data, rhs.data, sizeof(T) * rows * cols);
			return *this;
		}

		Matrix4x2<T>& operator+=(const Matrix4x2<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] += rhs.data[i];
			return *this;
		}

		Matrix4x2<T>& operator-=(const Matrix4x2<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] -= rhs.data[i];
			return *this;
		}

		Matrix4x2<T>& operator*=(T rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix4x2<T> operator+(
			Matrix4x2<T> lhs, const Matrix4x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix4x2<T> operator-(
			Matrix4x2<T> lhs, const Matrix4x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix4x2<T> operator*(
			Matrix4x2<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};
}
