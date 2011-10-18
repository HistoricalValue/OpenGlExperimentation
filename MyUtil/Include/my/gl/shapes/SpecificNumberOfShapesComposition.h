#ifndef __MY_UTIL__MY__GL__SHAPES__SPECIFIC_NUMBER_OF_SHAPES_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPES__SPECIFIC_NUMBER_OF_SHAPES_COMPOSITION__H__

#include <MyUtil.h>
#include <my/gl/shapes/ShapeComposition.h>

namespace my { namespace gl { namespace shapes {

// abstract
class MYUTIL_API SpecificNumberOfShapesComposition: public ShapeComposition {
protected:
					SpecificNumberOfShapesComposition (size_t numberOfShapes);
	virtual			~SpecificNumberOfShapesComposition (void);

	Shape**			GetShapesArray (void) const;
private:
	SpecificNumberOfShapesComposition (SpecificNumberOfShapesComposition const&);
	void operator = (SpecificNumberOfShapesComposition const&);
}; // SpecificNumberOfShapesComposition

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SPECIFIC_NUMBER_OF_SHAPES_COMPOSITION__H__
