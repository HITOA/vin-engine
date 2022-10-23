#pragma once

#include "vinpch.hpp"

namespace Vin {
	template<typename T>
	struct Matrix2x2;
	template<typename T>
	struct Matrix2x3;
	template<typename T>
	struct Matrix2x4;
	template<typename T>
	struct Matrix3x2;
	template<typename T>
	struct Matrix3x3;
	template<typename T>
	struct Matrix3x4;
	template<typename T>
	struct Matrix4x2;
	template<typename T>
	struct Matrix4x3;
	template<typename T>
	struct Matrix4x4;

	template<typename T, size_t m, size_t n>
	struct MatrixType;

	template<typename T, size_t m, size_t n>
	struct Matrix {
		using MType = MatrixType<T, m, n>;

		static constexpr size_t rows = m;
		static constexpr size_t cols = n;

		T data[m * n];

		Matrix() : data{ 0 } {}
		template<typename... Args>
		Matrix(T v1, Args... vs) : data{ v1, vs... } {}
	};
	
	template<typename T, size_t m, size_t n>
	struct MatrixType {
		using type = Matrix<T, m, n>;
	};

	template<typename T>
	struct MatrixType<T, 2, 2> {
		using type = Matrix2x2<T>;
	};
	template<typename T>
	struct MatrixType<T, 2, 3> {
		using type = Matrix2x3<T>;
	};
	template<typename T>
	struct MatrixType<T, 2, 4> {
		using type = Matrix2x4<T>;
	};
	template<typename T>
	struct MatrixType<T, 3, 2> {
		using type = Matrix3x2<T>;
	};
	template<typename T>
	struct MatrixType<T, 3, 3> {
		using type = Matrix3x3<T>;
	};
	template<typename T>
	struct MatrixType<T, 3, 4> {
		using type = Matrix3x4<T>;
	};
	template<typename T>
	struct MatrixType<T, 4, 2> {
		using type = Matrix4x2<T>;
	};
	template<typename T>
	struct MatrixType<T, 4, 3> {
		using type = Matrix4x3<T>;
	};
	template<typename T>
	struct MatrixType<T, 4, 4> {
		using type = Matrix4x4<T>;
	};
}