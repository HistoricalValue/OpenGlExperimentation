#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__

#include "MyUtil.h"
#include "my/gl/shapes/Vertex.h"
#include "my/gl/shapes/Colour.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API VertexData {
	public:
														VertexData (math::Vector4 const&, Colour const&);
														VertexData (VertexData const&);
														~VertexData (void);

		math::Vector4 const&							GetPosition (void) const;
		Colour const&									GetColour (void) const;


		static size_t									PositionOffset (void);
		static size_t									ColourOffset (void);
		static void*									PositionOffsetPointer (void);
		static void*									ColourOffsetPointer (void);
		static size_t									Stride (void);
	private:
		math::Vector4 const								position;
		Colour const									colour;

		static size_t									Offset (size_t);
	}; // class VertexData

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__