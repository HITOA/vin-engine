#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix3x2 : Matrix<T, 3, 2> {
		Matrix3x2() : Matrix<T, 3, 2>{} {}
		Matrix3x2(T v) : Matrix<T, 3, 2>{} {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] = v;
		}
		Matrix3x2(
			T v1, T v2, 
			T v3, T v4, 
			T v5, T v6) :
			Matrix<T, 3, 2>{ v1, v2, v3, v4, v5, v6 } {}
		Matrix3x2(const Matrix<T, 3, 2>& rhs) : Matrix<T, 3, 2>{} {
			memcpy(this->data, rhs.data, this->rows * this->cols * sizeof(T));
		}

		T& operator()(size_t c, size_t r) {
			return this->data[r + c * this->cols];
		}
		T operator()(size_t c, size_t r) const {
			return this->data[r + c * this->cols];
		}

		//Assignment

		Matrix3x2<T>& operator=(const Matrix3x2<T>& rhs) {
			memcpy(this->data, rhs.data, sizeof(T) * this->rows * this->cols);
			return *this;
		}

		Matrix3x2<T>& operator+=(const Matrix3x2<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] += rhs.data[i];
			return *this;
		}

		Matrix3x2<T>& operator-=(const Matrix3x2<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] -= rhs.data[i];
			return *this;
		}

		Matrix3x2<T>& operator*=(T rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] *= rhs;
			return *this;
		}

		//Arithmetic

		friend Matrix3x2<T> operator+(
			Matrix3x2<T> lhs, const Matrix3x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x2<T> operator-(
			Matrix3x2<T> lhs, const Matrix3x2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Matrix3x2<T> operator*(
			Matrix3x2<T> lhs, T rhs) {
			lhs *= rhs;
			return lhs;
		}
	};
}
