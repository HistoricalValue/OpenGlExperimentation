#ifndef __MY_UTIL__MY__GL__SHAPES__COLOUR__H__
#define __MY_UTIL__MY__GL__SHAPES__COLOUR__H__

#include "MyUtil.h"
#include "my/gl/math/Vector4.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Colour {
	public:
		explicit										Colour (math::Vector4 const& colour);
														Colour (Colour const&);

		void											operator = (Colour const&);

		float											GetRed (void) const;
		float											GetGreen (void) const;
		float											GetBlue (void) const;
		float											GetAlpha (void) const;

		Colour											operator * (math::Vector4 const& mod) const;

		math::Vector4									operator - (Colour const&) const;
		Colour											operator + (math::Vector4 const&) const;
		bool											operator < (Colour const&) const;
		bool											operator == (Colour const&) const;


	private:

		math::Vector4									colour;
	}; // class Colour



}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__COLOUR__H__
