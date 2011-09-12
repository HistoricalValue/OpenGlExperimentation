#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__

#include <my/gl/shapes/ShapeCompositionFactory.h>

namespace my { namespace gl { namespace shapes {

template <typename InputIteratorType>
inline
ShapeCompositionFactory& ShapeCompositionFactory::AddAll (InputIteratorType const& begin, InputIteratorType const& end) {
	std::for_each(
		begin,
		end,
		ubind1st(
			uspecific_mem_fun1<
				void,
				ShapeCompositionFactory,
				Shape const&,
				&ShapeCompositionFactory::Add>(),
			this
		)
	);
	return *this;
}

template <typename ContainerType>
inline
ShapeCompositionFactory& ShapeCompositionFactory::AddAll (ContainerType const& c) {
	return AddAll(c.begin(), c.end());
}

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY_INL__H__
