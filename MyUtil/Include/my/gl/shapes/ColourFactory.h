#ifndef __MY_UTIL__MY__GL__SHAPES__COLOUR_FACTORY__H__
#define __MY_UTIL__MY__GL__SHAPES__COLOUR_FACTORY__H__

#include "MyUtil.h"
#include "my/gl/shapes/Colour.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API ColourFactory {

	public:

		static Colour									New (float r = .6f, float g = .6f, float b = .6f, float a = 1.f);

		static Colour									LightRed (void);
		static Colour									LightGreen (void);
		static Colour									LightBlue (void);
		static Colour									LightYellow (void);
		static Colour									LightTyal (void);
		static Colour									LightPurple (void);
		static Colour									LightWhite (void);
		static Colour									LightBlack (void);
		static Colour									Red (void);
		static Colour									Green (void);
		static Colour									Blue (void);
		static Colour									Yellow (void);
		static Colour									Tyal (void);
		static Colour									Purple (void);
		static Colour									White (void);
		static Colour									Black (void);

		static math::Vector4							DimmingFactor (void);
		static Colour									Dimmer (Colour const& original);

		static math::Vector4							LighteningFactor (void);
		static Colour									Brighter (Colour const& original);

		static Colour									Next (Colour const& previous);
		static Colour									Previous (Colour const& next);

	}; // class ColourFactory

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__COLOUR_FACTORY__H__