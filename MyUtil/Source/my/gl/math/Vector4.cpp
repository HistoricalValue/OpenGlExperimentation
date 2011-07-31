#include <stdafx.h>

#define __MY_UTIL__MY__GL__MATH__VECTOR_4_DEFINING__
#include <my/gl/math/Vector4_inl.h>

namespace my { namespace gl { namespace math {

	Vector4::Vector4 (float const x, float const y, float const z, float const w):
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "v" will be default initialised
		v() {
#pragma warning( pop )
		P_STATIC_ASSERT( sizeof(Vector4) == 0
				+ sizeof(v)
			)
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}

	///////////////////////////////////

	Vector4::~Vector4 (void) {
	}

	///////////////////////////////////

	Vector4::Vector4 (Vector4 const& other):
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "v" will be default initialised
		v() {
#pragma warning( pop )
		P_STATIC_ASSERT(sizeof(Vector4) == sizeof(v))
		v[0] = other.v[0];
		v[1] = other.v[1];
		v[2] = other.v[2];
		v[3] = other.v[3];
	}

	///////////////////////////////////

	Vector4 Vector4::New (float const x, float const y, float const z, float const w) {
		return Vector4(x, y, z, w);
	}

	///////////////////////////////////

	///////////////////////////////////

	///////////////////////////////////

	///////////////////////////////////

}}} // namespace my::gl::math

