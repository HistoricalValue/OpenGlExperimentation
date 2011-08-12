#include <stdafx.h>

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

	// static
	Shape* Shape::CloneShape (Shape const& shape) {
		size_t const bytesize(shape.GetSizeOf());
		return _DNOTNULL(shape.Clone(DNEWARR(util_ui8, bytesize), bytesize));
	}

	// static
	void Shape::DisposeClonedShape (Shape* const shape) {
		shape->~Shape();
		DDELARR(reinterpret_cast<util_ui8* const>(shape));
	}

}}} // namespace my::gl::shapes

