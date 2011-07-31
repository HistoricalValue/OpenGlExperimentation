#ifndef __MY_UTIL__MY__GL__SHAPES__LINE__H__
#define __MY_UTIL__MY__GL__SHAPES__LINE__H__

#include <MyUtil.h>
#include <my/gl/shapes/Vertex.h>
#include <my/gl/shapes/Shape.h>
#include <my/gl/shapes/ColourFactory.h>

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Line: public Shape {
	public:
														Line (
																Vertex const& beginning,
																Vertex const& end,
																Colour const& beginningColour	= ColourFactory::New(0.6f, 0.6f, 0.6f, 1.f),
																Colour const& endColour			= ColourFactory::New(0.9f, 0.9f, 0.9f, 1.f));
														Line (Line const&);
		virtual											~Line (void);

		static size_t									GetLineNumberOfVertices (void);
		static size_t									GetLineSizeOf (void)
															{ return sizeof(Line); }

		Vertex const&									GetBeginning (void) const;
		Vertex const&									GetEnd (void) const;

		Colour const&									GetBeginningColour (void) const;
		Colour const&									GetEndColour (void) const;

		void											SetBeginningColour (Colour const&);
		void											SetEndColour (Colour const&);


		// Shape
		VertexData*										GetVertexData (void* memory, size_t bytesize) const;
		Line*											Clone (void* memory, size_t bytesize) const;
		size_t											GetNumberOfVertices (void) const
															{ return GetLineNumberOfVertices(); }
		size_t											GetSizeOf (void) const
															{ return GetLineSizeOf(); }

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	private:
		Vertex											beginning;
		Vertex											end;
		Colour											beginningColour;
		Colour											endColour;
	};

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__LINE__H__
