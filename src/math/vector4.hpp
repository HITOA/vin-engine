#pragma once

#include "vector_base.hpp"

namespace Vin {
	template<typename T>
	struct Vector<T, 4> {
		using type = Vector4<T>;

		union {
			T data[4];

			SwizzleProxy1<T, 4, 0> x;
			SwizzleProxy1<T, 4, 1> y;
			SwizzleProxy1<T, 4, 2> z;
			SwizzleProxy1<T, 4, 3> w;
			SwizzleProxy2<T, 4, 0, 0> xx;
			SwizzleProxy2<T, 4, 1, 0> yx;
			SwizzleProxy2<T, 4, 2, 0> zx;
			SwizzleProxy2<T, 4, 3, 0> wx;
			SwizzleProxy2<T, 4, 0, 1> xy;
			SwizzleProxy2<T, 4, 1, 1> yy;
			SwizzleProxy2<T, 4, 2, 1> zy;
			SwizzleProxy2<T, 4, 3, 1> wy;
			SwizzleProxy2<T, 4, 0, 2> xz;
			SwizzleProxy2<T, 4, 1, 2> yz;
			SwizzleProxy2<T, 4, 2, 2> zz;
			SwizzleProxy2<T, 4, 3, 2> wz;
			SwizzleProxy2<T, 4, 0, 3> xw;
			SwizzleProxy2<T, 4, 1, 3> yw;
			SwizzleProxy2<T, 4, 2, 3> zw;
			SwizzleProxy2<T, 4, 3, 3> ww;
			SwizzleProxy3<T, 4, 0, 0, 0> xxx;
			SwizzleProxy3<T, 4, 1, 0, 0> yxx;
			SwizzleProxy3<T, 4, 2, 0, 0> zxx;
			SwizzleProxy3<T, 4, 3, 0, 0> wxx;
			SwizzleProxy3<T, 4, 0, 1, 0> xyx;
			SwizzleProxy3<T, 4, 1, 1, 0> yyx;
			SwizzleProxy3<T, 4, 2, 1, 0> zyx;
			SwizzleProxy3<T, 4, 3, 1, 0> wyx;
			SwizzleProxy3<T, 4, 0, 2, 0> xzx;
			SwizzleProxy3<T, 4, 1, 2, 0> yzx;
			SwizzleProxy3<T, 4, 2, 2, 0> zzx;
			SwizzleProxy3<T, 4, 3, 2, 0> wzx;
			SwizzleProxy3<T, 4, 0, 3, 0> xwx;
			SwizzleProxy3<T, 4, 1, 3, 0> ywx;
			SwizzleProxy3<T, 4, 2, 3, 0> zwx;
			SwizzleProxy3<T, 4, 3, 3, 0> wwx;
			SwizzleProxy3<T, 4, 0, 0, 1> xxy;
			SwizzleProxy3<T, 4, 1, 0, 1> yxy;
			SwizzleProxy3<T, 4, 2, 0, 1> zxy;
			SwizzleProxy3<T, 4, 3, 0, 1> wxy;
			SwizzleProxy3<T, 4, 0, 1, 1> xyy;
			SwizzleProxy3<T, 4, 1, 1, 1> yyy;
			SwizzleProxy3<T, 4, 2, 1, 1> zyy;
			SwizzleProxy3<T, 4, 3, 1, 1> wyy;
			SwizzleProxy3<T, 4, 0, 2, 1> xzy;
			SwizzleProxy3<T, 4, 1, 2, 1> yzy;
			SwizzleProxy3<T, 4, 2, 2, 1> zzy;
			SwizzleProxy3<T, 4, 3, 2, 1> wzy;
			SwizzleProxy3<T, 4, 0, 3, 1> xwy;
			SwizzleProxy3<T, 4, 1, 3, 1> ywy;
			SwizzleProxy3<T, 4, 2, 3, 1> zwy;
			SwizzleProxy3<T, 4, 3, 3, 1> wwy;
			SwizzleProxy3<T, 4, 0, 0, 2> xxz;
			SwizzleProxy3<T, 4, 1, 0, 2> yxz;
			SwizzleProxy3<T, 4, 2, 0, 2> zxz;
			SwizzleProxy3<T, 4, 3, 0, 2> wxz;
			SwizzleProxy3<T, 4, 0, 1, 2> xyz;
			SwizzleProxy3<T, 4, 1, 1, 2> yyz;
			SwizzleProxy3<T, 4, 2, 1, 2> zyz;
			SwizzleProxy3<T, 4, 3, 1, 2> wyz;
			SwizzleProxy3<T, 4, 0, 2, 2> xzz;
			SwizzleProxy3<T, 4, 1, 2, 2> yzz;
			SwizzleProxy3<T, 4, 2, 2, 2> zzz;
			SwizzleProxy3<T, 4, 3, 2, 2> wzz;
			SwizzleProxy3<T, 4, 0, 3, 2> xwz;
			SwizzleProxy3<T, 4, 1, 3, 2> ywz;
			SwizzleProxy3<T, 4, 2, 3, 2> zwz;
			SwizzleProxy3<T, 4, 3, 3, 2> wwz;
			SwizzleProxy3<T, 4, 0, 0, 3> xxw;
			SwizzleProxy3<T, 4, 1, 0, 3> yxw;
			SwizzleProxy3<T, 4, 2, 0, 3> zxw;
			SwizzleProxy3<T, 4, 3, 0, 3> wxw;
			SwizzleProxy3<T, 4, 0, 1, 3> xyw;
			SwizzleProxy3<T, 4, 1, 1, 3> yyw;
			SwizzleProxy3<T, 4, 2, 1, 3> zyw;
			SwizzleProxy3<T, 4, 3, 1, 3> wyw;
			SwizzleProxy3<T, 4, 0, 2, 3> xzw;
			SwizzleProxy3<T, 4, 1, 2, 3> yzw;
			SwizzleProxy3<T, 4, 2, 2, 3> zzw;
			SwizzleProxy3<T, 4, 3, 2, 3> wzw;
			SwizzleProxy3<T, 4, 0, 3, 3> xww;
			SwizzleProxy3<T, 4, 1, 3, 3> yww;
			SwizzleProxy3<T, 4, 2, 3, 3> zww;
			SwizzleProxy3<T, 4, 3, 3, 3> www;
			SwizzleProxy4<T, 4, 0, 0, 0, 0> xxxx;
			SwizzleProxy4<T, 4, 1, 0, 0, 0> yxxx;
			SwizzleProxy4<T, 4, 2, 0, 0, 0> zxxx;
			SwizzleProxy4<T, 4, 3, 0, 0, 0> wxxx;
			SwizzleProxy4<T, 4, 0, 1, 0, 0> xyxx;
			SwizzleProxy4<T, 4, 1, 1, 0, 0> yyxx;
			SwizzleProxy4<T, 4, 2, 1, 0, 0> zyxx;
			SwizzleProxy4<T, 4, 3, 1, 0, 0> wyxx;
			SwizzleProxy4<T, 4, 0, 2, 0, 0> xzxx;
			SwizzleProxy4<T, 4, 1, 2, 0, 0> yzxx;
			SwizzleProxy4<T, 4, 2, 2, 0, 0> zzxx;
			SwizzleProxy4<T, 4, 3, 2, 0, 0> wzxx;
			SwizzleProxy4<T, 4, 0, 3, 0, 0> xwxx;
			SwizzleProxy4<T, 4, 1, 3, 0, 0> ywxx;
			SwizzleProxy4<T, 4, 2, 3, 0, 0> zwxx;
			SwizzleProxy4<T, 4, 3, 3, 0, 0> wwxx;
			SwizzleProxy4<T, 4, 0, 0, 1, 0> xxyx;
			SwizzleProxy4<T, 4, 1, 0, 1, 0> yxyx;
			SwizzleProxy4<T, 4, 2, 0, 1, 0> zxyx;
			SwizzleProxy4<T, 4, 3, 0, 1, 0> wxyx;
			SwizzleProxy4<T, 4, 0, 1, 1, 0> xyyx;
			SwizzleProxy4<T, 4, 1, 1, 1, 0> yyyx;
			SwizzleProxy4<T, 4, 2, 1, 1, 0> zyyx;
			SwizzleProxy4<T, 4, 3, 1, 1, 0> wyyx;
			SwizzleProxy4<T, 4, 0, 2, 1, 0> xzyx;
			SwizzleProxy4<T, 4, 1, 2, 1, 0> yzyx;
			SwizzleProxy4<T, 4, 2, 2, 1, 0> zzyx;
			SwizzleProxy4<T, 4, 3, 2, 1, 0> wzyx;
			SwizzleProxy4<T, 4, 0, 3, 1, 0> xwyx;
			SwizzleProxy4<T, 4, 1, 3, 1, 0> ywyx;
			SwizzleProxy4<T, 4, 2, 3, 1, 0> zwyx;
			SwizzleProxy4<T, 4, 3, 3, 1, 0> wwyx;
			SwizzleProxy4<T, 4, 0, 0, 2, 0> xxzx;
			SwizzleProxy4<T, 4, 1, 0, 2, 0> yxzx;
			SwizzleProxy4<T, 4, 2, 0, 2, 0> zxzx;
			SwizzleProxy4<T, 4, 3, 0, 2, 0> wxzx;
			SwizzleProxy4<T, 4, 0, 1, 2, 0> xyzx;
			SwizzleProxy4<T, 4, 1, 1, 2, 0> yyzx;
			SwizzleProxy4<T, 4, 2, 1, 2, 0> zyzx;
			SwizzleProxy4<T, 4, 3, 1, 2, 0> wyzx;
			SwizzleProxy4<T, 4, 0, 2, 2, 0> xzzx;
			SwizzleProxy4<T, 4, 1, 2, 2, 0> yzzx;
			SwizzleProxy4<T, 4, 2, 2, 2, 0> zzzx;
			SwizzleProxy4<T, 4, 3, 2, 2, 0> wzzx;
			SwizzleProxy4<T, 4, 0, 3, 2, 0> xwzx;
			SwizzleProxy4<T, 4, 1, 3, 2, 0> ywzx;
			SwizzleProxy4<T, 4, 2, 3, 2, 0> zwzx;
			SwizzleProxy4<T, 4, 3, 3, 2, 0> wwzx;
			SwizzleProxy4<T, 4, 0, 0, 3, 0> xxwx;
			SwizzleProxy4<T, 4, 1, 0, 3, 0> yxwx;
			SwizzleProxy4<T, 4, 2, 0, 3, 0> zxwx;
			SwizzleProxy4<T, 4, 3, 0, 3, 0> wxwx;
			SwizzleProxy4<T, 4, 0, 1, 3, 0> xywx;
			SwizzleProxy4<T, 4, 1, 1, 3, 0> yywx;
			SwizzleProxy4<T, 4, 2, 1, 3, 0> zywx;
			SwizzleProxy4<T, 4, 3, 1, 3, 0> wywx;
			SwizzleProxy4<T, 4, 0, 2, 3, 0> xzwx;
			SwizzleProxy4<T, 4, 1, 2, 3, 0> yzwx;
			SwizzleProxy4<T, 4, 2, 2, 3, 0> zzwx;
			SwizzleProxy4<T, 4, 3, 2, 3, 0> wzwx;
			SwizzleProxy4<T, 4, 0, 3, 3, 0> xwwx;
			SwizzleProxy4<T, 4, 1, 3, 3, 0> ywwx;
			SwizzleProxy4<T, 4, 2, 3, 3, 0> zwwx;
			SwizzleProxy4<T, 4, 3, 3, 3, 0> wwwx;
			SwizzleProxy4<T, 4, 0, 0, 0, 1> xxxy;
			SwizzleProxy4<T, 4, 1, 0, 0, 1> yxxy;
			SwizzleProxy4<T, 4, 2, 0, 0, 1> zxxy;
			SwizzleProxy4<T, 4, 3, 0, 0, 1> wxxy;
			SwizzleProxy4<T, 4, 0, 1, 0, 1> xyxy;
			SwizzleProxy4<T, 4, 1, 1, 0, 1> yyxy;
			SwizzleProxy4<T, 4, 2, 1, 0, 1> zyxy;
			SwizzleProxy4<T, 4, 3, 1, 0, 1> wyxy;
			SwizzleProxy4<T, 4, 0, 2, 0, 1> xzxy;
			SwizzleProxy4<T, 4, 1, 2, 0, 1> yzxy;
			SwizzleProxy4<T, 4, 2, 2, 0, 1> zzxy;
			SwizzleProxy4<T, 4, 3, 2, 0, 1> wzxy;
			SwizzleProxy4<T, 4, 0, 3, 0, 1> xwxy;
			SwizzleProxy4<T, 4, 1, 3, 0, 1> ywxy;
			SwizzleProxy4<T, 4, 2, 3, 0, 1> zwxy;
			SwizzleProxy4<T, 4, 3, 3, 0, 1> wwxy;
			SwizzleProxy4<T, 4, 0, 0, 1, 1> xxyy;
			SwizzleProxy4<T, 4, 1, 0, 1, 1> yxyy;
			SwizzleProxy4<T, 4, 2, 0, 1, 1> zxyy;
			SwizzleProxy4<T, 4, 3, 0, 1, 1> wxyy;
			SwizzleProxy4<T, 4, 0, 1, 1, 1> xyyy;
			SwizzleProxy4<T, 4, 1, 1, 1, 1> yyyy;
			SwizzleProxy4<T, 4, 2, 1, 1, 1> zyyy;
			SwizzleProxy4<T, 4, 3, 1, 1, 1> wyyy;
			SwizzleProxy4<T, 4, 0, 2, 1, 1> xzyy;
			SwizzleProxy4<T, 4, 1, 2, 1, 1> yzyy;
			SwizzleProxy4<T, 4, 2, 2, 1, 1> zzyy;
			SwizzleProxy4<T, 4, 3, 2, 1, 1> wzyy;
			SwizzleProxy4<T, 4, 0, 3, 1, 1> xwyy;
			SwizzleProxy4<T, 4, 1, 3, 1, 1> ywyy;
			SwizzleProxy4<T, 4, 2, 3, 1, 1> zwyy;
			SwizzleProxy4<T, 4, 3, 3, 1, 1> wwyy;
			SwizzleProxy4<T, 4, 0, 0, 2, 1> xxzy;
			SwizzleProxy4<T, 4, 1, 0, 2, 1> yxzy;
			SwizzleProxy4<T, 4, 2, 0, 2, 1> zxzy;
			SwizzleProxy4<T, 4, 3, 0, 2, 1> wxzy;
			SwizzleProxy4<T, 4, 0, 1, 2, 1> xyzy;
			SwizzleProxy4<T, 4, 1, 1, 2, 1> yyzy;
			SwizzleProxy4<T, 4, 2, 1, 2, 1> zyzy;
			SwizzleProxy4<T, 4, 3, 1, 2, 1> wyzy;
			SwizzleProxy4<T, 4, 0, 2, 2, 1> xzzy;
			SwizzleProxy4<T, 4, 1, 2, 2, 1> yzzy;
			SwizzleProxy4<T, 4, 2, 2, 2, 1> zzzy;
			SwizzleProxy4<T, 4, 3, 2, 2, 1> wzzy;
			SwizzleProxy4<T, 4, 0, 3, 2, 1> xwzy;
			SwizzleProxy4<T, 4, 1, 3, 2, 1> ywzy;
			SwizzleProxy4<T, 4, 2, 3, 2, 1> zwzy;
			SwizzleProxy4<T, 4, 3, 3, 2, 1> wwzy;
			SwizzleProxy4<T, 4, 0, 0, 3, 1> xxwy;
			SwizzleProxy4<T, 4, 1, 0, 3, 1> yxwy;
			SwizzleProxy4<T, 4, 2, 0, 3, 1> zxwy;
			SwizzleProxy4<T, 4, 3, 0, 3, 1> wxwy;
			SwizzleProxy4<T, 4, 0, 1, 3, 1> xywy;
			SwizzleProxy4<T, 4, 1, 1, 3, 1> yywy;
			SwizzleProxy4<T, 4, 2, 1, 3, 1> zywy;
			SwizzleProxy4<T, 4, 3, 1, 3, 1> wywy;
			SwizzleProxy4<T, 4, 0, 2, 3, 1> xzwy;
			SwizzleProxy4<T, 4, 1, 2, 3, 1> yzwy;
			SwizzleProxy4<T, 4, 2, 2, 3, 1> zzwy;
			SwizzleProxy4<T, 4, 3, 2, 3, 1> wzwy;
			SwizzleProxy4<T, 4, 0, 3, 3, 1> xwwy;
			SwizzleProxy4<T, 4, 1, 3, 3, 1> ywwy;
			SwizzleProxy4<T, 4, 2, 3, 3, 1> zwwy;
			SwizzleProxy4<T, 4, 3, 3, 3, 1> wwwy;
			SwizzleProxy4<T, 4, 0, 0, 0, 2> xxxz;
			SwizzleProxy4<T, 4, 1, 0, 0, 2> yxxz;
			SwizzleProxy4<T, 4, 2, 0, 0, 2> zxxz;
			SwizzleProxy4<T, 4, 3, 0, 0, 2> wxxz;
			SwizzleProxy4<T, 4, 0, 1, 0, 2> xyxz;
			SwizzleProxy4<T, 4, 1, 1, 0, 2> yyxz;
			SwizzleProxy4<T, 4, 2, 1, 0, 2> zyxz;
			SwizzleProxy4<T, 4, 3, 1, 0, 2> wyxz;
			SwizzleProxy4<T, 4, 0, 2, 0, 2> xzxz;
			SwizzleProxy4<T, 4, 1, 2, 0, 2> yzxz;
			SwizzleProxy4<T, 4, 2, 2, 0, 2> zzxz;
			SwizzleProxy4<T, 4, 3, 2, 0, 2> wzxz;
			SwizzleProxy4<T, 4, 0, 3, 0, 2> xwxz;
			SwizzleProxy4<T, 4, 1, 3, 0, 2> ywxz;
			SwizzleProxy4<T, 4, 2, 3, 0, 2> zwxz;
			SwizzleProxy4<T, 4, 3, 3, 0, 2> wwxz;
			SwizzleProxy4<T, 4, 0, 0, 1, 2> xxyz;
			SwizzleProxy4<T, 4, 1, 0, 1, 2> yxyz;
			SwizzleProxy4<T, 4, 2, 0, 1, 2> zxyz;
			SwizzleProxy4<T, 4, 3, 0, 1, 2> wxyz;
			SwizzleProxy4<T, 4, 0, 1, 1, 2> xyyz;
			SwizzleProxy4<T, 4, 1, 1, 1, 2> yyyz;
			SwizzleProxy4<T, 4, 2, 1, 1, 2> zyyz;
			SwizzleProxy4<T, 4, 3, 1, 1, 2> wyyz;
			SwizzleProxy4<T, 4, 0, 2, 1, 2> xzyz;
			SwizzleProxy4<T, 4, 1, 2, 1, 2> yzyz;
			SwizzleProxy4<T, 4, 2, 2, 1, 2> zzyz;
			SwizzleProxy4<T, 4, 3, 2, 1, 2> wzyz;
			SwizzleProxy4<T, 4, 0, 3, 1, 2> xwyz;
			SwizzleProxy4<T, 4, 1, 3, 1, 2> ywyz;
			SwizzleProxy4<T, 4, 2, 3, 1, 2> zwyz;
			SwizzleProxy4<T, 4, 3, 3, 1, 2> wwyz;
			SwizzleProxy4<T, 4, 0, 0, 2, 2> xxzz;
			SwizzleProxy4<T, 4, 1, 0, 2, 2> yxzz;
			SwizzleProxy4<T, 4, 2, 0, 2, 2> zxzz;
			SwizzleProxy4<T, 4, 3, 0, 2, 2> wxzz;
			SwizzleProxy4<T, 4, 0, 1, 2, 2> xyzz;
			SwizzleProxy4<T, 4, 1, 1, 2, 2> yyzz;
			SwizzleProxy4<T, 4, 2, 1, 2, 2> zyzz;
			SwizzleProxy4<T, 4, 3, 1, 2, 2> wyzz;
			SwizzleProxy4<T, 4, 0, 2, 2, 2> xzzz;
			SwizzleProxy4<T, 4, 1, 2, 2, 2> yzzz;
			SwizzleProxy4<T, 4, 2, 2, 2, 2> zzzz;
			SwizzleProxy4<T, 4, 3, 2, 2, 2> wzzz;
			SwizzleProxy4<T, 4, 0, 3, 2, 2> xwzz;
			SwizzleProxy4<T, 4, 1, 3, 2, 2> ywzz;
			SwizzleProxy4<T, 4, 2, 3, 2, 2> zwzz;
			SwizzleProxy4<T, 4, 3, 3, 2, 2> wwzz;
			SwizzleProxy4<T, 4, 0, 0, 3, 2> xxwz;
			SwizzleProxy4<T, 4, 1, 0, 3, 2> yxwz;
			SwizzleProxy4<T, 4, 2, 0, 3, 2> zxwz;
			SwizzleProxy4<T, 4, 3, 0, 3, 2> wxwz;
			SwizzleProxy4<T, 4, 0, 1, 3, 2> xywz;
			SwizzleProxy4<T, 4, 1, 1, 3, 2> yywz;
			SwizzleProxy4<T, 4, 2, 1, 3, 2> zywz;
			SwizzleProxy4<T, 4, 3, 1, 3, 2> wywz;
			SwizzleProxy4<T, 4, 0, 2, 3, 2> xzwz;
			SwizzleProxy4<T, 4, 1, 2, 3, 2> yzwz;
			SwizzleProxy4<T, 4, 2, 2, 3, 2> zzwz;
			SwizzleProxy4<T, 4, 3, 2, 3, 2> wzwz;
			SwizzleProxy4<T, 4, 0, 3, 3, 2> xwwz;
			SwizzleProxy4<T, 4, 1, 3, 3, 2> ywwz;
			SwizzleProxy4<T, 4, 2, 3, 3, 2> zwwz;
			SwizzleProxy4<T, 4, 3, 3, 3, 2> wwwz;
			SwizzleProxy4<T, 4, 0, 0, 0, 3> xxxw;
			SwizzleProxy4<T, 4, 1, 0, 0, 3> yxxw;
			SwizzleProxy4<T, 4, 2, 0, 0, 3> zxxw;
			SwizzleProxy4<T, 4, 3, 0, 0, 3> wxxw;
			SwizzleProxy4<T, 4, 0, 1, 0, 3> xyxw;
			SwizzleProxy4<T, 4, 1, 1, 0, 3> yyxw;
			SwizzleProxy4<T, 4, 2, 1, 0, 3> zyxw;
			SwizzleProxy4<T, 4, 3, 1, 0, 3> wyxw;
			SwizzleProxy4<T, 4, 0, 2, 0, 3> xzxw;
			SwizzleProxy4<T, 4, 1, 2, 0, 3> yzxw;
			SwizzleProxy4<T, 4, 2, 2, 0, 3> zzxw;
			SwizzleProxy4<T, 4, 3, 2, 0, 3> wzxw;
			SwizzleProxy4<T, 4, 0, 3, 0, 3> xwxw;
			SwizzleProxy4<T, 4, 1, 3, 0, 3> ywxw;
			SwizzleProxy4<T, 4, 2, 3, 0, 3> zwxw;
			SwizzleProxy4<T, 4, 3, 3, 0, 3> wwxw;
			SwizzleProxy4<T, 4, 0, 0, 1, 3> xxyw;
			SwizzleProxy4<T, 4, 1, 0, 1, 3> yxyw;
			SwizzleProxy4<T, 4, 2, 0, 1, 3> zxyw;
			SwizzleProxy4<T, 4, 3, 0, 1, 3> wxyw;
			SwizzleProxy4<T, 4, 0, 1, 1, 3> xyyw;
			SwizzleProxy4<T, 4, 1, 1, 1, 3> yyyw;
			SwizzleProxy4<T, 4, 2, 1, 1, 3> zyyw;
			SwizzleProxy4<T, 4, 3, 1, 1, 3> wyyw;
			SwizzleProxy4<T, 4, 0, 2, 1, 3> xzyw;
			SwizzleProxy4<T, 4, 1, 2, 1, 3> yzyw;
			SwizzleProxy4<T, 4, 2, 2, 1, 3> zzyw;
			SwizzleProxy4<T, 4, 3, 2, 1, 3> wzyw;
			SwizzleProxy4<T, 4, 0, 3, 1, 3> xwyw;
			SwizzleProxy4<T, 4, 1, 3, 1, 3> ywyw;
			SwizzleProxy4<T, 4, 2, 3, 1, 3> zwyw;
			SwizzleProxy4<T, 4, 3, 3, 1, 3> wwyw;
			SwizzleProxy4<T, 4, 0, 0, 2, 3> xxzw;
			SwizzleProxy4<T, 4, 1, 0, 2, 3> yxzw;
			SwizzleProxy4<T, 4, 2, 0, 2, 3> zxzw;
			SwizzleProxy4<T, 4, 3, 0, 2, 3> wxzw;
			SwizzleProxy4<T, 4, 0, 1, 2, 3> xyzw;
			SwizzleProxy4<T, 4, 1, 1, 2, 3> yyzw;
			SwizzleProxy4<T, 4, 2, 1, 2, 3> zyzw;
			SwizzleProxy4<T, 4, 3, 1, 2, 3> wyzw;
			SwizzleProxy4<T, 4, 0, 2, 2, 3> xzzw;
			SwizzleProxy4<T, 4, 1, 2, 2, 3> yzzw;
			SwizzleProxy4<T, 4, 2, 2, 2, 3> zzzw;
			SwizzleProxy4<T, 4, 3, 2, 2, 3> wzzw;
			SwizzleProxy4<T, 4, 0, 3, 2, 3> xwzw;
			SwizzleProxy4<T, 4, 1, 3, 2, 3> ywzw;
			SwizzleProxy4<T, 4, 2, 3, 2, 3> zwzw;
			SwizzleProxy4<T, 4, 3, 3, 2, 3> wwzw;
			SwizzleProxy4<T, 4, 0, 0, 3, 3> xxww;
			SwizzleProxy4<T, 4, 1, 0, 3, 3> yxww;
			SwizzleProxy4<T, 4, 2, 0, 3, 3> zxww;
			SwizzleProxy4<T, 4, 3, 0, 3, 3> wxww;
			SwizzleProxy4<T, 4, 0, 1, 3, 3> xyww;
			SwizzleProxy4<T, 4, 1, 1, 3, 3> yyww;
			SwizzleProxy4<T, 4, 2, 1, 3, 3> zyww;
			SwizzleProxy4<T, 4, 3, 1, 3, 3> wyww;
			SwizzleProxy4<T, 4, 0, 2, 3, 3> xzww;
			SwizzleProxy4<T, 4, 1, 2, 3, 3> yzww;
			SwizzleProxy4<T, 4, 2, 2, 3, 3> zzww;
			SwizzleProxy4<T, 4, 3, 2, 3, 3> wzww;
			SwizzleProxy4<T, 4, 0, 3, 3, 3> xwww;
			SwizzleProxy4<T, 4, 1, 3, 3, 3> ywww;
			SwizzleProxy4<T, 4, 2, 3, 3, 3> zwww;
			SwizzleProxy4<T, 4, 3, 3, 3, 3> wwww;

		};

