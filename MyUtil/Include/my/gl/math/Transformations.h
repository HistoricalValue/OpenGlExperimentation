#ifndef __MY_UTIL__MY__GL__MATH__TRANSFORMATIONS_H__
#define __MY_UTIL__MY__GL__MATH__TRANSFORMATIONS_H__

#include <MyUtil.h>
#include <my/gl/math/Matrix4x4.h>

///////////////////////////////////////////////////////////

namespace my { namespace gl { namespace math {
namespace Transformations {

///////////////////////////////////////////////////////////

inline mat4 const Id (void)
	{ return mat4(1); }

///////////////////////////////////////////////////////////

inline mat4 const Affine (vec4 const& x, vec4 const& y, vec4 const& z, vec4 const& u) {
	PASSERT(x.w() == 0)
	PASSERT(y.w() == 0)
	PASSERT(z.w() == 0)
	PASSERT(u.w() == 1)
	return mat4(x, y, z, u);
}

inline mat4 const Affine (
		float const x_1, float const x_2, float const x_3,
		float const y_1, float const y_2, float const y_3,
		float const z_1, float const z_2, float const z_3,
		float const u_1, float const u_2, float const u_3) {
	return mat4(
			vec4::New(x_1, x_2, x_3, 0),
			vec4::New(y_1, y_2, y_3, 0),
			vec4::New(z_1, z_2, z_3, 0),
			vec4::New(u_1, u_2, u_3, 1));
}


///////////////////////////////////////////////////////////

inline mat4 const Transform (vec4 const& x, vec4 const& y, vec4 const& z)
	{ return Affine(x, y, z, vec4()); }

inline mat4 const Transform (
		float const x_1, float const x_2, float const x_3,
		float const y_1, float const y_2, float const y_3,
		float const z_1, float const z_2, float const z_3)
	{ return Transform(vec4::New(x_1, x_2, x_3, 0), vec4::New(y_1, y_2, y_3, 0), vec4::New(z_1, z_2, z_3, 0); }

///////////////////////////////////////////////////////////

inline mat4 const Translate (vec4 const& u)
	{ return Affine(Axis_X, Axis_Y, Axis_Z, u); }

inline mat4 const Translate (float const u_1, float const u_2, float const u_3)
	{ return Translate(vec4::New(u_1, u_2, u_3)); }

///////////////////////////////////////////////////////////

inline mat4 const TransformX (vec4 const& x)
	{ return Transform(x, Axis_Y, Axis_Z); }

inline mat4 const TransformX (float const x_1, float const x_2, float const x_3)
	{ return TransformX(vec4::New(x_1, x_2, x_3)); }

///////////////////////////////////////////////////////////

inline mat4 const ScaleX (float const f)
	{ return TransformX(f, 0, 0); }

///////////////////////////////////////////////////////////

inline mat4 const Rotate (vec4 const& axis, float const rad) {
	float const	s	= sin(rad);
	float const	c	= cos(rad);
	float const	cc	= 1.0f - c;
	vec4 const	d	= axis.normalised();
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
}		// Transformations
}}}		// my::gl::math
///////////////////////////////////////////////////////////

#endif // __MY_UTIL__MY__GL__MATH__TRANSFORMATIONS_H__
