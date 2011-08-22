#include <stdafx.h>

namespace my { namespace gl { namespace math {

	Matrix4x4::Matrix4x4 (float const v):
		a11(v),
		a12(0),
		a13(0),
		a14(0),
		a21(0),
		a22(v),
		a23(0),
		a24(0),
		a31(0),
		a32(0),
		a33(v),
		a34(0),
		a41(0),
		a42(0),
		a43(0),
		a44(v)
		{ }


	Matrix4x4::Matrix4x4 (
			float const _a11, float const _a12, float const _a13, float const _a14,
			float const _a21, float const _a22, float const _a23, float const _a24,
			float const _a31, float const _a32, float const _a33, float const _a34,
			float const _a41, float const _a42, float const _a43, float const _a44
			):
		a11(_a11),
		a12(_a12),
		a13(_a13),
		a14(_a14),
		a21(_a21),
		a22(_a22),
		a23(_a23),
		a24(_a24),
		a31(_a31),
		a32(_a32),
		a33(_a33),
		a34(_a34),
		a41(_a41),
		a42(_a42),
		a43(_a43),
		a44(_a44)
		{ }


//	Matrix4x4::Matrix4x4 (Matrix4x4 const& m):
//		a11(m.a11),
//		a12(m.a12),
//		a13(m.a13),
//		a14(m.a14),
//		a21(m.a21),
//		a22(m.a22),
//		a23(m.a23),
//		a24(m.a24),
//		a31(m.a31),
//		a32(m.a32),
//		a33(m.a33),
//		a34(m.a34),
//		a41(m.a41),
//		a42(m.a42),
//		a43(m.a43),
//		a44(m.a44)
//		{ }


//	Matrix4x4 Matrix4x4::operator * (Matrix4x4 const& m) const {
//		return Matrix4x4(
//			a11*m.a11 + a12*m.a21 + a13*m.a31 + a14*m.a41, a11*m.a12 + a12*m.a22 + a13*m.a32 + a14*m.a42, a11*m.a13 + a12*m.a23 + a13*m.a33 + a14*m.a43, a11*m.a14 + a12*m.a24 + a13*m.a34 + a14*m.a44,
//			a21*m.a11 + a22*m.a21 + a23*m.a31 + a24*m.a41, a21*m.a12 + a22*m.a22 + a23*m.a32 + a24*m.a42, a11*m.a23 + a22*m.a23 + a23*m.a33 + a24*m.a43, a21*m.a14 + a22*m.a24 + a23*m.a34 + a24*m.a44,
//			a31*m.a11 + a32*m.a21 + a33*m.a31 + a34*m.a41, a31*m.a12 + a32*m.a22 + a33*m.a32 + a34*m.a42, a11*m.a33 + a32*m.a23 + a33*m.a33 + a34*m.a43, a31*m.a14 + a32*m.a24 + a33*m.a34 + a34*m.a44,
//			a41*m.a11 + a42*m.a21 + a43*m.a31 + a44*m.a41, a41*m.a12 + a42*m.a22 + a43*m.a32 + a44*m.a42, a11*m.a43 + a42*m.a23 + a43*m.a33 + a44*m.a43, a41*m.a14 + a42*m.a24 + a43*m.a34 + a44*m.a44
//			);
//	}

//	Matrix4x4 Matrix4x4::operator * (float const f) const {
//		return Matrix4x4(
//			a11 * f, a12 * f, a13 * f, a14 * f,
//			a21 * f, a22 * f, a23 * f, a24 * f,
//			a31 * f, a32 * f, a33 * f, a34 * f,
//			a41 * f, a42 * f, a43 * f, a44 * f
//			);
//	}

	Vector4 Matrix4x4::operator * (Vector4 const& vec4) const {
		float const v0(vec4[0]);
		float const v1(vec4[1]);
		float const v2(vec4[2]);
		float const v3(vec4[3]);
		return Vector4::New(
			a11*v0 + a12*v1 + a13*v2 + a14*v3,
			a21*v0 + a22*v1 + a23*v2 + a24*v3,
			a31*v0 + a32*v1 + a33*v2 + a34*v3,
			a41*v0 + a42*v1 + a43*v2 + a44*v3
			);
	}
//
//	void Matrix4x4::mul (float const with[4], float result[4]) const {
//		float const w1 = with[0];
//		float const w2 = with[1];
//		float const w3 = with[2];
//		float const w4 = with[3];
//
//		result[0] = a11*w1 + a12*w2 + a13*w3 + a14*w4;
//		result[1] = a21*w1 + a22*w2 + a23*w3 + a24*w4;
//		result[2] = a31*w1 + a32*w2 + a33*w3 + a34*w4;
//		result[3] = a41*w1 + a42*w2 + a43*w3 + a44*w4;
//	}

	float const* Matrix4x4::as_float_array_16 (void) const {
		P_STATIC_ASSERT(offsetof(Matrix4x4, a11) == sizeof(float) *  0);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a12) == sizeof(float) *  1);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a13) == sizeof(float) *  2);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a14) == sizeof(float) *  3);
		//
		P_STATIC_ASSERT(offsetof(Matrix4x4, a21) == sizeof(float) *  4);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a22) == sizeof(float) *  5);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a23) == sizeof(float) *  6);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a24) == sizeof(float) *  7);
		//
		P_STATIC_ASSERT(offsetof(Matrix4x4, a31) == sizeof(float) *  8);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a32) == sizeof(float) *  9);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a33) == sizeof(float) * 10);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a34) == sizeof(float) * 11);
		//
		P_STATIC_ASSERT(offsetof(Matrix4x4, a41) == sizeof(float) * 12);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a42) == sizeof(float) * 13);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a43) == sizeof(float) * 14);
		P_STATIC_ASSERT(offsetof(Matrix4x4, a44) == sizeof(float) * 15);
		//
		typedef void const* voidp;
		PASSERT(voidp(this) == voidp(&a11))
		
		return reinterpret_cast<float const* const>(this);
	}


}}} // namespace my::gl::math
