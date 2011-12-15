#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__

#include <my/gl/shapes/Shape.h>
#include <MyUtil.h>

namespace my { namespace gl { namespace shapes {

namespace ShapeUtilities {

	extern MYUTIL_API void	Initialise (void);
	extern MYUTIL_API void	CleanUp (void);
	extern MYUTIL_API void*	_ShapeAlloc (size_t);

	extern MYUTIL_API void	Reset (void);
	extern MYUTIL_API void	DisposeClonedShape (Shape* shape);

	template <typename TShape>
	static inline TShape*		CloneShape (TShape const& shape) {
									size_t const bytesize(shape.GetSizeOf());
									return DNULLCHECK(shape.Clone(_ShapeAlloc(bytesize), bytesize));
								}

}

}}}

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE_UTILITIES__H__
