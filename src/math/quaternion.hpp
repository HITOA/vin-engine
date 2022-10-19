#pragma once

#include "mathhelper.hpp"
#include "vector_base.hpp"
#include "vector3.hpp"
#include "matrix.hpp"

namespace Vin {

	template<typename T>
	struct Quaternion {
		union {
			T data[4];

			SwizzleProxy1<T, 4, 0> w;
			SwizzleProxy1<T, 4, 1> x;
			SwizzleProxy1<T, 4, 2> y;
			SwizzleProxy1<T, 4, 3> z;
		};

		Quaternion() : data{ 1, 0, 0, 0 } {};
		Quaternion(const T& w, const T& x, const T& y, const T& z) : data{ w, x, y, z } {};
		Quaternion(const Quaternion<T>& quaternion) : data{ quaternion.w, quaternion.x, quaternion.y, quaternion.z } {};

		T Length() const {
			T length = Sqrt<T>(w * w + x * x + y * y + z * z);
			return length;
		}

		Quaternion<T> Normalize() const {
			T length = Length();
			return Quaternion<T>(w / length, x / length, y / length, z / length);
		}

		Quaternion<T> Conjugate() const {
			return Quaternion{ w, -x, -y, -z };
		}

		Matrix4x4<T> GetRotationMatrix() const {
			Matrix4x4<T> r{ Matrix4x4<T>::identity };

			r(0, 0) = 1 - 2 * y * y - 2 * z * z;
			r(0, 1) = 2 * (x * y + w * z);
			r(0, 2) = 2 * (x * z - w * y);

			r(1, 0) = 2 * (x * y - w * z);
			r(1, 1) = 1 - 2 * x * x - 2 * z * z;
			r(1, 2) = 2 * (y * z + w * x);

			r(2, 0) = 2 * (x * z + w * y);
			r(2, 1) = 2 * (y * z - w * x);
			r(2, 2) = 1 - 2 * x * x - 2 * y * y;

			return r;
		}

		static Quaternion<T> Euler(Vector3<T> rot) {
			T cr = Cos<T>(rot.x * (T)0.5);
			T sr = Sin<T>(rot.x * (T)0.5);
			T cp = Cos<T>(rot.y * (T)0.5);
			T sp = Sin<T>(rot.y * (T)0.5);
			T cy = Cos<T>(rot.z * (T)0.5);
			T sy = Sin<T>(rot.z * (T)0.5);

			Quaternion<T> q{ 0, 0, 0, 0 };
			q.w = cr * cp * cy + sr * sp * sy;
			q.x = sr * cp * cy - cr * sp * sy;
			q.y = cr * sp * cy + sr * cp * sy;
			q.z = cr * cp * sy - sr * sp * cy;

			return q;
		}

		Quaternion<T>& operator+=(const Quaternion<T>& rhs) {
			w += rhs.w;
			x += rhs.x;
			y += rhs.y;
			z += rhs.y;
			return *this;
		}
		Quaternion<T>& operator-=(const Quaternion<T>& rhs) {
			w -= rhs.w;
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.y;
			return *this;
		}

		Quaternion<T>& operator*=(const Quaternion<T>& rhs) {
			//w x y z
			//  w1     w2 - x1     x2 - y1     y2 - z1      z2
			w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
			//  w1     x2 + x1     w2 + y1     z2 - z1      y2
			x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
			//  w1     y2 - x1     z2 + y1     w2 + z1      x2
			y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
			//  w1     z2 + x1     y2 - y1     x2 + z1      w2
			z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
			return *this;
		}

		friend Quaternion<T> operator+(
			Quaternion<T> lhs, const Quaternion<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Quaternion<T> operator-(
			Quaternion<T> lhs, const Quaternion<T>& rhs) {
			lhs -= rhs;
			return lhs;
		}

		friend Quaternion<T> operator*(
			Quaternion<T> lhs, const Quaternion<T>& rhs) {
			lhs *= rhs;
			return lhs;
		}
	};

}
