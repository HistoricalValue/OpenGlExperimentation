#include <stdafx.h>

#include <PPointerUtilities_inl.h>
#include <my/gl/math/Vector4_inl.h>

namespace _ {
	static const size_t numberOfVertices(0x3u);
}

namespace my { namespace gl { namespace shapes {


	Triangle::Triangle (Colour const& colour):
		Shape(colour),
		a(math::Vector4::New(-1.0f, -1.0f, -1.0f, 1.0f)),
		b(math::Vector4::New( 1.0f, -1.0f, -1.0f, 1.0f)),
		c(math::Vector4::New(-1.0f,  1.0f, -1.0f, 1.0f)),
		na(math::Vector4::New(9999.0f, 9999.0f, 9999.0f, 1.0f)),
		nb(math::Vector4::New(9999.0f, 9999.0f, 9999.0f, 1.0f)),
		nc(math::Vector4::New(9999.0f, 9999.0f, 9999.0f, 1.0f)),
		col_a(colour),
		col_b(colour),
		col_c(colour)
		{ P_STATIC_ASSERT(sizeof(Triangle) == 0
				+ sizeof(Shape)
				+ sizeof(a)
				+ sizeof(b)
				+ sizeof(c)
				+ sizeof(na)
				+ sizeof(nb)
				+ sizeof(nc)
				+ sizeof(col_a)
				+ sizeof(col_b)
				+ sizeof(col_c)
				)
		}

	Triangle::Triangle (Triangle const& other):
		Shape(other),
		a(other.a),
		b(other.b),
		c(other.c),
		na(other.na),
		nb(other.nb),
		nc(other.nc),
		col_a(other.col_a),
		col_b(other.col_b),
		col_c(other.col_c)
		{ P_STATIC_ASSERT(sizeof(Triangle) == 0
				+ sizeof(Shape)
				+ sizeof(a)
				+ sizeof(b)
				+ sizeof(c)
				+ sizeof(na)
				+ sizeof(nb)
				+ sizeof(nc)
				+ sizeof(col_a)
				+ sizeof(col_b)
				+ sizeof(col_c)
				)
		}

	Triangle::~Triangle (void) {
	}

	VertexData* Triangle::GetVertexData (void* const memory, size_t const bytesize) const {
		using			codeshare::utilities::pointer_utilities::reinterpret_assign;
		using			math::Vector4;
		VertexData*		result				(NULL);
		size_t const	requiredBytesize	(3 * sizeof(VertexData));

		PASSERT(abs(na.magnitude() - 1.0f) < 1e-6)
		PASSERT(abs(nb.magnitude() - 1.0f) < 1e-6)
		PASSERT(abs(nc.magnitude() - 1.0f) < 1e-6)
		
		if (bytesize >= requiredBytesize) {
			reinterpret_assign(result, memory);

			new(&result[0]) VertexData(a.xyzw(), col_a, na);
			new(&result[1]) VertexData(b.xyzw(), col_b, nb);
			new(&result[2]) VertexData(c.xyzw(), col_c, nc);
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

	Triangle& Triangle::RecomputeNormal (void) {
		PASSERT(!(a == b))
		PASSERT(!(a == c))
		PASSERT(!(b == c))
		na = ((b.xyzw().sub_asvec3(a.xyzw())).cross_asvec3(c.xyzw().sub_asvec3(a.xyzw())).normalised());
		nb = na;
		nc = nb;
		return *this;
	}
}}} // namespace my::gl::shapes
