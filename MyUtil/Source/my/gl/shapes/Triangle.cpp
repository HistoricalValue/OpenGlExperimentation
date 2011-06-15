#include "stdafx.h"


namespace _ {
	static const size_t numberOfVertices(0x3u);
}

namespace my { namespace gl { namespace shapes {


	Triangle::Triangle (Colour const& colour):
		Shape(colour),
		a(math::Vector4::New(-1.0f, -1.0f, -1.0f, 1.0f)),
		b(math::Vector4::New( 1.0f, -1.0f, -1.0f, 1.0f)),
		c(math::Vector4::New(-1.0f,  1.0f, -1.0f, 1.0f))
		{ P_STATIC_ASSERT(sizeof(Triangle) == 0
				+ sizeof(Shape)
				+ sizeof(a)
				+ sizeof(b)
				+ sizeof(c)) }

	Triangle::Triangle (Triangle const& other):
		Shape(other),
		a(other.a),
		b(other.b),
		c(other.c)
		{ P_STATIC_ASSERT(sizeof(Triangle) == 0
				+ sizeof(Shape)
				+ sizeof(a)
				+ sizeof(b)
				+ sizeof(c)) }

	Triangle::~Triangle (void) {
	}

	VertexData* Triangle::GetVertexData (void* const memory, size_t const bytesize) const {
		PASSERT(!"Not implemented")
		return NULL;
	}

	Triangle* Triangle::Clone (void* memory, size_t size) const {
		Triangle* result(NULL);

		if (size >= sizeof(Triangle)) {
			Triangle* const triangle(reinterpret_cast<Triangle* const>(memory));

			new(triangle) Triangle(*this);

			result = triangle;
		}

		return result;
	}

	
	void Triangle::Apply (math::Matrix4x4 const& mat4) {
		a.Apply(mat4);
		b.Apply(mat4);
		c.Apply(mat4);
	}

	void Triangle::Adjust (math::Vector4 const& vec4) {
		a.Adjust(vec4);
		b.Adjust(vec4);
		c.Adjust(vec4);
	}

	size_t Triangle::GetTriangleNumberOfVertices (void) {
		return _::numberOfVertices;
	}

	Vertex const& Triangle::GetA (void) const {
		return a;
	}

	Vertex const& Triangle::GetB (void) const {
		return b;
	}

	Vertex const& Triangle::GetC (void) const {
		return c;
	}

}}} // namespace my::gl::shapes
