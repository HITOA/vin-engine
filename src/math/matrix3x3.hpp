#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix3x3 : Matrix<T, 3, 3> {
		static const Matrix3x3<T> identity;

		Matrix3x3() : Matrix<T, 3, 3>{} {}
		Matrix3x3(T v) : Matrix<T, 3, 3>{} {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] = v;
		}
		Matrix3x3(
			T v1, T v2, T v3,
			T v4, T v5, T v6,
			T v7, T v8, T v9) :
			Matrix<T, 3, 3>{ v1, v2, v3, v4, v5, v6, v7, v8, v9 } {}
		Matrix3x3(const Matrix<T, 3, 3>& rhs) : Matrix<T, 3, 3>{} {
			memcpy(this->data, rhs.data, this->rows * this->cols * sizeof(T));
		}

		T& operator()(size_t c, size_t r) {
			return this->data[r + c * this->cols];
		}
		T operator()(size_t c, size_t r) const {
			return this->data[r + c * this->cols];
		}

		//Assignment

		Matrix3x3<T>& operator=(const Matrix3x3<T>& rhs) {
			memcpy(this->data, rhs.data, sizeof(T) * this->rows * this->cols);
			return *this;
		}

		Matrix3x3<T>& operator+=(const Matrix3x3<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] += rhs.data[i];
			return *this;
		}

		Matrix3x3<T>& operator-=(const Matrix3x3<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] -= rhs.data[i];
			return *this;
		}

		Matrix3x3<T>& operator*=(T rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix3x3<T> operator+(
			Matrix3x3<T> lhs, const Matrix3x3<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x3<T> operator-(
			Matrix3x3<T> lhs, const Matrix3x3<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x3<T> operator*(
			Matrix3x3<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};

	template<typename T>
	const  Matrix3x3<T> Matrix3x3<T>::identity = Matrix3x3<T>{ 1, 0, 0, 0, 1, 0, 0, 0, 1 };
}
