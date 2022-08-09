#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix4x4 : Matrix<T, 4, 4> {
		static const Matrix4x4<T> identity;

		Matrix4x4() : Matrix{} {}
		Matrix4x4(T v) : Matrix{} {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] = v;
		}
		Matrix4x4(
			T v1, T v2, T v3, T v4, 
			T v5, T v6, T v7, T v8, 
			T v9, T v10, T v11, T v12,
			T v13, T v14, T v15, T v16) :
			Matrix{ v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16 } {}
		Matrix4x4(const Matrix<T, rows, cols>& rhs) : Matrix{} {
			memcpy(data, rhs.data, rows * cols * sizeof(T));
		}

		T& operator()(size_t m, size_t n) {
			return data[m + n * cols];
		}
		T operator()(size_t m, size_t n) const {
			return data[m + n * cols];
		}

		//Assignment

		Matrix4x4<T>& operator=(const Matrix4x4<T>& rhs) {
			memcpy(data, rhs.data, sizeof(T) * rows * cols);
			return *this;
		}

		Matrix4x4<T>& operator+=(const Matrix4x4<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] += rhs.data[i];
			return *this;
		}

		Matrix4x4<T>& operator-=(const Matrix4x4<T>& rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] -= rhs.data[i];
			return *this;
		}

		Matrix4x4<T>& operator*=(T rhs) {
			for (size_t i = 0; i < rows * cols; i++)
				data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix4x4<T> operator+(
			Matrix4x4<T> lhs, const Matrix4x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix4x4<T> operator-(
			Matrix4x4<T> lhs, const Matrix4x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix4x4<T> operator*(
			Matrix4x4<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};

	template<typename T>
	const  Matrix4x4<T> Matrix4x4<T>::identity = Matrix4x4<T>{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}
