#ifndef __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__
#define __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__

#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Triangle: public Shape {
	public:
		explicit										Triangle (Colour const& colour = Colour(math::Vector4::New(0.6f, 0.6f, 0.6f, 1.0f)));
														Triangle (Triangle const&);
		virtual											~Triangle (void);

		static size_t									GetTriangleNumberOfVertices (void);
		static size_t									GetTriangleSizeOf (void)
															{ return sizeof(Triangle); }

		// Shape
		VertexData*										GetVertexData (void* memory, size_t bytesize) const;
		Triangle*										Clone (void* memory, size_t bytesize) const;
		size_t											GetNumberOfVertices (void) const
															{ return GetTriangleNumberOfVertices(); }
		size_t											GetSizeOf (void) const
															{ return GetTriangleSizeOf(); }

		Vertex const&									GetA (void) const;
		Vertex const&									GetB (void) const;
		Vertex const&									GetC (void) const;

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	private:
		Vertex											a;
		Vertex											b;
		Vertex											c;

	}; // class Triangle

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__
