#ifndef __MY_UTIL__MY__GL__SHAPES__CUBE__H__
#define __MY_UTIL__MY__GL__SHAPES__CUBE__H__

#include "MyUtil.h"
#include "my/gl/shapes/Transformable.h"
#include "my/gl/shapes/Line.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Cube: public Shape {
	public:
														Cube (Colour const& colour = Colour(math::Vector4::New(0.6f, 0.6f, 0.6f, 1.0f)));
														Cube (Cube const&);
	virtual												~Cube (void);

	static size_t										GetCubeNumberOfVertices (void);
	static size_t										GetCubeSizeOf (void)
															{ return sizeof(Cube); }

	// Shape
	VertexData*											GetVertexData (void* memory, size_t bytesize) const;
	Cube*												Clone (void* memory, size_t bytesize) const;
	size_t												GetNumberOfVertices (void) const
															{ return GetCubeNumberOfVertices(); }
	size_t												GetSizeOf (void) const
															{ return GetCubeSizeOf(); }
	
	// Transformable
	void												Apply (my::gl::math::Matrix4x4 const&);
	void												Adjust (math::Vector4 const&);


	private:
		Line											line00;
		Line											line01;
		Line											line02;
		Line											line03;
		Line											line04;
		Line											line05;
		Line											line06;
		Line											line07;
		Line											line08;
		Line											line09;
		Line											line10;
		Line											line11;

		Line*											GetLines (void);
		Line const*										GetConstLines (void) const;
	};

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__CUBE__H__