#include <stdafx.h>

#include <my/gl/shapes/Transformable.h>

namespace my { namespace gl { namespace shapes {

	Transformable::Transformable (void) {
		const size_t s(sizeof(Transformable));
		P_STATIC_ASSERT(
				sizeof(Transformable) == sizeof(void*) // vt
			)
	}

	Transformable::Transformable (Transformable const&) {
		P_STATIC_ASSERT(
				sizeof(Transformable) == sizeof(void*) // vt
			)
	}

	Transformable::~Transformable (void) {
	}

	void Transformable::Scale (float const factor) {
		ScaleX(factor);
		ScaleY(factor);
		ScaleZ(factor);
	}

	void Transformable::ScaleX (float const factor) {
		Apply(my::gl::math::Matrix4x4(
				factor,	0.f,	0.f,	0.f,
				0.f,	1.f,	0.f,	0.f,
				0.f,	0.f,	1.f,	0.f,
				0.f,	0.f,	0.f,	1.f
			));
	}

	void Transformable::ScaleY (float const factor) {
		Apply(my::gl::math::Matrix4x4(
				1.f,	0.f,	0.f,	0.f,
				0.f,	factor,	0.f,	0.f,
				0.f,	0.f,	1.f,	0.f,
				0.f,	0.f,	0.f,	1.f
			));
	}

	void Transformable::ScaleZ (float const factor) {
		Apply(my::gl::math::Matrix4x4(
				1.f,	0.f,	0.f,	0.f,
				0.f,	1.f,	0.f,	0.f,
				0.f,	0.f,	factor,	0.f,
				0.f,	0.f,	0.f,	1.f
			));
	}

	void Transformable::ScaleW (float const factor) {
		Apply(my::gl::math::Matrix4x4(
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, factor
			));
	}

	void Transformable::RotateX (float const radians) {
		float const cosine(cos(-radians));
		float const sine(sin(-radians));
		Apply(my::gl::math::Matrix4x4(
				1.f,	0.f,		0.f,		0.f,
				0.f,	cosine,		sine,		0.f,
				0.f,	-sine,		cosine,		0.f,
				0.f,	0.f,		0.f,		1.f
			));
	}

	void Transformable::RotateY (float const radians) {
		float const cosine(cos(-radians));
		float const sine(sin(-radians));
		Apply(my::gl::math::Matrix4x4(
				cosine,	0.f,	-sine,		0.f,
				0.f,	1.f,	0.f,		0.f,
				sine,	0.f,	cosine,		0.f,
				0.f,	0.f,	0.f,		1.f
			));
	}

	void Transformable::RotateZ (float const radians) {
		float const cosine(cos(-radians));
		float const sine(sin(-radians));
		Apply(my::gl::math::Matrix4x4(
				cosine,		sine,	0.f,	0.f,
				-sine,		cosine,	0.f,	0.f,
				0.f,		0.f,	1.f,	0.f,
				0.f,		0.f,	0.f,	1.f
			));
	}

	void Transformable::TranslateX (float const d) {
		Adjust(math::Vector4::New(d, 0.f, 0.f, 0.f));
	}

	void Transformable::TranslateY (float const d) {
		Adjust(math::Vector4::New(0.f, d, 0.f, 0.f));
	}

	void Transformable::TranslateZ (float const d) {
		Adjust(math::Vector4::New(0.f, 0.f, d, 0.f));
	}

}}} // namespace my::gl::shapes