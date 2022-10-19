#pragma once

#include "vector_base.hpp"

namespace Vin {
	template<typename T>
	struct Vector<T, 3> {
		using type = Vector3<T>;

		union {
			T data[3];

			SwizzleProxy1<T, 3, 0> x;
			SwizzleProxy1<T, 3, 1> y;
			SwizzleProxy1<T, 3, 2> z;
			SwizzleProxy2<T, 3, 0, 0> xx;
			SwizzleProxy2<T, 3, 1, 0> yx;
			SwizzleProxy2<T, 3, 2, 0> zx;
			SwizzleProxy2<T, 3, 0, 1> xy;
			SwizzleProxy2<T, 3, 1, 1> yy;
			SwizzleProxy2<T, 3, 2, 1> zy;
			SwizzleProxy2<T, 3, 0, 2> xz;
			SwizzleProxy2<T, 3, 1, 2> yz;
			SwizzleProxy2<T, 3, 2, 2> zz;
			SwizzleProxy3<T, 3, 0, 0, 0> xxx;
			SwizzleProxy3<T, 3, 1, 0, 0> yxx;
			SwizzleProxy3<T, 3, 2, 0, 0> zxx;
			SwizzleProxy3<T, 3, 0, 1, 0> xyx;
			SwizzleProxy3<T, 3, 1, 1, 0> yyx;
			SwizzleProxy3<T, 3, 2, 1, 0> zyx;
			SwizzleProxy3<T, 3, 0, 2, 0> xzx;
			SwizzleProxy3<T, 3, 1, 2, 0> yzx;
			SwizzleProxy3<T, 3, 2, 2, 0> zzx;
			SwizzleProxy3<T, 3, 0, 0, 1> xxy;
			SwizzleProxy3<T, 3, 1, 0, 1> yxy;
			SwizzleProxy3<T, 3, 2, 0, 1> zxy;
			SwizzleProxy3<T, 3, 0, 1, 1> xyy;
			SwizzleProxy3<T, 3, 1, 1, 1> yyy;
			SwizzleProxy3<T, 3, 2, 1, 1> zyy;
			SwizzleProxy3<T, 3, 0, 2, 1> xzy;
			SwizzleProxy3<T, 3, 1, 2, 1> yzy;
			SwizzleProxy3<T, 3, 2, 2, 1> zzy;
			SwizzleProxy3<T, 3, 0, 0, 2> xxz;
			SwizzleProxy3<T, 3, 1, 0, 2> yxz;
			SwizzleProxy3<T, 3, 2, 0, 2> zxz;
			SwizzleProxy3<T, 3, 0, 1, 2> xyz;
			SwizzleProxy3<T, 3, 1, 1, 2> yyz;
			SwizzleProxy3<T, 3, 2, 1, 2> zyz;
			SwizzleProxy3<T, 3, 0, 2, 2> xzz;
			SwizzleProxy3<T, 3, 1, 2, 2> yzz;
			SwizzleProxy3<T, 3, 2, 2, 2> zzz;
			SwizzleProxy4<T, 3, 0, 0, 0, 0> xxxx;
			SwizzleProxy4<T, 3, 1, 0, 0, 0> yxxx;
			SwizzleProxy4<T, 3, 2, 0, 0, 0> zxxx;
			SwizzleProxy4<T, 3, 0, 1, 0, 0> xyxx;
			SwizzleProxy4<T, 3, 1, 1, 0, 0> yyxx;
			SwizzleProxy4<T, 3, 2, 1, 0, 0> zyxx;
			SwizzleProxy4<T, 3, 0, 2, 0, 0> xzxx;
			SwizzleProxy4<T, 3, 1, 2, 0, 0> yzxx;
			SwizzleProxy4<T, 3, 2, 2, 0, 0> zzxx;
			SwizzleProxy4<T, 3, 0, 0, 1, 0> xxyx;
			SwizzleProxy4<T, 3, 1, 0, 1, 0> yxyx;
			SwizzleProxy4<T, 3, 2, 0, 1, 0> zxyx;
			SwizzleProxy4<T, 3, 0, 1, 1, 0> xyyx;
			SwizzleProxy4<T, 3, 1, 1, 1, 0> yyyx;
			SwizzleProxy4<T, 3, 2, 1, 1, 0> zyyx;
			SwizzleProxy4<T, 3, 0, 2, 1, 0> xzyx;
			SwizzleProxy4<T, 3, 1, 2, 1, 0> yzyx;
			SwizzleProxy4<T, 3, 2, 2, 1, 0> zzyx;
			SwizzleProxy4<T, 3, 0, 0, 2, 0> xxzx;
			SwizzleProxy4<T, 3, 1, 0, 2, 0> yxzx;
			SwizzleProxy4<T, 3, 2, 0, 2, 0> zxzx;
			SwizzleProxy4<T, 3, 0, 1, 2, 0> xyzx;
			SwizzleProxy4<T, 3, 1, 1, 2, 0> yyzx;
			SwizzleProxy4<T, 3, 2, 1, 2, 0> zyzx;
			SwizzleProxy4<T, 3, 0, 2, 2, 0> xzzx;
			SwizzleProxy4<T, 3, 1, 2, 2, 0> yzzx;
			SwizzleProxy4<T, 3, 2, 2, 2, 0> zzzx;
			SwizzleProxy4<T, 3, 0, 0, 0, 1> xxxy;
			SwizzleProxy4<T, 3, 1, 0, 0, 1> yxxy;
			SwizzleProxy4<T, 3, 2, 0, 0, 1> zxxy;
			SwizzleProxy4<T, 3, 0, 1, 0, 1> xyxy;
			SwizzleProxy4<T, 3, 1, 1, 0, 1> yyxy;
			SwizzleProxy4<T, 3, 2, 1, 0, 1> zyxy;
			SwizzleProxy4<T, 3, 0, 2, 0, 1> xzxy;
			SwizzleProxy4<T, 3, 1, 2, 0, 1> yzxy;
			SwizzleProxy4<T, 3, 2, 2, 0, 1> zzxy;
			SwizzleProxy4<T, 3, 0, 0, 1, 1> xxyy;
			SwizzleProxy4<T, 3, 1, 0, 1, 1> yxyy;
			SwizzleProxy4<T, 3, 2, 0, 1, 1> zxyy;
			SwizzleProxy4<T, 3, 0, 1, 1, 1> xyyy;
			SwizzleProxy4<T, 3, 1, 1, 1, 1> yyyy;
			SwizzleProxy4<T, 3, 2, 1, 1, 1> zyyy;
			SwizzleProxy4<T, 3, 0, 2, 1, 1> xzyy;
			SwizzleProxy4<T, 3, 1, 2, 1, 1> yzyy;
			SwizzleProxy4<T, 3, 2, 2, 1, 1> zzyy;
			SwizzleProxy4<T, 3, 0, 0, 2, 1> xxzy;
			SwizzleProxy4<T, 3, 1, 0, 2, 1> yxzy;
			SwizzleProxy4<T, 3, 2, 0, 2, 1> zxzy;
			SwizzleProxy4<T, 3, 0, 1, 2, 1> xyzy;
			SwizzleProxy4<T, 3, 1, 1, 2, 1> yyzy;
			SwizzleProxy4<T, 3, 2, 1, 2, 1> zyzy;
			SwizzleProxy4<T, 3, 0, 2, 2, 1> xzzy;
			SwizzleProxy4<T, 3, 1, 2, 2, 1> yzzy;
			SwizzleProxy4<T, 3, 2, 2, 2, 1> zzzy;
			SwizzleProxy4<T, 3, 0, 0, 0, 2> xxxz;
			SwizzleProxy4<T, 3, 1, 0, 0, 2> yxxz;
			SwizzleProxy4<T, 3, 2, 0, 0, 2> zxxz;
			SwizzleProxy4<T, 3, 0, 1, 0, 2> xyxz;
			SwizzleProxy4<T, 3, 1, 1, 0, 2> yyxz;
			SwizzleProxy4<T, 3, 2, 1, 0, 2> zyxz;
			SwizzleProxy4<T, 3, 0, 2, 0, 2> xzxz;
			SwizzleProxy4<T, 3, 1, 2, 0, 2> yzxz;
			SwizzleProxy4<T, 3, 2, 2, 0, 2> zzxz;
			SwizzleProxy4<T, 3, 0, 0, 1, 2> xxyz;
			SwizzleProxy4<T, 3, 1, 0, 1, 2> yxyz;
			SwizzleProxy4<T, 3, 2, 0, 1, 2> zxyz;
			SwizzleProxy4<T, 3, 0, 1, 1, 2> xyyz;
			SwizzleProxy4<T, 3, 1, 1, 1, 2> yyyz;
			SwizzleProxy4<T, 3, 2, 1, 1, 2> zyyz;
			SwizzleProxy4<T, 3, 0, 2, 1, 2> xzyz;
			SwizzleProxy4<T, 3, 1, 2, 1, 2> yzyz;
			SwizzleProxy4<T, 3, 2, 2, 1, 2> zzyz;
			SwizzleProxy4<T, 3, 0, 0, 2, 2> xxzz;
			SwizzleProxy4<T, 3, 1, 0, 2, 2> yxzz;
			SwizzleProxy4<T, 3, 2, 0, 2, 2> zxzz;
			SwizzleProxy4<T, 3, 0, 1, 2, 2> xyzz;
			SwizzleProxy4<T, 3, 1, 1, 2, 2> yyzz;
			SwizzleProxy4<T, 3, 2, 1, 2, 2> zyzz;
			SwizzleProxy4<T, 3, 0, 2, 2, 2> xzzz;
			SwizzleProxy4<T, 3, 1, 2, 2, 2> yzzz;
			SwizzleProxy4<T, 3, 2, 2, 2, 2> zzzz;

		};

