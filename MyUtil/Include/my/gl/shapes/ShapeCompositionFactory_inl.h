#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__

#include <my/gl/shapes/ShapeCompositionFactory.h>
#include <algorithm>
#include <ufunctors.h>
#include <my/gl/shapes/DynamicShapeComposition_inl.h>

namespace my { namespace gl { namespace shapes {

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__SHAPES_COMPOSITION_FACTORY__DEFINING)

P_INLINE
void ShapeCompositionFactory::Add (Shape const& shape) {
	shapes.push_back(Shape::CloneShape(shape));
}

P_INLINE
void ShapeCompositionFactory::Reset (void) {
	std::for_each(shapes.begin(), shapes.end(), uptr_fun(&Shape::DisposeClonedShape));
	shapes.clear();
}

P_INLINE
DynamicShapeComposition* ShapeCompositionFactory::Generate (void) const {
	DynamicShapeComposition* const result(DNEWCLASS(DynamicShapeComposition, (shapes.size())));

	{
		ShapesList::const_iterator const end(shapes.end());
		for (ShapesList::const_iterator i(shapes.begin()); i != end; ++i)
			DPTR(result)->Add(Shape::CloneShape(**i));
	}

	PASSERT(result->IsFull());

	return result;
}

P_INLINE
void ShapeCompositionFactory::Dispose (DynamicShapeComposition* const comp) const {
	DPTR(comp)->ForEachShape(uptr_fun(&Shape::DisposeClonedShape));
	DDELETE(comp);
}

P_INLINE
ShapeCompositionFactory::ShapeCompositionFactory (void) {
}

P_INLINE
ShapeCompositionFactory::~ShapeCompositionFactory (void) {
	Reset();
}

#endif

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__
