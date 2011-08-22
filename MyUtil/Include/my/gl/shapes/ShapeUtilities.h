#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__

#include <my/gl/shapes/Shape.h>
#include <DDebug.h>

namespace my { namespace gl { namespace shapes {

namespace ShapeUtilities {
	inline void								DisposeClonedShape (Shape* const shape) {
												DPTR(_DNOTNULL(shape))->~Shape();
												DDELARR(reinterpret_cast<util_ui8* const>(shape));
											}

	template <typename TShape>
	inline TShape*							CloneShape (TShape const& shape) {
												size_t const bytesize(shape.GetSizeOf());
												return DPTR(_DNOTNULL(shape.Clone(DNEWARR(util_ui8, bytesize), bytesize)));
											}

}

}}}

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__
