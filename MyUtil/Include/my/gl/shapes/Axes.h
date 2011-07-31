#ifndef __MY_UTIL__MY__GL__SHAPES__AXES__H__
#define __MY_UTIL__MY__GL__SHAPES__AXES__H__

#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>
#include <my/gl/shapes/Line.h>
#include <my/gl/shapes/ShapeComposition.h>

#include <PPlaceholder.h>

#define MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION		(100)

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Axes: public ShapeComposition {
	public:
														Axes (void);
		virtual											~Axes (void);

	private:
		typedef codeshare::utilities::Placeholder<Line>	LinePlaceholder;
		typedef LinePlaceholder							LinePlaceholders[3 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION];
#pragma warning( push )
#pragma warning( disable: 4251 ) // Placeholder needs to have dll interface
		LinePlaceholders								lines;
#pragma warning( pop )
		Shape*											shapesArray[sizeof(LinePlaceholders)/sizeof(LinePlaceholder)];
														Axes (Axes const&);
		void											operator = (Axes const&);
	}; // class Axes


}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__AXES__H__

