#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY__H__

#include <MyUtil.h>
#include <my/gl/shapes/DynamicShapeComposition.h>
#include <my/gl/shapes/Shape.h>

namespace my { namespace gl { namespace shapes {



class MYUTIL_API ShapeCompositionFactory {
public:
	void						Add (Shape const&);
	DynamicShapeComposition*	Generate (void) const;
	void						Reset (void);

	void						Dispose (DynamicShapeComposition*) const;

								ShapeCompositionFactory (void);
								~ShapeCompositionFactory (void);

private:
	void* const data;
};

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE_COMPOSITION_FACTORY__H__
