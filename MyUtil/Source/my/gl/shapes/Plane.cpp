#include <stdafx.h>
#include <my/gl/shapes/NShapesComposition_inl.h>


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
		Base(),
		down(colour),
		up(colour) {
		P_STATIC_ASSERT(sizeof(Plane) == 0
			+ sizeof(Base)
			+ sizeof(up)
			+ sizeof(down))

		Add(&down);
		up.RotateZ(M_PI);
		Add(&up);
	}

	Plane::Plane (Plane const& other):
		Base(),
		down(other.down),
		up(other.up) {
		P_STATIC_ASSERT(sizeof(Plane) == 0
			+ sizeof(Base)
			+ sizeof(up)
			+ sizeof(down))

		Add(&down);
		Add(&up);
	}

	Plane::~Plane (void) {
	}

	Plane* Plane::Clone (void* const here, size_t const bytesize) const {
		Plane* result(NULL);

		if (bytesize >= GetSizeOf())
			result = new(here) Plane(*this);

		return result;
	}

	size_t Plane::GetSizeOf (void) const {
		return sizeof(Plane);
	}

}}} // namespace my::gl::shapes
