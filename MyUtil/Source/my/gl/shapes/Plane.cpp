#include "stdafx.h"


namespace my { namespace gl { namespace shapes {

	TexturedVertexData* Plane::GetTexturedVertexData (void* const memory, size_t const bytesize) const {
		using				codeshare::utilities::pointer_utilities::reinterpret_assign;
		using				math::Vector4;
		TexturedVertexData*	result				(NULL);
		size_t const		requiredBytesize	(Triangle::GetTriangleNumberOfVertices() * 2 * sizeof(TexturedVertexData));

		if (bytesize >= requiredBytesize) {
			reinterpret_assign(result, memory);

			PASSERT(requiredBytesize / sizeof(TexturedVertexData) == 6);

			new(&result[0]) TexturedVertexData(down.GetA().xyzw(), Vector4::New( 0.f,  0.f));
			new(&result[1]) TexturedVertexData(down.GetB().xyzw(), Vector4::New( 1.f,  0.f));
			new(&result[2]) TexturedVertexData(down.GetC().xyzw(), Vector4::New( 0.f,  1.f));

			new(&result[3]) TexturedVertexData(  up.GetA().xyzw(), Vector4::New( 1.f,  1.f));
			new(&result[4]) TexturedVertexData(  up.GetB().xyzw(), Vector4::New( 0.f,  1.f));
			new(&result[5]) TexturedVertexData(  up.GetC().xyzw(), Vector4::New( 1.f,  0.f));
		}

		return result;
	}

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
