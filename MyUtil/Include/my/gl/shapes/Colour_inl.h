#ifndef __MY_UTIL__MY__GL__SHAPES__COLOUR_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__COLOUR_INL__H__

#include <my/gl/shapes/Colour.h>
#include <PConfigurations.h>
#include <PStaticAssert.h>
#include <my/gl/shapes/Shape.h>

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__COLOUR__DEFINING)

namespace my { namespace gl { namespace shapes {

	P_INLINE
	Colour::Colour (math::Vector4 const& _colour):
		colour(_colour) {
		P_STATIC_ASSERT(sizeof(Colour) == sizeof(colour))
	}

	///////////////////////////////////

	P_INLINE
	Colour::Colour (Colour const& colour):
		colour(colour.colour) {
		P_STATIC_ASSERT(sizeof(Colour) == sizeof(colour))
	}

	///////////////////////////////////

	P_INLINE
	float Colour::GetRed (void) const {
		return colour.r();
	}

	///////////////////////////////////

	P_INLINE
	float Colour::GetGreen (void) const {
		return colour.g();
	}

	///////////////////////////////////

	P_INLINE
	float Colour::GetBlue (void) const {
		return colour.b();
	}

	///////////////////////////////////

	P_INLINE
	float Colour::GetAlpha (void) const {
		return colour.a();
	}

	///////////////////////////////////

	P_INLINE
	Colour Colour::operator * (math::Vector4 const& mod) const {
		return Colour(colour.mul(mod));
	}

	///////////////////////////////////

	P_INLINE
	void Colour::operator = (Colour const& other) {
		colour = other.colour;
	}

	///////////////////////////////////

	P_INLINE
	Colour const& Shape::GetColour (void) const {
		return colour;
	}

	///////////////////////////////////

	P_INLINE
	math::Vector4 Colour::operator - (Colour const& other) const {
		return colour - other.colour;
	}

	///////////////////////////////////

	P_INLINE
	Colour Colour::operator + (math::Vector4 const& d) const {
		return Colour(colour + d);
	}

	///////////////////////////////////

	P_INLINE
	bool Colour::operator < (Colour const& other) const {
		return
				(colour.r() + colour.g() + colour.b()) * colour.a() <
				(other.colour.r() + other.colour.g() + other.colour.b()) * other.colour.a();
	}

	///////////////////////////////////

}}} // namespace my::gl::shapes

#endif

#endif // __MY_UTIL__MY__GL__SHAPES__COLOUR_INL__H__
