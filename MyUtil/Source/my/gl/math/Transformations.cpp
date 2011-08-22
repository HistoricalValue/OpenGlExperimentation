#include <stdafx.h>

///////////////////////////////////////////////////////////

namespace my { namespace gl { namespace math {
namespace Transformations {

///////////////////////////////////////////////////////////

mat4 const Rotate (vec4 const& axis, float const rad) {
	float const	s	= sin(rad);
	float const	c	= cos(rad);
	float const	cc	= 1.0f - c;
	vec4 const	d	= axis.normalised();

	float const d_x	= d.x();
	float const d_y	= d.y();
	float const d_z	= d.z();

	return 	mat4(
				vec4::New(cc*d_x*d_x + c		, cc*d_y*d_x - s*d_z	, cc*d_z*d_x + s*d_y	,	0),
				vec4::New(cc*d_x*d_y + s*d_z	, cc*d_y*d_y + c		, cc*d_z*d_y - s*d_z	,	0),
				vec4::New(cc*d_x*d_z - s*d_y	, cc*d_y*d_z + s*d_x	, cc*d_z*d_z + c		,	0),
				vec4::New(0						, 0						, 0						,	1)
			)
	;
}

///////////////////////////////////////////////////////////

}	// Transformations
}}}	// my::gl::math

///////////////////////////////////////////////////////////
