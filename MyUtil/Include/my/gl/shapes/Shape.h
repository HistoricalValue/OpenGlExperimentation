#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE__H__

#include "MyUtil.h"
#include "my/gl/shapes/Colour.h"
#include "my/gl/shapes/Transformable.h"
#include "my/gl/shapes/VertexData.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Shape: public Transformable {
	public:
														// memory should be at least as much as NumberOfVertices * sizeof(VertexData)
		virtual VertexData*								GetVertexData (void* memory, size_t bytesize) const = 0;
														// memory should be at least GetSizeOf() bytes
		virtual Shape*									Clone (void* memory, size_t bytesize) const = 0;

		virtual											~Shape (void);

		virtual size_t									GetSizeOf (void) const = 0;
		virtual size_t									GetNumberOfVertices (void) const = 0;

		void											SetColour (Colour const&);
		Colour const&									GetColour (void) const;


	protected:
														Shape (Colour const& colour);
														Shape (Shape const&);

		Colour											colour;
	}; // class Shape

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE__H__
