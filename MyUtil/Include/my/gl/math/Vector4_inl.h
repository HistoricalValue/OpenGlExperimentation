#ifndef __MY_UTIL__MY__GL__MATH__VECTOR_4_INL__H__
#define __MY_UTIL__MY__GL__MATH__VECTOR_4_INL__H__

#include <my/gl/math/Vector4.h>

namespace my { namespace gl { namespace math {

	///////////////////////////////////

	inline
	Vector4 const Vector4::cross_asvec3 (Vector4 const& other) const {
		float const	u1	(v[0]/v[3]),
					u2	(v[1]/v[3]),
					u3	(v[2]/v[3]),
					v1	(other.v[0]/other.v[3]),
					v2	(other.v[1]/other.v[3]),
					v3	(other.v[2]/other.v[3]);
		return	Vector4(
					u2 * v3 - u3 * v2,
					u3 * v1 - u1 * v3,
					u1 * v2 - u2 * v1,
					1.0f
				);
	}

	///////////////////////////////////

	template <typename T>
	Vector4 Vector4::operator / (T const d) const {
		return Vector4(x()/d, y()/d, z()/d, w()/d);
	}

	///////////////////////////////////

	template <typename T>
	Vector4 Vector4::operator + (T const d) const {
		return Vector4(x()+d, y()+d, z()+d, w()+d);
	}

	///////////////////////////////////

	template <typename T>
	void Vector4::addtothis (T const d) {
		v[0] += d;
		v[1] += d;
		v[2] += d;
	}

	///////////////////////////////////

	template <typename T>
	Vector4 Vector4::add (T const d) const {
		return Vector4(x()+d, y()+d, z()+d, w());
	}

	///////////////////////////////////

}}} // namespace my::gl::math

#endif // __MY_UTIL__MY__GL__SHAPES__VECTOR_4_INL__H__
