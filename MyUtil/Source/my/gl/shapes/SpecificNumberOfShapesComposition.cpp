#include <stdafx.h>

namespace my { namespace gl { namespace shapes {

SpecificNumberOfShapesComposition::SpecificNumberOfShapesComposition (size_t const numberOfShapes):
	ShapeComposition(DNEWARR(Shape*, numberOfShapes), sizeof(Shape*) * numberOfShapes)
	{ }

SpecificNumberOfShapesComposition::~SpecificNumberOfShapesComposition (void) {
	DDELARR(GetShapesArray());
}

Shape** SpecificNumberOfShapesComposition::GetShapesArray (void) const {
	return DPTR(ShapeComposition::GetShapesArray());
}

}}} // namespace my::gl::shapes
