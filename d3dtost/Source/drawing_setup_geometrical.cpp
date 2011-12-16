#include "stdafx.h"
#include <drawing_setup_geometrical.h>
#include "TheCursed.h"

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

float GetRotationAngle (unsigned long int const dt_milli) {
	// times in milliseconds
	// ----
	// we want one round per second => w = rad/sec = 2pi/sec => ang = w*sec = 2pi*sec = 360*sec (%360)
	// 360 * dt * 1e-3  = 0.360 * dt
	float const result(.010f * dt_milli);
	return result - floorf(result/360.f)*360.f;
}

GLuint GetTextureZ (unsigned long int const dt_milli) {
	// we want a change every second/8, total changes = 32
	// 0~1, 1~2, 2~3, ...
	// return (dt_milli / 125) % 16;		// 16 changes over 8 seconds
	// return (dt_milli / 500) % 3;			// 3 changes over 1.5 seconds
	return 0u;// (dt_milli / 250) % 11;			// 10 changes over 2.5 seconds
}

size_t GetTextureIndex (unsigned long int const dt_milli) {
	// there are TEXTURES_NUM textures and we want to keep each one for 3 seconds
	UCOMPILECHECK(_::NUMBER_OF_TEXTURES == sizeof(_::TexturesUnits)/sizeof(_::TexturesUnits[0]))
	return TexturesUnits[1u]; // (dt_milli / ((3 * 1000) / _::TEXTURES_NUM )) % _::TEXTURES_NUM];
}

void SetupCamera (void) {
	using namespace my::gl::math::Transformations;
	using namespace my::gl::math;

	mat4 m(1);
	m *= Translate(0, 0, 2.0f);

	if (_::WITH_CAMERA) {
		m *= Rotate(Axis_X(), M_PI_4 + M_PI_8);
	}

#if WITH_INVERSE_IT == 1
	m *= Rotate(Axis_X(), M_PI);
#endif

	gl::ext::glUniformMatrix4fv(::my::OpenGL::VUL_CAMERA, 1, GL_TRUE, m.as_float_array_16());
}

void SetupFrustrum (float const n, float const f, float const l, float const r, float const b, float const t) {
	PASSERT(n > 0)
	PASSERT(f > 0)
	PASSERT(f > n)
	PASSERT(r > l)
	PASSERT(t > b)

	float const r_l		= r - l;
	float const t_b		= t - b;
	float const f_n		= f - n;
	float const _2n		= 2.0f * n;
	float const Cwn		= _2n / r_l;
	float const Dw		= (-r -l) / r_l;
	float const Chn		= _2n / t_b;
	float const Dh		= (-t -b) / t_b;
	float const A		= (f + n) / f_n;
	float const B		= (_2n * -f) / f_n;

	using my::gl::math::mat4;
	mat4 m(
			Cwn	,	0	,	Dw	,	0,
			0	,	Chn	,	Dh	,	0,
			0	,	0	,	A	,	B,
			0	,	0	,	1	,	0);
	gl::ext::glUniformMatrix4fv(::my::OpenGL::VUL_PROJECTION, 1, GL_TRUE, m.as_float_array_16());
}

///////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////
