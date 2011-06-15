#include "stdafx.h"


namespace my { namespace gl { namespace shapes { 

	Plane::Plane (Colour const& colour):
		ShapeComposition(shapesArray, sizeof(shapesArray)),
		down(colour),
		up(colour),
#pragma warning( push )
#pragma warning( disable: 4351 )
		shapesArray() {
#pragma warning( pop )
		P_STATIC_ASSERT(sizeof(Plane) == 0
			+ sizeof(ShapeComposition)
			+ sizeof(up)
			+ sizeof(down)
			+ sizeof(shapesArray))

		Add(&down);
		up.RotateZ(M_PI);
		Add(&up);
	}

	Plane::~Plane (void) {
	}

}}} // namespace my::gl::shapes
