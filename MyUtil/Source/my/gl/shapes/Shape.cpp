#include "stdafx.h"

namespace my { namespace gl { namespace shapes {

	Shape::Shape (Colour const& _colour):
		colour(_colour) {
		P_STATIC_ASSERT(
			sizeof(Shape) == 0
				+ sizeof(void*)			// vt
				+ sizeof(colour)
			)
	}

	Shape::Shape (Shape const& other):
		colour(other.colour) {
		P_STATIC_ASSERT(
			sizeof(Shape) == 0
				+ sizeof(void*)			// vt
				+ sizeof(colour)
			)
	}

	Shape::~Shape (void) {
	}

	void Shape::SetColour (Colour const& _colour) {
		colour = _colour;
	}

}}} // namespace my::gl::shapes