		Vector() : data{ T{}, T{}, T{}, T{} } {};
		Vector(const T& v) : data{ v, v, v, v } {};
		Vector(const T& x, const T& y, const T& z, const T& w) : data{ x, y, z, w } {};
	};

	template<typename T>
	struct Vector4 : Vector<T, 4>, VectorMath<Vector4<T>, 4> {
		Vector4() : Vector<T, 4>{} {};
		Vector4(const T& v) : Vector<T, 4>{ v } {};
		Vector4(const T& x, const T& y, const T& z, const T& w) : Vector<T, 4>{ x, y, z, w } {};
		Vector4(const Vector2<T>& xy, T z, T w) : Vector<T, 4>{ xy.x, xy.y, z, w } {};
		Vector4(const Vector3<T>& xyz, T w) : Vector<T, 4>{ xyz.x, xyz.y, xyz.z, w } {};

		T Length() const {
			T length = Sqrt<T>(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
			return length;
		}

		//Assignment

		Vector4<T>& operator=(const Vector4<T>& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->w = rhs.w;
			return *this;
		}

		Vector4<T>& operator+=(const Vector4<T>& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;
			return *this;
		}

		Vector4<T>& operator-=(const Vector4<T>& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;
			return *this;
		}

		Vector4<T>& operator*=(const Vector4<T>& rhs) {
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			this->w *= rhs.w;
			return *this;
		}

		Vector4<T>& operator/=(const Vector4<T>& rhs) {
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			this->w /= rhs.w;
			return *this;
		}

		//Arithmetic

		friend Vector4<T> operator+(
			Vector4<T> lhs, const Vector4<T>& rhs) {
			lhs += rhs;
			return lhs;
		}

		friend Vector4<T> operator-(
			Vector4<T> lhs, const Vector4<T>& rhs) {
			lhs -= rhs;
			return lhs;
		}

		friend Vector4<T> operator*(
			Vector4<T> lhs, const Vector4<T>& rhs) {
			lhs *= rhs;
			return lhs;
		}

		friend Vector4<T> operator/(
			Vector4<T> lhs, const Vector4<T>& rhs) {
			lhs /= rhs;
			return lhs;
		}

		Vector4<T> operator-() const {
			Vector4<T> v{ *this };
			v *= -1;
			return v;
		}
	};
}
