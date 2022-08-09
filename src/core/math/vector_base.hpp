#pragma once
#pragma unroll

#include "mathhelper.hpp"

namespace Vin {

	template<typename T, size_t size>
	struct Vector {};
	template<typename T>
	struct Vector2;
	template<typename T>
	struct Vector3;
	template<typename T>
	struct Vector4;

	template<typename T, size_t size>
	struct VectorMath {
		static T Add(const T& v1, const T& v2) {
			return v1 + v2;
		}

		static void Add(const T& v1, const T& v2, T& out) {
			out = v1 + v2;
		}

		//Every for loop must be unroll by the compiler

		static T Ceil(T& v) {
			T vr{};
			for (size_t i = 0; i < size; i++) {
				vr.data[i] = Vin::Ceil(v.data[i]);
			}
			return vr;
		}

		static T Floor(T& v) {
			T vr{};
			for (size_t i = 0; i < size; i++) {
				vr.data[i] = Vin::Floor(v.data[i]);
			}
			return vr;
		}

		static T Round(T& v) {
			T vr{};
			for (size_t i = 0; i < size; i++) {
				vr.data[i] = Vin::Round(v.data[i]);
			}
			return vr;
		}

		static T Clamp(T& v, T m, T M) {
			T vr{};
			for (size_t i = 0; i < size; i++) {
				vr.data[i] = Vin::Clamp(v.data[i], m.data[i], M.data[i]);
			}
			return vr;
		}

		template<typename R>
		static R Dot(Vector<R, size> v1, Vector<R, size> v2) {
			R r{};
			for (size_t i = 0; i < size; i++)
				r += v1.data[i] * v2.data[i];
			return r;
		}
	};

	template<typename T, size_t count, unsigned int i1>
	struct SwizzleProxy1 {
		T data[count];

		operator T() const {
			return data[i1];
		}

		//Assignment

		SwizzleProxy1<T, count, i1>& operator=(const T& rhs) {
			data[i1] = rhs;
			return *this;
		}

		SwizzleProxy1<T, count, i1>& operator+=(const T& rhs) {
			data[i1] += rhs;
			return *this;
		}

		SwizzleProxy1<T, count, i1>& operator-=(const T& rhs) {
			data[i1] -= rhs;
			return *this;
		}

		SwizzleProxy1<T, count, i1>& operator*=(const T& rhs) {
			data[i1] *= rhs;
			return *this;
		}

		SwizzleProxy1<T, count, i1>& operator/=(const T& rhs) {
			data[i1] /= rhs;
			return *this;
		}

		//Increment

		SwizzleProxy1<T, count, i1>& operator++() {
			data[i1]++;
			return *this;
		}

		SwizzleProxy1<T, count, i1> operator++(int) {
			SwizzleProxy1<T, count, i1> old = *this;
			operator++();
			return old;
		}

		//Decrement

		SwizzleProxy1<T, count, i1>& operator--() {
			data[i1]--;
			return *this;
		}

		SwizzleProxy1<T, count, i1> operator--(int) {
			SwizzleProxy1<T, count, i1> old = *this;
			operator--();
			return old;
		}

		//Arithmetic

		friend SwizzleProxy1<T, count, i1> operator+(
			SwizzleProxy1<T, count, i1> lhs, const T& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend SwizzleProxy1<T, count, i1> operator-(
			SwizzleProxy1<T, count, i1> lhs, const T& rhs) {
			lhs -= rhs;
			return lhs;
		}

		friend SwizzleProxy1<T, count, i1> operator*(
			SwizzleProxy1<T, count, i1> lhs, const T& rhs) {
			lhs *= rhs;
			return lhs;
		}

		friend SwizzleProxy1<T, count, i1> operator/(
			SwizzleProxy1<T, count, i1> lhs, const T& rhs) {
			lhs /= rhs;
			return lhs;
		}
	};

	template<typename T, size_t count, unsigned int i1, unsigned int i2>
	struct SwizzleProxy2 {
		T data[count];

		operator Vector2<T>() const {
			Vector2<T> vec2{};
			vec2.x = data[i1];
			vec2.y = data[i2];

			return vec2;
		}
	};

	template<typename T, size_t count, unsigned int i1, unsigned int i2, unsigned int i3>
	struct SwizzleProxy3 {
		T data[count];

		operator Vector3<T>() const {
			Vector3<T> vec3{};
			vec3.x = data[i1];
			vec3.y = data[i2];
			vec3.z = data[i3];

			return vec3;
		}
	};

	template<typename T, size_t count, unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4>
	struct SwizzleProxy4 {
		T data[count];

		operator Vector4<T>() const {
			Vector4<T> vec4{};
			vec4.x = data[i1];
			vec4.y = data[i2];
			vec4.z = data[i3];
			vec4.w = data[i4];

			return vec4;
		}
	};
}
