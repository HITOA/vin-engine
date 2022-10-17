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

		Quaternion() : data{ 0, 0, 0, 0 } {};
		Quaternion(const T& x, const T& y, const T& z, const T& w) : data{ x, y, z, w } {};

		Matrix4x4<T> GetRotationMatrix() {
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

			Quaternion<T> q;
			q.w = cr * cp * cy + sr * sp * sy;
			q.x = sr * cp * cy - cr * sp * sy;
			q.y = cr * sp * cy + sr * cp * sy;
			q.z = cr * cp * sy - sr * sp * cy;

			return q;
		}
	};

}
