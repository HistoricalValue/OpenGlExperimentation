#ifndef __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__

#include <MyUtil.h>
#include <my/gl/shapes/ShapeComposition.h>

namespace my { namespace gl { namespace shapes {

class MYUTIL_API DynamicShapeComposition: public ShapeComposition {
public:
	size_t								GetNumberOfShapes (void) const;
	template <typename F>
	void								ForEachShape (F const& f) const;

	virtual DynamicShapeComposition*	Clone (void*, size_t) const;

										DynamicShapeComposition (size_t numberOfShapes);
	virtual								~DynamicShapeComposition (void);
};

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__
