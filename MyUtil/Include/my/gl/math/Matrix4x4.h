#ifndef __MY_UTIL__MY__GL__MATH__MATRIX_4X4__H__
#define __MY_UTIL__MY__GL__MATH__MATRIX_4X4__H__

#include <MyUtil.h>
#include <my/gl/math/Vector4.h>

namespace my { namespace gl { namespace math {

	class MYUTIL_API Matrix4x4 {
	public:
								Matrix4x4 (float v); // Diagonal of value v
								Matrix4x4 (
										float a11, float a12, float a13, float a14,
										float a21, float a22, float a23, float a24,
										float a31, float a32, float a33, float a34,
										float a41, float a42, float a43, float a44
									);
								Matrix4x4 (Matrix4x4 const&);
								Matrix4x4 (
										Vector4 const& col1,
										Vector4 const& col2,
										Vector4 const& col3,
										Vector4 const& col4
									);

		Matrix4x4				operator * (Matrix4x4 const&) const;		// linear multiplication
		Matrix4x4				operator * (float) const;
		Vector4					operator * (Vector4 const&) const;

		void					mul (float const with[4], float result[4]) const;

		float const*			as_float_array_16 (void) const;		// [row][column]

		Matrix4x4 const&		operator = (Matrix4x4 const& other);
		inline Matrix4x4 const&	operator *= (Matrix4x4 const& other)
									{ return (*this) = (*this) * other; }

		Vector4					operator [] (size_t i) const;		// column vector
		Vector4					row (size_t i) const;				// row vector

		Matrix4x4				transpose (void) const;
	private:
		float const a11;
		float const a12;
		float const a13;
		float const a14;
		//
		float const a21;
		float const a22;
		float const a23;
		float const a24;
		//
		float const a31;
		float const a32;
		float const a33;
		float const a34;
		//
		float const a41;
		float const a42;
		float const a43;
		float const a44;

	}; // class Matrix4x4

	typedef Matrix4x4 mat4;

}}} // namespace my::gl::math


#endif // __MY_UTIL__MY__GL__MATH__MATRIX_4X4__H__

