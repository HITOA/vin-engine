#pragma once

#include "mathhelper.hpp"

namespace Vin {
	template<typename T>
	Matrix4x4<T> Scale(const Vector3<T>& scale) {
		Matrix4x4<T> result{
			scale.x, T{}, T{}, T{},
			T{}, scale.y, T{}, T{},
			T{}, T{}, scale.z, T{},
			T{}, T{}, T{}, (T)1 };
		return result;
	}

	template<typename T>
	Matrix4x4<T>& Scale(Matrix4x4<T>& result, const Vector3<T>& scale) {
		Matrix4x4<T> mat{ Scale(scale) };
		result = result * mat;
		return result;
	}

	template<typename T>
	Matrix4x4<T> Rotate(const Vector3<T>& axis, const T& angle) {
		Matrix4x4<T> result{ Matrix4x4<T>::identity };

		const T c = Cos(angle);
		const T s = Sin(angle);
		const T t = (T)1 - c;

		result(0, 0) = c + axis.x * axis.x * t;
		result(1, 0) = axis.x * axis.y * t - axis.z * s;
		result(2, 0) = axis.x * axis.z * t + axis.y * s;

		result(0, 1) = axis.y * axis.x * t + axis.z * s;
		result(1, 1) = c + axis.y * axis.y * t;
		result(2, 1) = axis.y * axis.z * t - axis.x * s;

		result(0, 2) = axis.z * axis.x * t - axis.y * s;
		result(1, 2) = axis.z * axis.y * t + axis.x * s;
		result(2, 2) = c + axis.z * axis.z * t;

		return result;
	}

	template<typename T>
	Matrix4x4<T>& Rotate(Matrix4x4<T>& result, const Vector3<T>& axis, const T& angle) {
		Matrix4x4<T> mat{ Rotate(axis, angle) };
		result = result * mat;
		return result;
	}

	template<typename T>
	Matrix4x4<T> Translate(const Vector3<T>& translation) {
		Matrix4x4<T> result{ Vin::Matrix4x4<T>::identity };

		result(0, 3) = translation.x;
		result(1, 3) = translation.y;
		result(2, 3) = translation.z;

		return result;
	}

	template<typename T>
	Matrix4x4<T>& Translate(Matrix4x4<T>& result, const Vector3<T>& translation) {
		result(0, 3) += translation.x;
		result(1, 3) += translation.y;
		result(2, 3) += translation.z;

		return result;
	}

	template<typename T>
	Matrix4x4<T> Orthographic(T left, T right, T top, T bottom, T near, T far) {
		Matrix4x4<T> result{ Matrix4x4<T>::identity };

		result(0, 0) = (T)2 / (right - left);
		result(1, 1) = (T)2 / (top - bottom);
		result(2, 2) = (T)-2 / (far - near);

		result(0, 3) = -(right + left) / (right - left);
		result(1, 3) = -(top + bottom) / (top - bottom);
		result(2, 3) = -(far + near) / (far - near);

		return result;
	}

	template<typename T>
	Matrix4x4<T> Perspective(T fov, T aspect, T near, T far) {
		Matrix4x4<T> result{ Matrix4x4<T>::identity };

		T top = near * Tan(fov / (T)2);
		T bottom = -top;
		T right = top * aspect;
		T left = -top * aspect;

		result(0, 0) = ((T)2 * near) / (right - left);
		result(1, 1) = ((T)2 * near) / (top - bottom);

		//result(0, 2) = (right - left) / (right - left);
		//result(1, 2) = (top + bottom) / (top - bottom);

		result(2, 2) = -(far + near) / (far - near);
		result(3, 2) = (T)-1;

		result(2, 3) = -((T)2 * near * far) / (far - near);
		result(3, 3) = (T)0;

		return result;
	}
}