		Vector() : data{ T{}, T{}, T{} } {};
		Vector(const T& v) : data{ v, v, v } {};
		Vector(const T& x, const T& y, const T& z) : data{ x, y, z } {};
	};

	template<typename T>
	struct Vector3 : Vector<T, 3>, VectorMath<Vector3<T>, 3> {
		Vector3() : Vector{} {};
		Vector3(const T& v) : Vector{ v } {};
		Vector3(const T& x, const T& y, const T& z) : Vector{ x, y, z } {};
		Vector3(const Vector2<T>& xy, T z) : Vector{ xy.x, xy.y, z } {};

		Vector3<T> Cross(Vector3<T>& v1, Vector3<T>& v2) {
			Vector3<T> r{};
			r.x = v1.y * v2.z - v1.z * v2.y;
			r.y = v1.z * v2.x - v1.x * v2.z;
			r.z = v1.x * v2.y - v1.y * v2.x;
			return r;
		}

		T Length() const {
			T length = Sqrt<T>(x * x + y * y + z * z);
			return length;
		}

		Vector3<T> Normalize() const {
			if (x == y == z == 0)
				return *this;
			T length = Length();
			return Vector3<T>{ x / length, y / length, z / length};
		}

		//Assignment

		Vector3<T>& operator=(const Vector3<T>& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}

		Vector3<T>& operator+=(const Vector3<T>& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T>& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		Vector3<T>& operator*=(const Vector3<T>& rhs) {
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			return *this;
		}

		Vector3<T>& operator/=(const Vector3<T>& rhs) {
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			return *this;
		}

		//Arithmetic

		friend Vector3<T> operator+(
			Vector3<T> lhs, const Vector3<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Vector3<T> operator-(
			Vector3<T> lhs, const Vector3<T>& rhs) {
			lhs -= rhs;
			return lhs;
		}

		friend Vector3<T> operator*(
			Vector3<T> lhs, const Vector3<T>& rhs) {
			lhs *= rhs;
			return lhs;
		}

		friend Vector3<T> operator/(
			Vector3<T> lhs, const Vector3<T>& rhs) {
			lhs /= rhs;
			return lhs;
		}

		Vector3<T> operator-() const {
			Vector3<T> v{ *this };
			v *= -1;
			return v;
		}
	};
}
