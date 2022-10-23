#pragma once

#include "matrix_base.hpp"

namespace Vin {

	template<typename T>
	struct Matrix2x2 : Matrix<T, 2, 2> {
		static const Matrix2x2<T> identity;

		Matrix2x2() : Matrix<T, 2, 2> {} {}
		Matrix2x2(T v) : Matrix<T, 2, 2> {} {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] = v;
		}
		Matrix2x2(
			T v1, T v2, 
			T v3, T v4) :
			Matrix<T, 2, 2> { v1, v2, v3, v4 } {}
		Matrix2x2(const Matrix<T, 2, 2>& rhs) : Matrix<T, 2, 2> {} {
			memcpy(this->data, rhs.data, this->rows * this->cols * sizeof(T));
		}

		T& operator()(size_t c, size_t r) {
			return this->data[r + c * this->cols];
		}
		T operator()(size_t c, size_t r) const {
			return this->data[r + c * this->cols];
		}

		//Assignment

		Matrix2x2<T>& operator=(const Matrix2x2<T>& rhs) {
			memcpy(this->data, rhs.data, sizeof(T) * this->rows * this->cols);
			return *this;
		}

		Matrix2x2<T>& operator+=(const Matrix2x2<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] += rhs.data[i];
			return *this;
		}

		Matrix2x2<T>& operator-=(const Matrix2x2<T>& rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] -= rhs.data[i];
			return *this;
		}

		Matrix2x2<T>& operator*=(T rhs) {
			for (size_t i = 0; i < this->rows * this->cols; i++)
                this->data[i] *= rhs;
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
