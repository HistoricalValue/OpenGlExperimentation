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
		Triangle&										SetA (Vertex const& _a) { a = _a; return *this; }
		Triangle&										SetB (Vertex const& _b) { b = _b; return *this; }
		Triangle&										SetC (Vertex const& _c) { c = _c; return *this; }
		Triangle&										RecomputeNormal (void);
		Triangle&										SetNormals (math::Vector4 const& _na,
																	math::Vector4 const& _nb,
																	math::Vector4 const& _nc)
															{ return na = _na, nb = _nb, nc = _nc, *this;}

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	private:
		Vertex											a;
		Vertex											b;
		Vertex											c;
		math::Vector4									na, nb, nc; // normals
	}; // class Triangle

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__
