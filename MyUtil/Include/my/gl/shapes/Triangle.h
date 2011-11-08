#ifndef __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__
#define __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__

#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>
#include <PAssert.h>

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Triangle: public Shape {
	public:
		explicit										Triangle (Colour const& colour = Colour(math::Vector4::New(0.6f, 0.6f, 0.6f, 1.0f)));
														Triangle (Triangle const&);
		virtual											~Triangle (void);
														UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(Triangle)

		static size_t									GetTriangleNumberOfVertices (void);
		static size_t									GetTriangleSizeOf (void)
															{ return sizeof(Triangle); }

		// Shape
		VertexData*										GetVertexData (void* memory, size_t bytesize) const;
		TexturedVertexData*								GetTexturedVertexData (void* memory, size_t bytesize) const;

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

		Triangle&										SetColourA (Colour const& col)	{ col_a = col; return *this; }
		Triangle&										SetColourB (Colour const& col)	{ col_b = col; return *this; }
		Triangle&										SetColourC (Colour const& col)	{ col_c = col; return *this; }
		Colour const&									GetColourA (void) const	{ return col_a; }
		Colour const&									GetColourB (void) const	{ return col_b; }
		Colour const&									GetColourC (void) const	{ return col_c; }
		Triangle&										AdjustColourA (float f)	{ SetColourA(GetColourA() * f); return *this; }
		Triangle&										AdjustColourB (float f)	{ SetColourA(GetColourB() * f); return *this; }
		Triangle&										AdjustColourC (float f)	{ SetColourA(GetColourC() * f); return *this; }

		Triangle&										SetAO (size_t const i, float const _ao)
															{ PASSERT(i < 3) ao[i] = _ao; return *this; }

		Triangle&										SetTextCoord (size_t const i, float const s, float const t) {
															PASSERT(i < 3 && 0.0f <= s && s <= 1.0f && 0.0f <= t && t <= 1.0f)
															texcoords[i][0] = s;
															texcoords[i][1] = t;
															return *this;
														}

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	private:
		Vertex											a;
		Vertex											b;
		Vertex											c;
		math::Vector4									na, nb, nc;				// normals
		Colour											col_a, col_b, col_c;	// colour per vertex
		float											ao[3];
		float											texcoords[3][2];
	}; // class Triangle

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__TRIANGLE__H__
