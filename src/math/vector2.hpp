#pragma once

#include "vector_base.hpp"

namespace Vin {
	template<typename T>
	struct Vector<T, 2> {
		using type = Vector2<T>;

		union {
			T data[2];

			SwizzleProxy1<T, 2, 0> x;
			SwizzleProxy1<T, 2, 1> y;
			SwizzleProxy2<T, 2, 0, 0> xx;
			SwizzleProxy2<T, 2, 1, 0> yx;
			SwizzleProxy2<T, 2, 0, 1> xy;
			SwizzleProxy2<T, 2, 1, 1> yy;
			SwizzleProxy3<T, 2, 0, 0, 0> xxx;
			SwizzleProxy3<T, 2, 1, 0, 0> yxx;
			SwizzleProxy3<T, 2, 0, 1, 0> xyx;
			SwizzleProxy3<T, 2, 1, 1, 0> yyx;
			SwizzleProxy3<T, 2, 0, 0, 1> xxy;
			SwizzleProxy3<T, 2, 1, 0, 1> yxy;
			SwizzleProxy3<T, 2, 0, 1, 1> xyy;
			SwizzleProxy3<T, 2, 1, 1, 1> yyy;
			SwizzleProxy4<T, 2, 0, 0, 0, 0> xxxx;
			SwizzleProxy4<T, 2, 1, 0, 0, 0> yxxx;
			SwizzleProxy4<T, 2, 0, 1, 0, 0> xyxx;
			SwizzleProxy4<T, 2, 1, 1, 0, 0> yyxx;
			SwizzleProxy4<T, 2, 0, 0, 1, 0> xxyx;
			SwizzleProxy4<T, 2, 1, 0, 1, 0> yxyx;
			SwizzleProxy4<T, 2, 0, 1, 1, 0> xyyx;
			SwizzleProxy4<T, 2, 1, 1, 1, 0> yyyx;
			SwizzleProxy4<T, 2, 0, 0, 0, 1> xxxy;
			SwizzleProxy4<T, 2, 1, 0, 0, 1> yxxy;
			SwizzleProxy4<T, 2, 0, 1, 0, 1> xyxy;
			SwizzleProxy4<T, 2, 1, 1, 0, 1> yyxy;
			SwizzleProxy4<T, 2, 0, 0, 1, 1> xxyy;
			SwizzleProxy4<T, 2, 1, 0, 1, 1> yxyy;
			SwizzleProxy4<T, 2, 0, 1, 1, 1> xyyy;
			SwizzleProxy4<T, 2, 1, 1, 1, 1> yyyy;
		};

		Vector() : data{ T{}, T{} } {};
		Vector(const T& v) : data{ v, v } {};
		Vector(const T& x, const T& y) : data{ x, y } {};
	};

	template<typename T>
	struct Vector2 : Vector<T, 2>, VectorMath<Vector2<T>, 2> {
		Vector2() : Vector{} {};
		Vector2(const T& v) : Vector{ v } {};
		Vector2(const T& x, const T& y) : Vector{ x, y } {};

		//Func

		T Length() const {
			T length = Sqrt<T>(x * x + y * y);
			return length;
		}

		//Assignment

		Vector2<T>& operator=(const Vector2<T>& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			return *this;
		}

		Vector2<T>& operator+=(const Vector2<T>& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		Vector2<T>& operator-=(const Vector2<T>& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		Vector2<T>& operator*=(const Vector2<T>& rhs) {
			this->x *= rhs.x;
			this->y *= rhs.y;
			return *this;
		}

		Vector2<T>& operator/=(const Vector2<T>& rhs) {
			this->x /= rhs.x;
			this->y /= rhs.y;
			return *this;
		}

		//Arithmetic

		friend Vector2<T> operator+(
			Vector2<T> lhs, const Vector2<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Vector2<T> operator-(
			Vector2<T> lhs, const Vector2<T>& rhs) {
			lhs -= rhs;
			return lhs;
		}

		friend Vector2<T> operator*(
			Vector2<T> lhs, const Vector2<T>& rhs) {
			lhs *= rhs;
			return lhs;
		}

		friend Vector2<T> operator/(
			Vector2<T> lhs, const Vector2<T>& rhs) {
			lhs /= rhs;
			return lhs;
		}

		Vector2<T> operator-() const {
			Vector2<T> v{ *this };
			v *= -1;
			return v;
		}
	};
}
