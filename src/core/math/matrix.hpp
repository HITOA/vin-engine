#pragma once

namespace Vin {

	template<typename T, size_t M, size_t N>
	struct Matrix {
		T data[M * N];

		T& operator[](size_t m, size_t n) {
			return data[m + n * M];
		}
	};

}
