#ifndef __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION_INL__H__

#include <my/gl/shapes/DynamicShapeComposition.h>

namespace my { namespace gl { namespace shapes {

template <typename F>
P_INLINE
void DynamicShapeComposition::ForEachShape (F const& f) const {
	size_t const capacity(GetCapacity());
	PASSERT(IsFull())
	PASSERT(capacity == GetNumberOfAddedShapes())

	Shape** const shapes = GetShapesArray(); // stupid microsoft: cannot handle c-tor initialisation syntax, even with extra parentheses?

	std::for_each(&shapes[0], &shapes[capacity], f);
}

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__DEFINING)

P_INLINE
size_t	DynamicShapeComposition::GetNumberOfShapes (void) const {
	size_t const capacity(GetCapacity());
	PASSERT(IsFull());
	PASSERT(capacity == GetNumberOfAddedShapes())
	return capacity;
}

P_INLINE
DynamicShapeComposition::DynamicShapeComposition (size_t const numberOfShapes):
	ShapeComposition(DNEWARR(Shape*, numberOfShapes), numberOfShapes * sizeof(Shape*))
	{ }

P_INLINE
DynamicShapeComposition::~DynamicShapeComposition (void) {
	DDELARR(GetShapesArray());
}


#endif

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION_INL__H__
