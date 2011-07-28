#ifndef __MY_UTIL__MY__GL__MATH__VECTOR_4_INL__H__
#define __MY_UTIL__MY__GL__MATH__VECTOR_4_INL__H__

#include "my/gl/math/Vector4.h"

namespace my { namespace gl { namespace math {

	///////////////////////////////////

	template <typename T>
	Vector4 Vector4::operator / (T const d) const {
		return Vector4(x()/d, y()/d, z()/d, w()/d);
	}


#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__MATH__VECTOR_4_DEFINING__)

	// element-by-element multiplication
	P_INLINE
	Vector4 Vector4::mul (Vector4 const& other) const {
		return Vector4(v[0]*other.v[0], v[1]*other.v[1], v[2]*other.v[2], v[3]*other.v[3]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4 Vector4::operator + (Vector4 const& other) const {
		return Vector4(v[0]+other.v[0], v[1]+other.v[1], v[2]+other.v[2], v[3]+other.v[3]);
	}

	///////////////////////////////////

	P_INLINE
	void Vector4::operator = (Vector4 const& other) {
		new(this) Vector4(other);
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::magnitude (void) const {
		return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) / abs(v[3]);
	}

	///////////////////////////////////

	P_INLINE
	// Inner (dot) product
	float Vector4::operator * (Vector4 const& other) const {
		return v[0]*other.v[0] + v[1]*other.v[1] + v[2]*other.v[2] + v[3]*other.v[3];
	}

	///////////////////////////////////

	P_INLINE
	Vector4 Vector4::operator * (float const f) const {
		return Vector4(v[0]*f, v[1]*f, v[2]*f, v[3]*f);
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::angle (Vector4 const& other) const {
		return acos( operator *(other) / (magnitude() * other.magnitude()) );
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::r (void) const {
		return v[0];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::g (void) const {
		return v[1];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::b (void) const {
		return v[2];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::a (void) const {
		return v[3];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::x (void) const {
		return v[0];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::y (void) const {
		return v[1];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::z (void) const {
		return v[2];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::w (void) const {
		return v[3];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::s (void) const {
		return v[0];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::t (void) const {
		return v[1];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::p (void) const {
		return v[2];
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::q (void) const {
		return v[3];
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::x (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::y (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::z (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::w (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::r (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::g (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::b (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::a (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::s (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::t (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::p (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::q (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter Vector4::operator [] (size_t i) {
		bool null(i < sizeof(v)/sizeof(v[0]));
		return Setter(v[i], null);
	}

	///////////////////////////////////

	P_INLINE
	float Vector4::operator [] (size_t i) const {
		return i < sizeof(v)/sizeof(v[0])? v[i] : static_cast<float>(0xffffffff);
	}

	///////////////////////////////////

	P_INLINE
	Vector4::Setter::Setter (float& v, bool _null):
		e(v),
		null(_null)
		{ P_STATIC_ASSERT(sizeof(Setter) == 3 + sizeof(float&) + sizeof(null)) }

	///////////////////////////////////

	P_INLINE
	float Vector4::Setter::operator = (float const v) const {
		return null? static_cast<float>(0xffffffff) : e = v;
	}

	///////////////////////////////////

	P_INLINE
	Vector4 Vector4::operator - (Vector4 const& other) const {
		return Vector4(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2], v[3] - other.v[3]);

	}

	///////////////////////////////////

#endif

}}} // namespace my::gl::math

#endif // __MY_UTIL__MY__GL__SHAPES__VECTOR_4_INL__H__
