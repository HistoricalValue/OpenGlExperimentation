#ifndef __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION_INL__H__
#define __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION_INL__H__


#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__DEFINING)

#include "my/gl/shapes/ShapeComposition.h"
#include "PConfigurations.h"
#include "PTypes.h"

namespace my { namespace gl { namespace shapes {

	using namespace codeshare::utilities::types;


	P_INLINE
	bool ShapeComposition::IsFull (void) const {
		return i == shapesBytesize/sizeof(shapes[0]);
	}

	P_INLINE
	void ShapeComposition::Add (Shape* const shape) {
		PASSERT(!IsFull())
		if (!IsFull())
			shapes[i++] = shape;
	}


	// Transformable
	P_INLINE
	void ShapeComposition::Apply (math::Matrix4x4 const& mat4) {
		PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

		ShapeComposition const& self(*this);

		for (Shape* const* shape = &self.shapes[0]; shape < &self.shapes[self.i]; ++shape)
			(*shape)->Apply(mat4);
	}

	P_INLINE
	void ShapeComposition::Adjust (math::Vector4 const& vec4) {
		PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

		ShapeComposition const& self(*this);

		for (Shape* const* shape = &self.shapes[0]; shape < &self.shapes[self.i]; ++shape)
			(*shape)->Adjust(vec4);
	}

}}} // namespace my::gl::shapes

#endif

#endif // __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION_INL__H__
