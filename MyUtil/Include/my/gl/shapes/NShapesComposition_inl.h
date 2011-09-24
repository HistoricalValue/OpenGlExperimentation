#ifndef __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION_INL__H__

#include <MyUtil.h>
#include <my/gl/shapes/NShapesComposition.h>
#include <PAssert.h>

///////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

///////////////////////////////////////////////////////////

template <const size_t N>
NShapesComposition<N>* NShapesComposition<N>::Clone (void* const, size_t const) const {
	PASSERT(!"Not allowed")
	return NULL;
}

///////////////////////////////////////////////////////////

template <const size_t N>
size_t NShapesComposition<N>::GetSizeOf (void) const {
	PASSERT(!"Not allowed")
	return size_t(-1);
}

///////////////////////////////////////////////////////////

template <const size_t N>
NShapesComposition<N>::NShapesComposition (void):
	SpecificNumberOfShapesComposition(N)
	{ }

///////////////////////////////////////////////////////////

template <const size_t N>
NShapesComposition<N>::~NShapesComposition (void) {
}

///////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

///////////////////////////////////////////////////////////

#endif // __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION_INL__H__
