#include <stdafx.h>

#include <my/gl/shapes/ColourFactory.h>

namespace _ {

	P_INLINE
	static float module1 (float const n) {
		return
				n > 1.f?	n - floorf(n):
				n < 0.f?	1.f + n + floorf(-n):
							n;
	}

} // namespace _


namespace my { namespace gl { namespace shapes {


	Colour ColourFactory::New (float const r, float const g, float const b, float const a) {
		return Colour(math::Vector4::New(r, g, b, a));
	}


	math::Vector4 ColourFactory::DimmingFactor (void) {
		return math::Vector4::New(.6f, .6f, .6f, 1.f);
	}


	Colour ColourFactory::Red (void) {
		return New(1.f, 0.f, 0.f);
	}


	Colour ColourFactory::Green (void) {
		return New(0.f, 1.f, 0.f);
	}


	Colour ColourFactory::Blue (void) {
		return New(0.f, 0.f, 1.f);
	}

	Colour ColourFactory::Yellow (void) {
		return New(1.f, 1.f, 0.f);
	}

	Colour ColourFactory::Tyal (void) {
		return New(0.f, 1.f, 1.f);
	}

	Colour ColourFactory::Purple (void) {
		return New(1.f, 0.f, 1.f);
	}

	Colour ColourFactory::White (void) {
		return New(1.f, 1.f, 1.f);
	}

	Colour ColourFactory::Black(void) {
		return New(0.f, 0.f, 0.f);
	}
	Colour ColourFactory::LightRed (void) {
		return Red() * DimmingFactor();
	}


	Colour ColourFactory::LightGreen (void) {
		return Green() * DimmingFactor();
	}


	Colour ColourFactory::LightBlue (void) {
		return Blue() * DimmingFactor();
	}

	Colour ColourFactory::LightYellow (void) {
		return Yellow() * DimmingFactor();
	}

	Colour ColourFactory::LightTyal (void) {
		return Tyal() * DimmingFactor();
	}

	Colour ColourFactory::LightPurple (void) {
		return Purple() * DimmingFactor();
	}

	Colour ColourFactory::LightBlack (void) {
		return New(.4f, .4f, .4f, 1.f);
	}

	Colour ColourFactory::LightWhite (void) {
		return White() * DimmingFactor();
	}

	Colour ColourFactory::Dimmer (Colour const& original) {
		return original * DimmingFactor();
	}

	math::Vector4 ColourFactory::LighteningFactor (void) {
		return math::Vector4::New(1.2f, 1.2f, 1.2f, 1.f);
	}

	Colour ColourFactory::Brighter (Colour const& original) {
		return original * LighteningFactor();
	}

	Colour ColourFactory::Next (Colour const& original) {
		return New(original.GetBlue(), original.GetRed(), _::module1(original.GetGreen() + 1.3f), original.GetAlpha());
	}

	Colour ColourFactory::Previous (Colour const& original) {
		return New(original.GetGreen(), _::module1(original.GetBlue() - 1.3f), original.GetRed(), original.GetAlpha());
	}

}}} // namespace my::gl::shape
