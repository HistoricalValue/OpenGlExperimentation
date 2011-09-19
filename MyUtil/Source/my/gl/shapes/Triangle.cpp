#include <stdafx.h>

#include <PPointerUtilities_inl.h>

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
		using			codeshare::utilities::pointer_utilities::reinterpret_assign;
		using			math::Vector4;
		VertexData*		result				(NULL);
		size_t const	requiredBytesize	(3 * sizeof(VertexData));

		Vector4 const	normal				((b.xyzw().sub_asvec3(a.xyzw())).cross_asvec3(c.xyzw().sub_asvec3(a.xyzw())).normalised());
		DASSERT(abs(normal.magnitude() - 1.0f) < 1e-6);

		Colour const	colour1				(ColourFactory::Dimmer(colour));
		Colour const	colour2				(colour);
		Colour const	colour3				(ColourFactory::Brighter(colour));

		
		if (bytesize >= requiredBytesize) {
			reinterpret_assign(result, memory);

			new(&result[0]) VertexData(a.xyzw(), colour1, normal);
			new(&result[1]) VertexData(b.xyzw(), colour2, normal);
			new(&result[2]) VertexData(c.xyzw(), colour3, normal);
		}

		return result;
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
