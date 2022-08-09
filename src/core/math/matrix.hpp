#pragma once

#include "matrix2x2.hpp"
#include "matrix2x3.hpp"
#include "matrix2x4.hpp"
#include "matrix3x2.hpp"
#include "matrix3x3.hpp"
#include "matrix3x4.hpp"
#include "matrix4x2.hpp"
#include "matrix4x3.hpp"
#include "matrix4x4.hpp"

template<typename T, size_t m1, size_t n1, size_t n2>
auto operator*(const Vin::Matrix<T, m1, n1>& lhs, const Vin::Matrix<T, n1, n2>& rhs) {
	Vin::Matrix<T, m1, n2>::MatrixType::type result{};

	for (size_t y = 0; y < m1; y++)
		for (size_t x = 0; x < n2; x++)
			for (size_t i = 0; i < n1; i++)
				result.data[x + y * n2] += lhs.data[i + y * n1] * rhs.data[x + i * n2];

	return result;
}
