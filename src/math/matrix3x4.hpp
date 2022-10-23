#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix3x4 : Matrix<T, 3, 4> {
		Matrix3x4() : Matrix<T, 3, 4>{} {}
		Matrix3x4(T v) : Matrix<T, 3, 4>{} {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] = v;
		}
		Matrix3x4(
			T v1, T v2, T v3, T v4, 
			T v5, T v6, T v7, T v8, 
			T v9, T v10, T v11, T v12) :
			Matrix<T, 3, 4>{ v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12 } {}
		Matrix3x4(const Matrix<T, 3, 4>& rhs) : Matrix<T, 3, 4>{} {
			memcpy(this->data, rhs.data, this->rows * this->cols * sizeof(T));
		}

		T& operator()(size_t c, size_t r) {
			return this->data[r + c * this->cols];
		}
		T operator()(size_t c, size_t r) const {
			return this->data[r + c * this->cols];
		}

		//Assignment

		Matrix3x4<T>& operator=(const Matrix3x4<T>& rhs) {
			memcpy(this->data, rhs.data, sizeof(T) * this->rows * this->cols);
			return *this;
		}

		Matrix3x4<T>& operator+=(const Matrix3x4<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] += rhs.data[i];
			return *this;
		}

		Matrix3x4<T>& operator-=(const Matrix3x4<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] -= rhs.data[i];
			return *this;
		}

		Matrix3x4<T>& operator*=(T rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix3x4<T> operator+(
			Matrix3x4<T> lhs, const Matrix3x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x4<T> operator-(
			Matrix3x4<T> lhs, const Matrix3x4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x4<T> operator*(
			Matrix3x4<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};
}
