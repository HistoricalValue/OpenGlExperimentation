#ifndef __MY_UTIL__MY__GL__SHAPES__SOLID_CUBE__H__
#define __MY_UTIL__MY__GL__SHAPES__SOLID_CUBE__H__


#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>
#include <my/gl/shapes/Colour.h>
#include <my/gl/shapes/Triangle.h>

namespace my { namespace gl { namespace shapes {


	class MYUTIL_API SolidCube: public Shape {

	public:

		explicit										SolidCube (Colour const& colour = Colour(math::Vector4::New(0.6f, 0.6f, 0.6f, 1.0f)));
														SolidCube (SolidCube const&);
		virtual											~SolidCube (void);

		static size_t									GetSolidCubeNumberOfVertices (void);
		static size_t									GetSolidCubeSizeOf (void)
															{ return sizeof(SolidCube); }

		// Shape
		VertexData*										GetVertexData (void* memory, size_t bytesize) const;
		TexturedVertexData*								GetTexturedVertexData (void* memory, size_t bytesize) const;
		SolidCube*										Clone (void* memory, size_t bytesize) const;
		size_t											GetNumberOfVertices (void) const
															{ return GetSolidCubeNumberOfVertices(); }
		size_t											GetSizeOf (void) const
															{ return GetSolidCubeSizeOf(); }


		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	private:
		Triangle										triangle00;
		Triangle										triangle01;
		Triangle										triangle02;
		Triangle										triangle03;
		Triangle										triangle04;
		Triangle										triangle05;
		Triangle										triangle06;
		Triangle										triangle07;
		Triangle										triangle08;
		Triangle										triangle09;
		Triangle										triangle10;
		Triangle										triangle11;

		Triangle*										GetTriangles (void);
		Triangle const*									GetConstTriangles (void) const;
	}; // class SolidCube


}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SOLID_CUBE__H__
