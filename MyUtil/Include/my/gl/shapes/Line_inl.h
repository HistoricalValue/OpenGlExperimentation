#ifndef __MY_UTIL__MY__GL__SHAPES__LINE_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__LINE_INL__H__

#include "my/gl/shapes/Line.h"
#include "PConfigurations.h"

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__LINE__DEFINING)

#include "my/gl/shapes/Vertex_inl.h"

namespace my { namespace gl { namespace shapes {

		P_INLINE
		Line::Line (Vertex const& _beginning, Vertex const& _end, Colour const& _beginningColour, Colour const& _endColour):
			Shape(_beginningColour),
			beginning(_beginning),
			end(_end),
			beginningColour(_beginningColour),
			endColour(_endColour)
		{
			P_STATIC_ASSERT(sizeof(Line) == 0
					+ sizeof(Shape)
					+ sizeof(beginning)
					+ sizeof(end)
					+ sizeof(beginningColour)
					+ sizeof(endColour)
				)
		}

		P_INLINE
		Line::Line (Line const& other):
			Shape(other),
			beginning(other.beginning),
			end(other.end),
			beginningColour(other.beginningColour),
			endColour(other.endColour)
		{
			P_STATIC_ASSERT(sizeof(Line) == 0
					+ sizeof(Shape)
					+ sizeof(beginning)
					+ sizeof(end)
					+ sizeof(beginningColour)
					+ sizeof(endColour)
				)
		}

		P_INLINE
		Line::~Line (void) {
		}

		P_INLINE
		Vertex const& Line::GetBeginning (void) const {
			return beginning;
		}

		P_INLINE
		Vertex const& Line::GetEnd (void) const {
			return end;
		}
		
		P_INLINE
		void Line::SetBeginningColour (Colour const& colour) {
			beginningColour = colour;
		}
		
		P_INLINE
		void Line::SetEndColour (Colour const& colour) {
			endColour = colour;
		}
		
		P_INLINE
		Colour const& Line::GetBeginningColour (void) const {
			return beginningColour;
		}
		
		P_INLINE
		Colour const& Line::GetEndColour (void) const {
			return endColour;
		}

		// Transformable
		// -=-----------
		P_INLINE
		void Line::Apply (math::Matrix4x4 const& mat4) {
			beginning.Apply(mat4);
			end.Apply(mat4);
		}

		P_INLINE
		void Line::Adjust (math::Vector4 const& vec4) {
			beginning.Adjust(vec4);
			end.Adjust(vec4);
		}

}}} // namespace my::gl::shapes

#endif


#endif // __MY_UTIL__MY__GL__SHAPES__LINE_INL__H__
