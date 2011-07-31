#ifndef __MY_UTIL__MY__GL__SHAPES__PLANE__H__
#define __MY_UTIL__MY__GL__SHAPES__PLANE__H__

#include <MyUtil.h>
#include <my/gl/shapes/ShapeComposition.h>
#include <PTypes.h>

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Plane: public ShapeComposition {
	public:

		virtual TexturedVertexData*						GetTexturedVertexData (void* memory, size_t bytesize) const;

														Plane (Colour const&);
		virtual											~Plane (void);
	private:
		Triangle										down;
		Triangle										up;
		Shape*											shapesArray[2];
	}; // class Shape

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__PLANE__H__
